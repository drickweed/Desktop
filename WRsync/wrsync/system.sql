# Backup reports table
CREATE TABLE IF NOT EXISTS system_log (id INTEGER PRIMARY KEY, entry_time DATETIME, message TEXT NOT NULL, message_id INTEGER, detail TEXT);
