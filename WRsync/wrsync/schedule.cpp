#include "schedule.h"

/** QT headers **/
#include <QVariant>

#include "logfile.h"

Schedule::Schedule():
		tolerance(5) {
}

QList<QChar> CronToken::operators = QList<QChar>() << '-' << '/' << ',' << '*';
QStringList CronToken::days = CronToken::dayNames();

QStringList CronToken::dayNames() {
	QStringList days;
	for (int i=1; i<8; i++)
		days.append(QDate::shortDayName(i).toLower());
	return days;
}

CronToken CronToken::identify(QString token) {
	bool conversion_result;

	int number = token.toInt(&conversion_result, 10);
	if (conversion_result == true)
		return CronToken(number);

	if (token.size() == 1)
		return CronToken(token[0]);

	if (days.contains(token.toLower()))
		return CronToken((Qt::DayOfWeek)days.indexOf(token.toLower())+1);

	return CronToken(token);
}

QString CronToken::toString() const {
	if (isNumber())
		return QVariant(number).toString();
	if (isOperator())
		return QVariant(op).toString();
	if (isDay())
		return QDate::shortDayName(number).toLower();
	return command;
}

QString CronSegment::toString() const {
	QString str;

	if (isCommand())
		return command;

	for(int i=0; i<singles.size(); i++) {
		str.append(QVariant(singles[i]).toString());
		if (i < singles.size()-1)
			str.append(',');
	}

	if (not time_spans.isEmpty())
		str.append(',');

	for(int i=0; i<time_spans.size(); i++) {
		str.append(time_spans[i].toString());
		if (i < time_spans.size()-1)
			str.append(',');
	}

	if (glob) {
		if (not str.isEmpty()) str.append(',');
		str.append('*');
	}

	if (repeater != 0)
		str.append(QString("/%1").arg(repeater));

	return str;
}

void CronString::tokenize(QString line) {
	tokens.clear();
	int section = 0;
	QString token;
	while (not line.isEmpty()) {

		QChar next_char = line[0];
		line.remove(0,1);

		// Digit
		if (next_char.isDigit() or next_char.isLetter()) {
			LDEBUG_ << "Found digit";
			token.append(next_char);
			continue;

		// Operator
		} else if (CronToken::operators.contains(next_char)) {
			if (not token.isEmpty()) {
				LDEBUG_ << "Pushing token " << QSTR(token);
				tokens << CronToken::identify(token);
				token.clear();
			}
			LDEBUG_ << "Pushing operator " << next_char.toAscii();
			tokens << CronToken(next_char);
			continue;

		// Space separator
		} else if (next_char.isSpace()) {
			LDEBUG_ << "Found space";

			if (not token.isEmpty()) {
				LDEBUG_ << "Pushing token: " << QSTR (token);
				tokens << CronToken::identify(token);
				token.clear();
			}

			if (not tokens.last().isSeparator()) {
				LDEBUG_ << "Pushing separator";
				tokens << CronToken();
				section++;
			}

			if (section == 5) { // The rest is the command to execute
				LDEBUG_ << "Pushing command";
				tokens << CronToken(line.trimmed());
				LDEBUG_ << "Clearing the rest of the cron line";
				line.clear();
				continue;
			}

			continue;
		}

	}
	LDEBUG_ << "Done parsing cron line";

}

QString Schedule::toString() const {
	return QString("%1 %2").arg(cronString(), commandString());
}

QString Schedule::cronString() const {
	QString cron_string;
	cron_string = QString("%1 %2 %3 %4 %5")
				  .arg(cron[CronSegment::Minute].toString())
				  .arg(cron[CronSegment::Hour].toString())
				  .arg(cron[CronSegment::Day].toString())
				  .arg(cron[CronSegment::Month].toString())
				  .arg(cron[CronSegment::DayOfWeek].toString());
	return cron_string;
}

QString Schedule::commandString() const {
	return cron[CronSegment::Command].toString();
}

QString Schedule::displayTimeString() const {
	return QString();
}

CronSegment::State Schedule::processOperator(CronToken token) {
	LDEBUG_ << "Operator " << token.op.toAscii();
	if (token.isList())
		return CronSegment::ListSeparator;
	if (token.isRepeater())
		return CronSegment::Repeater;
	if (token.isRange())
		return CronSegment::Range;

	cron[m_current_segment].glob = true;
	return CronSegment::Glob;
}

void Schedule::build(QString cron_string) {
	CronString cron;
	cron.tokenize(cron_string);
	build(cron.tokens);
}

void Schedule::setOnce(QDateTime datetime) {
	build(datetime.toString("m h d M *"));
}

void Schedule::setDaily(QDateTime datetime) {
	build(datetime.toString("m h * * *"));
}

void Schedule::setWeekly(QDateTime datetime) {
	build(datetime.toString("m h * * ddd"));
}

