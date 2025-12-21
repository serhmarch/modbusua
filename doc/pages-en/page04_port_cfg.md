# Port Configuration {#sec_port_cfg}

Used to read configuration parameters of the gateway. `ItemReference` data is ReadOnly.

| `ItemReference`    | Data Type | Purpose                                                                        |
|:-------------------|:----------|:-------------------------------------------------------------------------------|
| `Name`             | `String`  | Port name                                                                      |
| `Enable`           | `Bool`    | Availability of the port for execution chain (1‑on, 0‑off)                    |
| `Type`             | `String`  | Modbus protocol type: `TCP`, `RTU`, `ASC`                                      |
| `Host`             | `String`  | IP address or DNS name of remote device (for TCP)                              |
| `Port`             | `UInt16`  | TCP port number (standard Modbus TCP port 502)                                 |
| `Timeout`          | `UInt32`  | Connection timeout (milliseconds) (for TCP)                                    |
| `SerialPortName`   | `String`  | Serial port name (for `RTU`, `ASC`), e.g. `"COM1"`                            |
| `BaudRate`         | `Int32`   | Serial port speed, e.g. `9600` (default), `19200`, `115200`...                 |
| `DataBits`         | `Int8`    | Data bits, e.g. `8` (default), `7`, `6`, `5`                                   |
| `Parity`           | `String`  | Parity: `No` (default), `Even`, `Odd`, `Space`, `Mark`                         |
| `StopBits`         | `String`  | Stop bits: `1` (default), `1.5`, `2`                                           |
| `FlowControl`      | `String`  | Flow control: `No` (default), `Hard`, `Soft`                                   |
| `TimeoutFirstByte` | `UInt32`  | Timeout waiting for first byte (ms) (for `RTU`, `ASC`)                         |
| `TimeoutInterByte` | `UInt32`  | Timeout waiting between subsequent bytes (ms) (for `RTU`, `ASC`)               |
| `LogFlags`         | `String`  | Logging flags                                                                  |
| `LogOutput`        | `String`  | Logging output types                                                           |
| `LogFileFormat`    | `String`  | File message format (see [Logging](#sec_journal_common))                       |
| `LogFileTimeformat`| `String`  | File time format (see [Logging](#sec_journal_common))                          |
| `LogFilePath`      | `String`  | Path to log file                                                               |
| `LogFileMaxCount`  | `Int32`   | Max number of rotated log files                                                |
| `LogFileMaxSize`   | `Int32`   | Max size of a rotated log file (bytes)                                         |
