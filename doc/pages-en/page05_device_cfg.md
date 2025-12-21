# Device/PLC Configuration {#sec_device_cfg}

Used to read configuration parameters of the gateway. `ItemReference` data is ReadOnly.

| `ItemReference`             | Data Type | Purpose                                                                                                                      |
|:----------------------------|:----------|:-----------------------------------------------------------------------------------------------------------------------------|
| `Name`                      | `String`  | Device/PLC name                                                                                                             |
| `EnableDevice`              | `Bool`    | Availability of this device/PLC for the execution chain (1‑on, 0‑off)                                                       |
| `ModbusUnit`                | `UInt8`   | Modbus address of this device/PLC                                                                                           |
| `RepeatCount`               | `UInt32`  | Maximum number of retries for a Modbus request when execution fails                                                          |
| `RestoreTimeout`            | `UInt32`  | Timeout to restore connection after error (exclusion from execution chain) (milliseconds)                                    |
| `MaxReadCoils`              | `UInt16`  | Maximum number of Coils to read in one message                                                                               |
| `MaxWriteMultipleCoils`     | `UInt16`  | Maximum number of Coils to write in one message                                                                              |
| `MaxReadDiscreteInputs`     | `UInt16`  | Maximum number of DiscreteInputs to read in one message                                                                      |
| `MaxReadInputRegisters`     | `UInt16`  | Maximum number of InputRegisters to read in one message                                                                      |
| `MaxReadHoldingRegisters`   | `UInt16`  | Maximum number of HoldingRegisters to read in one message                                                                    |
| `MaxWriteMultipleRegisters` | `UInt16`  | Maximum number of HoldingRegisters to write in one message                                                                   |
| `DefaultPeriod`             | `UInt32`  | Default polling period for each Modbus item, if not specified (milliseconds)                                                 |
| `RequestTimeout`            | `UInt32`  | Request timeout for each Modbus item, after which polling stops (milliseconds)                                               |
| `LogFlags`                  | `String`  | Logging flags                                                                                                               |
| `LogOutput`                 | `String`  | Logging output types                                                                                                        |
| `LogFileFormat`             | `String`  | File message format (see [Logging](#sec_journal_common))                                                                     |
| `LogFileTimeformat`         | `String`  | File time format (see [Logging](#sec_journal_common))                                                                        |
| `LogFilePath`               | `String`  | Path to log file                                                                                                            |
| `LogFileMaxCount`           | `Int32`   | Max number of rotated log files                                                                                              |
| `LogFileMaxSize`            | `Int32`   | Max size of a rotated log file (bytes)                                                                                       |
