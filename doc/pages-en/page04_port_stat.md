# Port Statistics/Diagnostics {#sec_port_stat}

Used to diagnose gateway operation. `ItemReference` data is ReadOnly.

| `ItemReference`        | Data Type     | Purpose                                                                                         |
|:-----------------------|:--------------|:------------------------------------------------------------------------------------------------|
| `ThreadCycleCount`     | `UInt32`      | Thread execution. Number of cycles since statistics started                                     |
| `ThreadLastCycle`      | `UInt32`      | Thread execution. Duration of the last cycle (microseconds)                                     |
| `ThreadMinCycle`       | `UInt32`      | Thread execution. Minimum cycle duration since statistics started (microseconds)                |
| `ThreadMaxCycle`       | `UInt32`      | Thread execution. Maximum cycle duration since statistics started (microseconds)                |
| `ThreadAvgCycle`       | `UInt32`      | Thread execution. Average cycle duration since statistics started (microseconds)                |
| `GoodCount`            | `UInt32`      | Total number of successful Modbus messages                                                      |
| `BadCount`             | `UInt32`      | Total number of failed Modbus messages                                                          |
| `Status`               | `ModbusStatus`| Status of the last Modbus message (see \ref sec_data_statusModbus)                              |
| `Timestamp`            | `Timestamp`   | (PC) time of the last Modbus message                                                            |
| `LastSuccessTimestamp` | `Timestamp`   | (PC) time of the last successful Modbus message                                                 |
| `LastErrorStatus`      | `ModbusStatus`| Status of the last Modbus error (see \ref sec_data_statusModbus)                               |
| `LastErrorTimestamp`   | `Timestamp`   | (PC) time of the last failed Modbus message                                                     |
| `LastErrorText`        | `String`      | Text representation of the last Modbus error                                                    |
