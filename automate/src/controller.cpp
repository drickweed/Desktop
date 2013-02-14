#include "controller.h"

#include <QXmlInputSource>

#include <jb/std/memory.h>
#include <jb/std/log.h>
#include "config.h"
#include "ftpdownloader.h"
#include "httpdownloader.h"
#include "tweakindexhandler.h"

Controller::Controller(QObject *parent):
	QObject(parent),
	m_abort(false),
	m_busy(false) {
	speedUpdateTimer = new QTimer(this);
	speedUpdateTimer->setInterval(WAIT_FOR_DOWNLOAD_DATA);
	connectController();
}

Controller::~Controller() {

}

std::vector<Tweak> Controller::loadTweaks() {
	TweakIndexHandler *handler = new TweakIndexHandler;

	QFile *fh = new QFile(QString((CONFIG.database() + "/tweaks/tweaks.xml").c_str()));
	if (fh->exists()) {
		LOG("Loading tweaks")
		QXmlInputSource *src = new QXmlInputSource(fh);
		QXmlSimpleReader *reader = new QXmlSimpleReader;
		reader->setContentHandler(handler);
		reader->parse(src);
	}
	return handler->tweaks();
}

bool sortVersionByInstallerType(Version i, Version j) {
	if (CONFIG.silentFirst()) {
		if (i.silent()) return false;
	} else {
		if (j.silent()) return false;
	}
	return true;
}

void Controller::processApplications(std::vector<Version> versions, bool download_only) {
	m_versions = versions;
	std::sort(m_versions.begin(), m_versions.end(), sortVersionByInstallerType);
	m_download_only = download_only;
	prepare();
	installNext();
}

void Controller::processTweaks(std::vector<TweakAction> tweaks) {
	LOG("Started applying tweaks")
	m_tweaks = tweaks;
	prepare();
	applyNext();
}

void Controller::prepare() {
	m_abort = false;
	m_busy = true;
	speedUpdateTimer->start();
	emit started();
}

void Controller::done() {
	m_busy = false;
	speedUpdateTimer->stop();
}

void Controller::connectController() {
	connect(&ftp_downloader, SIGNAL(downloadFinished(bool)), SLOT(install(bool)));
	connect(&ftp_downloader, SIGNAL(dataReadProgress(int, int)), SIGNAL(dataReadProgress(int, int)));
	connect(&ftp_downloader, SIGNAL(currentDownloadFilename(QString)), SIGNAL(currentProcess(QString)));
	connect(&http_downloader, SIGNAL(downloadFinished(bool)), SLOT(install(bool)));
	connect(&http_downloader, SIGNAL(dataReadProgress(int, int)), SIGNAL(dataReadProgress(int, int)));
	connect(&http_downloader, SIGNAL(currentDownloadFilename(QString)), SIGNAL(currentProcess(QString)));
	connect(this, SIGNAL(finished(bool)), SLOT(done()));
	connect(this, SIGNAL(dataReadProgress(int, int)), SLOT(updateSpeed(int, int)));
	connect(speedUpdateTimer, SIGNAL(timeout()), SLOT(signalSpeed()));
	connect(this, SIGNAL(installDone()), SLOT(installNext()));
	connect(&installer, SIGNAL(done()), SLOT(cleanupAfterInstall()));
}

void Controller::signalSpeed() {
	int time_delta = last_speed_update.msecsTo(QTime::currentTime());
	if (time_delta < 0) {
		time_delta = MSEC_IN_A_DAY - time_delta;
	}
	if (time_delta != 0) {
		emit downloadSpeed((currentDownloadSize - lastDownloadSize) / time_delta);
		last_speed_update = QTime::currentTime();
		lastDownloadSize = currentDownloadSize;
	}
}

void Controller::updateSpeed(int done, int /* total */) {
	currentDownloadSize = done;
}

void Controller::applyNext() {
	if (m_tweaks.empty()) {
		emit finished(false);
		return;
	}

	if (m_abort) {
		emit aborted();
		return;
	}

	TweakAction currentTweak = m_tweaks.front();
	m_tweaks.erase(m_tweaks.begin(), m_tweaks.begin()+1);

	LOG("Applying " << currentTweak.description())
	tweakRunner.apply(&currentTweak);

	applyNext();
}

void Controller::apply() {

}

void Controller::installNext() {
	// Finished
	if (m_versions.empty()) {
		emit finished(false);
		return;
	}
	// Aborted
	if (m_abort) {
		emit aborted();
		return;
	}
	// Process next version
	currentVersion = m_versions.front();
	m_versions.erase(m_versions.begin(), m_versions.begin()+1);

	LOG("Installing " << currentVersion.name())

	currentVersion.reload();// Last chance to find an application installer. NOTE should this be here ?

	if (currentVersion.hasInstaller() and !m_download_only) // Have an installer and not set to only download.
		install();
	else if (currentVersion.hasUrl()) // No installer, but can download one.
		download(currentVersion);
	else // Have neither installer, not place to download from . skip.
		installNext();
}

void Controller::abort() {
	m_abort = true;
	switch (download_type) {
		case Downloader::FTP:
			ftp_downloader.cancelDownload();
			break;
		case Downloader::HTTP:
			http_downloader.cancelDownload();
			break;
	}
}

void Controller::install(bool error) {
	if (m_download_only) {
		LOG("Set to download only. not installing")
		emit installDone();
		return;
		// FIXME recursion. better way without recursion ? use internal signals ?
	}

	if (error and CONFIG.haltOnDownloadError()) {
		emit finished(true);
		return;
	}

	if (!error and !m_abort) {
		currentVersion.findInstallerFile();
		emit status(QString((std::string("Proceeding to install ") + currentVersion.name() + " " + currentVersion.version()).c_str()));

		installer.install(currentVersion);
		return;
	}
}

void Controller::cleanupAfterInstall() {

	emit status(QString("Installer finished with exit code: ") + QString::number(installer.lastCode()));;
	if ((installer.lastCode() != 0) and CONFIG.haltOnInstallError()) {
		emit finished(true);
		return;
	}

	emit installDone();
}

void Controller::download(Version &version) {

	// Reset speed calculation variables
	last_speed_update = QTime::currentTime();
	lastDownloadSize = 0;
	currentDownloadSize = 0;
	emit startedNewDownload();

	// Start download
	QUrl url(version.url().c_str());
	if(url.scheme().toLower() == "ftp") {
		download_type = Downloader::FTP;
		ftp_downloader.downloadFile(version.url(), version.path());
	} else { //assume http/https
		download_type = Downloader::HTTP;
		http_downloader.downloadFile(version.url(), version.path());
	}

	// Notify what is being downloaded
	emit status(QString("Downloading %1 %2").arg(QString(version.name().c_str())).arg(QString(version.version().c_str())));
}
