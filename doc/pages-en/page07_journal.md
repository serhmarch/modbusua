# Gateway Logging {#sec_journal_common}

During gateway operation, diagnostic messages may be written to the log reflecting the program’s progress.

Logging is configured in the `[SYSTEM]` section with the following parameters:

| Name                     | Description                                                                                         |
|:-------------------------|:----------------------------------------------------------------------------------------------------|
| `Log.flags`              | List of flags defining logging level                                                                |
| `Log.output`             | List of logging output types (see below)                                                            |
| `Log.default.format`     | Default single‑message format (see below)                                                           |
| `Log.default.timeformat` | Default time format (see below)                                                                     |
| `Log.system.format`      | System log message format (see below)                                                               |
| `Log.system.timeformat`  | System log time format (see below)                                                                  |
| `Log.console.format`     | Console message format (see below)                                                                  |
| `Log.console.timeformat` | Console time format (see below)                                                                     |
| `Log.file.format`        | File message format (see below)                                                                     |
| `Log.file.timeformat`    | File time format (see below)                                                                        |
| `Log.file.path`          | Path to the file where messages are written                                                         |
| `Log.file.maxcount`      | Maximum number of rotated log files                                                                 |
| `Log.file.maxsize`       | Maximum size of a rotated log file (bytes). Suffixes supported: `KB`, `MB`, `GB` (e.g., `5 MB`)     |

Messages are categorized to enable selective output useful for debugging and analysis.
Categories are specified via `Log.flags` in the `[SYSTEM]` section as a `|`‑separated list:

| Category           | Purpose                                                        |
|:-------------------|:---------------------------------------------------------------|
| `Error`            | Critical errors that may cause stoppage or incorrect operation |
| `Warning`          | Non‑critical issues that do not stop the program               |
| `Info`             | General diagnostic messages                                    |
| `Trace`            | Diagnostic messages with more detail                           |
| `TraceDetails`     | Highly detailed diagnostic messages                            |
| `CtorDtor`         | Creation/destruction of main program components                |
| `EntryExit`        | Entry/exit of important program functions                      |
| `ThreadStartStop`  | Start/stop events for program threads                          |
| `Connection`       | Establish/teardown of network connections with remote objects  |
| `Item`             | Creation/execution of individual `ItemReference` variables     |
| `Message`          | Creation/execution of message objects (packets)                |
| `Protocol`         | Modbus protocol‑specific messages                              |
| `Receive`          | Incoming Modbus network frames                                 |
| `Send`             | Outgoing Modbus network frames                                 |

Output targets can be the system log, console, file, or any combination. By default, services/daemons use the system log;
otherwise console is used.

Output is set via `Log.output` in `[SYSTEM]`, which can have one or more space‑separated values:
* `system` — output to system log
* `console` — output to console
* `file` — output to file

Example:
```ini
[SYSTEM]
Log.output="file console"
```

`Log.default.*` parameters define defaults for all outputs, while `Log.console.*` and `Log.file.*` override these for console and file respectively.

`Log.file.maxcount` and `Log.file.maxsize` enable file logging with rotation.

The message format can include plain text and special placeholders replaced when the message is formed:

* `%%time` — message timestamp; its format is set by `%%timeformat`
* `%%text` — message text
* `%%cat`  — category name
* `%%src`  — source name (device/port or `modbusua` for global events)

Time format placeholders (`%%timeformat`):

* `%%Y` — year (4 digits)
* `%%M` — month (2 digits `01`‑`12`)
* `%%D` — day (2 digits `01`‑`31`)
* `%%h` — hour (2 digits `00`‑`23`)
* `%%m` — minute (2 digits `00`‑`59`)
* `%%s` — second (2 digits `00`‑`59`)
* `%%f` — millisecond (3 digits `000`‑`999`)

Example logging configuration:

```ini
[SYSTEM]

#...

Log.flags="Error|Warning|Info|TraceDetails"
Log.output="system file console"
Log.default.format="[%time] %cat %src. %text"
Log.default.timeformat="%Y-%M-%D %h:%m:%s.%f"
Log.concole.format="[%time] %src. %text"
Log.console.timeformat="%h:%m:%s.%f"
Log.file.format="%time;%cat;%src;%text"
Log.file.timeformat="%h:%m:%s.%f"
Log.file.path="log.csv"
Log.file.maxcount=10
Log.file.maxsize=1MB

#...

```

## Per‑port or Per‑device Logging {#sec_journal_common_sub}

You can specify logging independently for a port or device/PLC.
Available settings (subset of those above):
* `Log.flags`
* `Log.output`
* `Log.file.format`
* `Log.file.timeformat`
* `Log.file.path`
* `Log.file.maxcount`
* `Log.file.maxsize`

Thus each port/device can have its own flags, output targets (`system`, `console`, `file`), and file parameters.
Console output settings are specified in `[SYSTEM]`. File output settings can be set individually for each port/device in `[PORT]` or `[DEVICE]`.
If multiple ports/devices use the same file name, output will be written to that single file.
If no specific settings are provided, output follows `[SYSTEM]` logging.

## Windows System Log {#sec_journal_windows}

If `Log.output` includes `system`, messages are written to the Windows Event Log:
`Control Panel\System and Security\Administrative Tools\Event Viewer\Windows Logs\Application`.

## Linux System Log {#sec_journal_linux}

On Linux, `syslog/rsyslog` is used. For distributions using `systemd`, use `journalctl` to view logs.
Example:
```console
$ journalctl -e -l -f -u modbusua --since "2025-08-06 13:00:00"
[...]
Aug 05 11:58:42 DESKTOP-TNLLA10 systemd[1]: Starting OPC UA - Modbus Gateway...
Aug 05 11:58:42 DESKTOP-TNLLA10 modbusua[121426]: [2025-08-05 11:58:42.656] modbusua. Loading config file: /etc/modbusua/modbusua.conf
Aug 05 11:58:42 DESKTOP-TNLLA10 systemd[1]: Started OPC UA - Modbus Gateway.
[...]
```

Key `journalctl` parameters:
 Option         | Description
----------------|----------------------------------------------------
`-a`            | Show all fields
`-e`            | Jump to end
`-l`            | Show full printable fields
`-n <num>`      | Show last `<num>` entries
`-r`            | Reverse order
`-f`            | Follow updates
`-u <unit>`     | Show entries only for given unit/service
`--since <dt>`  | Show entries since given date/time
