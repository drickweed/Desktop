#pragma once

/** QT headers **/
#include <QDateTime>
#include <QList>
#include <QStringList>
#include <QMap>

class CronToken {
	public:
		enum Types {Separator, Number, Operator, Command, Day};

		CronToken() : type(Separator) {}
		CronToken(int number): number(number), type(Number) {}
		CronToken(QChar op): op(op), type(Operator) {}
		CronToken(QString command) : command(command), type(Command) {}
		CronToken(Qt::DayOfWeek day) : number(day), type(Day) {}

		static CronToken identify(QString token);

		static QStringList dayNames();
		static QStringList days;

		static QList<QChar> operators;

		QString toString() const;

		int number;
		QChar op;
		QString command;
		Types type;

		Qt::DayOfWeek day() const { return (Qt::DayOfWeek) number; }

		bool isNumber() const { return (type == Number) or (type == Day); }
		bool isOperator() const { return type == Operator; }
		bool isRange() const { return op == '-'; }
		bool isList() const { return op == ','; }
		bool isRepeater() const { return op == '/'; }
		bool isGlob() const { return op == '*'; }
		bool isCommand() const { return type == Command; }
		bool isSeparator() const { return type == Separator; }
		bool isDay() const { return type == Day; }
};

class NumericRange {
	public:
		NumericRange(int start, int end): start(start), end(end) {}
		int start, end;

		bool contains(int value) const { return (value >= start) and (value <= end); }
		bool overlaps(NumericRange span) const { return span.end >= start and span.start <= end; }
		QString toString() const { return QString("%1-%2").arg(start, end); }
};

class WeekDayRange {
	public:
		WeekDayRange(Qt::DayOfWeek start, Qt::DayOfWeek end): start(start), end(end) {}
		WeekDayRange(NumericRange span) {start = span.start; end = span.end; }

		bool contains(Qt::DayOfWeek day_of_week) const {
			int day = day_of_week;
			if (start < end)
				return (day >= start) and (day <= end);
			return (day >= start) or (day <= end);
		}

	private:
		int start, end;
};

class CronSegment {
	public:
		enum Types {Minute, Hour, Day, Month, DayOfWeek, Command};
		enum State {Number, Range, Repeater, ListSeparator, Separator, Glob};

		CronSegment(): glob(false), repeater(0) {}

		void addSingle(int i) { singles << i; }
		void addSpan(int start, int end) {time_spans << NumericRange(start, end); }
		void setRepeater(int repeater) { this->repeater = repeater; }
		void setCommand(QString command) { this->command = command; }
		void setType(Types type) { this->type = type; }
		bool isCommand() const { return not command.isEmpty(); }

		bool contains(int value) const;
		// bool contains(int value, int tolerance) const;
		int nextMatch(int value) const {
			if (contains(value))
				return value;

			int nearest = singles[0];
			foreach(int single, singles) {
				if ((single > value) and (single < nearest))
					nearest = single;
			}
			foreach(NumericRange span, time_spans) {
				if ((span.start > value) and (span.start < nearest))
					nearest = span.start;
				if ((span.end > value) and (value + 1 < nearest))
					nearest = value + 1;
			}

			return nearest;
		}

		QString toString() const;

		QList<int> singles;
		bool glob;
		int repeater;
		QList<NumericRange> time_spans;
		QString command;
		Types type;
};

class Schedule {
	public:
		enum ScheduleType {Daily, Weekly, Once, Monthly};

		Schedule();

		QMap<CronSegment::Types, CronSegment> cron;

		bool matches(QDateTime datetime);
		bool contained(QDateTime start, QDateTime end) const;

		bool matchesNow() { return matches(QDateTime::currentDateTime()); }

		CronSegment::State processOperator(CronToken op);

		void build(QList<CronToken> tokens);
		void build(QString cron_string);
		QString toString() const;

		QString cronString() const;
		QString displayTimeString() const;
		QString commandString() const;

		void setOnce(QDateTime date);
		void setDaily(QDateTime date);
		void setWeekly(QDateTime date);
		void setMonthly(QDateTime date);
		void setByMonthDay(QDateTime date);

		QDateTime nextMatch(QDateTime current) const;
		QDateTime previousMatch(QDateTime current) const;
		bool matches(QDateTime date) const;
		bool matches(QDate date) const;
		bool matchMonth(int month) const;

	private:
		int tolerance;
		CronSegment::Types m_current_segment;
};

class CronString {
	public:

		QList<CronToken> tokens;

		void tokenize(QString line);

};
