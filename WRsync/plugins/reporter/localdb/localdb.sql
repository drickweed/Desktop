# Backup reports table
CREATE TABLE IF NOT EXISTS reports (id INTEGER PRIMARY KEY, started_at DATETIME, state INTEGER, source TEXT NOT NULL, exitcode INTEGER, destination_id TEXT, destination_path TEXT);
