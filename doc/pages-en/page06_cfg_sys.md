# modbusua System Configuration {#sec_cfg_sys}

System configuration is provided in the `[SYSTEM]` section of `modbusua.conf`.

Options:

| Name                     | Description                                                                      |
|:-------------------------|:----------------------------------------------------------------------------------|
| `Name`                   | System project name                                                               |
| `include`                | Name of another configuration file to include and extend the current one         |
| `Log.flags`              | Logging flags                                                                     |
| `Log.output`             | Logging output types (see below)                                                  |
| `Log.default.format`     | Default message format (see below)                                                |
| `Log.default.timeformat` | Default time format (see below)                                                   |
| `Log.console.format`     | Console message format (see below)                                                |
| `Log.console.timeformat` | Console time format (see below)                                                   |
| `Log.file.format`        | File message format (see below)                                                   |
| `Log.file.timeformat`    | File time format (see below)                                                      |
| `Log.file.path`          | Path to log file                                                                  |
| `Log.file.maxcount`      | Max number of rotated log files                                                   |
| `Log.file.maxsize`       | Max size of a rotated log file (bytes). Suffixes: `KB`, `MB`, `GB` (e.g., `5 MB`) |

Description of `Log.flags` categories is in [Logging](#sec_journal_common).

## OPC UA Protocol Configuration {#sec_cfg_sys_ua}

OPC UA configuration is provided in the `[OPCUA]` section of `modbusua.conf`.

| Name         | Description                                                        |
|:-------------|:-------------------------------------------------------------------|
| `Port`       | TCP port on which the server listens (default `4840`)              |
| `UncertainAs`| Treat `Uncertain` as: `Good`, `Bad`, or `Uncertain` (default)      |
