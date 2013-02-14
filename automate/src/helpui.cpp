#include "helpui.h"
#include <QDebug>
#include <QFile>

HelpUi::HelpUi(QWidget* parent) : QDialog(parent) {
	setupUi(this);
	QFile page(":help/appdb.html");
	page.open(QIODevice::ReadOnly | QIODevice::Text);
	QString html = "";
	QTextStream in(&page);
	while (!in.atEnd())
		html += in.readLine();
	webView->setHtml(html);
}
