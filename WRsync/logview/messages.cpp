#include "messages.h"

QMap<int, QString> Messages::m_state_messages = QMap<int, QString>();
QMap<int, QString> Messages::m_rsync_messages = QMap<int, QString>();

Messages::Messages() {
}

void Messages::init_messages() {
	m_rsync_messages[0]="Success";
	m_rsync_messages[1]="Syntax or usage error";
	m_rsync_messages[2]="Protocol incompatibility";
	m_rsync_messages[3]="Errors selecting input/output files, dirs";
	m_rsync_messages[4]="Requested  action not supported: an attempt was made to manipulate 64-bit files on a platform that cannot support them, or an option was specified that is supported by the client and not by the server.";
	m_rsync_messages[5]="Error starting client-server protocol";
	m_rsync_messages[6]="Daemon unable to append to log-file";
	m_rsync_messages[10]="Error in socket I/O";
	m_rsync_messages[11]="Error in file I/O";
	m_rsync_messages[12]="Error in rsync protocol data stream";
	m_rsync_messages[13]="Errors with program diagnostics";
	m_rsync_messages[14]="Error in IPC code";
	m_rsync_messages[20]="Received SIGUSR1 or SIGINT";
	m_rsync_messages[21]="Some error returned by waitpid()";
	m_rsync_messages[22]="Error allocating core memory buffers";
	m_rsync_messages[23]="Partial transfer due to error";
	m_rsync_messages[24]="Partial transfer due to vanished source files";
	m_rsync_messages[25]="The --max-delete limit stopped deletions";
	m_rsync_messages[30]="Timeout in data send/receive";
	m_rsync_messages[35]="Timeout waiting for daemon connection";

	m_state_messages[Result::None]="No job state. This should never happen. Something went horribly wrong";
	m_state_messages[Result::OK]="OK";
	m_state_messages[Result::Error]="Error copying files. See detailed error log";
	m_state_messages[Result::Crashed]="Crash";
	m_state_messages[Result::FailedToStart]="Copy process failed to start";
	m_state_messages[Result::MissingDestination]="Backup destination missing";
	m_state_messages[Result::MissingSource]="Backup sources missing";
	m_state_messages[Result::UnknownError]="Unknown error occured";

}

QString Messages::rsync(int exit_code) {
	return m_rsync_messages.value(exit_code, "Undocumented error. Please inform the developers.");
}

QString Messages::state(Result::State state) {
	return m_state_messages.value((int)state, "Undocumented error. Please inform the developers.");
}
