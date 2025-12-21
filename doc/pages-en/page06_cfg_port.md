# Port Object Configuration {#sec_cfg_port}

Before creating a device object, a port object is created in the gateway configuration tree. It sets network parameters.

Port configuration is provided in the `[PORT]` section of `modbusua.conf`.

Configuration options for the port object:

| Name                 | Description                                                                           |
|:---------------------|:---------------------------------------------------------------------------------------|
| `Name`               | Port name                                                                              |
| `Enable`             | Availability of the port in the execution chain (1‑on, 0‑off)                          |
| `RepeatCount`        | Max number of retries for Modbus requests on failure                                   |
| `Type`               | Modbus protocol type: `TCP`, `RTU`, `ASC`                                              |
| `Host`               | IP address or DNS name of the remote device (for `TCP`)                                |
| `Port`               | TCP port number (standard Modbus TCP port 502)                                         |
| `Timeout`            | Connection timeout (ms) (for `TCP`)                                                    |
| `SerialPortName`     | Serial port name (for `RTU`, `ASC`), e.g. `"COM1"`                                    |
| `BaudRate`           | Serial port speed, e.g. `9600` (default), `19200`, `115200`...                         |
| `DataBits`           | Data bits, e.g. `8` (default), `7`, `6`, `5`                                           |
| `Parity`             | Parity: `No` (default), `Even`, `Odd`, `Space`, `Mark`                                 |
| `StopBits`           | Stop bits: `1` (default), `1.5`, `2`                                                   |
| `FlowControl`        | Flow control: `No` (default), `Hard`, `Soft`                                           |
| `TimeoutFirstByte`   | Timeout waiting for first byte (ms) (for `RTU`, `ASC`)                                 |
| `TimeoutInterByte`   | Timeout waiting between subsequent bytes (ms) (for `RTU`, `ASC`)                       |
| `Log.flags`          | Logging flags                                                                          |
| `Log.output`         | Logging output types (see [Logging](#sec_journal_common))                              |
| `Log.file.format`    | File message format (see [Logging](#sec_journal_common))                               |
| `Log.file.timeformat`| Time format for file messages (see [Logging](#sec_journal_common))                     |
| `Log.file.path`      | Path to log file                                                                       |
| `Log.file.maxcount`  | Max number of rotated log files                                                        |
| `Log.file.maxsize`   | Max size of a rotated log file (bytes). Suffixes: `KB`, `MB`, `GB` (e.g., `5 MB`)      |
