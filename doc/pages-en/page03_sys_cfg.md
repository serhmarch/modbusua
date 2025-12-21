# modbusua System Configuration {#sec_sys_cfg}

Used to read configuration parameters of the gateway. `ItemReference` data is ReadOnly.

| `ItemReference`        | Data Type | Purpose                                                                    |
|:-----------------------|:----------|:---------------------------------------------------------------------------|
| `LogFlags`             | `String`  | List of flags defining logging level                                       |
| `LogOutput`            | `String`  | List of logging output types                                               |
| `LogDefaultFormat`     | `String`  | Default single-message format (see [Logging](#sec_journal_common))         |
| `LogDefaultTimeformat` | `String`  | Default time format for a message (see [Logging](#sec_journal_common))     |
| `LogSystemFormat`      | `String`  | System log message format (see [Logging](#sec_journal_common))             |
| `LogSystemTimeformat`  | `String`  | System log time format (see [Logging](#sec_journal_common))                |
| `LogConsoleFormat`     | `String`  | Console message format (see [Logging](#sec_journal_common))                |
| `LogConsoleTimeformat` | `String`  | Console message time format (see [Logging](#sec_journal_common))           |
| `LogFileFormat`        | `String`  | File message format (see [Logging](#sec_journal_common))                   |
| `LogFileTimeformat`    | `String`  | File message time format (see [Logging](#sec_journal_common))              |
| `LogFilePath`          | `String`  | Path to the file where messages are written                                |
| `LogFileMaxCount`      | `Int32`   | Max number of rotated log files                                            |
| `LogFileMaxSize`       | `Int32`   | Max size of a rotated log file (bytes)                                     |