void Schedule::setMonthly(QDateTime datetime) {
	build(datetime.toString("m h d * *"));
}

void Schedule::build(QList<CronToken> tokens) {
	cron.clear();

	LSEPARATOR_;
	m_current_segment = CronSegment::Minute;
	cron[m_current_segment].setType((CronSegment::Types)m_current_segment);
	CronSegment::State last_token;
	last_token = CronSegment::Separator;
	int last_number = -1;
	int low_range = -1;
	bool used_as_repeater = false;

	foreach(CronToken token, tokens) {



		switch (last_token) {
			case CronSegment::Separator:
				if (token.isSeparator()) {
						LDEBUG_ << "Duplicate separator";
						last_token = CronSegment::Separator;
						continue;
				}

				if (token.isNumber()) {
					LDEBUG_ << "First number " << token.number;
					last_number = token.number;
					last_token = CronSegment::Number;
				}

				if (token.isGlob()) {
					cron[m_current_segment].glob = true;
					last_token = CronSegment::Glob;
				}

				if (token.isCommand()) {
					cron[m_current_segment].command = token.command;
				}

				break;

			case CronSegment::Number:
				if (token.isList() or token.isSeparator()) {
					if (used_as_repeater) {
						used_as_repeater = false;
					} else if (low_range == -1) {
						LDEBUG_ << "Single";
						cron[m_current_segment].addSingle(last_number);
					} else {
						LDEBUG_ << "Range";
						cron[m_current_segment].addSpan(low_range, last_number);
						low_range = -1;
					}
					if (token.isList())
						last_token = CronSegment::ListSeparator;
					else
						last_token = CronSegment::Separator;
				}
				if (token.isRange()) {
					LDEBUG_ << "Entering range";
					low_range = last_number;
					last_token = CronSegment::Range;
				}

				break;

			case CronSegment::Range:
				if (token.isNumber()) {
					last_number = token.number;
					last_token = CronSegment::Number;
				}
				break;

			case CronSegment::ListSeparator:
				if (token.isNumber()) {
					last_number = token.number;
					last_token = CronSegment::Number;
				}
				break;

			case CronSegment::Repeater:
				if (token.isNumber()) {
					LDEBUG_ << "Repeater";
					cron[m_current_segment].setRepeater(token.number);
					last_token = CronSegment::Number;
					used_as_repeater = true;
				}
				break;

			case CronSegment::Glob:
				if (token.isRepeater()) {
					LDEBUG_ << "Glob";
					last_token = CronSegment::Repeater;
				}
				break;

		}

		if (token.isSeparator()) {
			LDEBUG_ << "Separator";
			last_token = CronSegment::Separator;
			LDEBUG_ << "Segment: " << QSTR(cron[m_current_segment].toString());
			m_current_segment = (CronSegment::Types) ((int) m_current_segment + 1);
			cron[m_current_segment].setType((CronSegment::Types)m_current_segment);
		}

	}

}


bool CronSegment::contains(int value) const {
	if (glob and repeater == 0)
		return true;

	if (glob and repeater != 0) {
		if (((value-1) % repeater) == 0)
			return true;
	}

	switch (type) {
		case CronSegment::Minute:
		case CronSegment::Hour:
		case CronSegment::Day:
		case CronSegment::Month:
			if (singles.contains(value))
				return true;
			foreach(NumericRange span, time_spans)
				if (span.contains(value))
					return true;
			break;
		case CronSegment::DayOfWeek:
			if (singles.contains(value))
				return true;
			foreach(NumericRange span, time_spans)
				if (WeekDayRange(span).contains((Qt::DayOfWeek)value))
					return true;
		default:
			break;
	}

	return false;
}

bool Schedule::contained(QDateTime start, QDateTime end) const {
	return nextMatch(start) < end;
}

bool Schedule::matches(QDateTime date) const {
	return
			cron[CronSegment::Minute].contains(date.time().minute()) and
			cron[CronSegment::Hour].contains(date.time().hour()) and
			cron[CronSegment::Day].contains(date.date().day()) and
			cron[CronSegment::Month].contains(date.date().month()) and
			cron[CronSegment::DayOfWeek].contains((date.date().dayOfWeek()+1) % 7);
}

bool Schedule::matches(QDate date) const {
	return
			cron[CronSegment::Day].contains(date.day()) and
			cron[CronSegment::Month].contains(date.month()) and
			cron[CronSegment::DayOfWeek].contains((date.dayOfWeek()+1) % 7);
}

bool Schedule::matchMonth(int month) const {
	return cron[CronSegment::Month].contains(month);
}

QDateTime Schedule::nextMatch(QDateTime current) const {
	QDateTime match;



	return match;
}

QDateTime Schedule::previousMatch(QDateTime current) const {
	QDateTime match;

	return match;
}
