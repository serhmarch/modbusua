# Device/PLC Statistics/Diagnostics {#sec_device_stat}

Used to diagnose gateway operation. `ItemReference` data is ReadOnly.

| `ItemReference`                | Data Type      | Purpose                                                                                                                |
|:-------------------------------|:---------------|:-----------------------------------------------------------------------------------------------------------------------|
| `State`                        | `UInt32`       | Bit array of the current gateway state                                                                                 |
| `StateEnableDevice`            | `Bool`         | Communication allowed (matches bit 0 of `Stat.State`)                                                                  |
| `StateConnectionAlive`         | `Bool`         | PLC connection alive (matches bit 2 of `Stat.State`)                                                                   |
| `StateIncluded`                | `Bool`         | PLC included in the execution chain; may be excluded while recovering after an error if `RestoreTimeout` not elapsed (matches bit 10 of `Stat.State`) |
| `CommonGoodCount`              | `UInt32`       | Total number of successful Modbus messages                                                                             |
| `CommonBadCount`               | `UInt32`       | Total number of failed Modbus messages                                                                                 |
| `CommonStatus`                 | `ModbusStatus` | Status of the last Modbus message (see \ref sec_data_statusModbus)                                                     |
| `CommonTimestamp`              | `Timestamp`    | (PC) time of the last Modbus message                                                                                   |
| `CommonLastSuccessTimestamp`   | `Timestamp`    | (PC) time of the last successful Modbus message                                                                        |
| `CommonLastErrorStatus`        | `ModbusStatus` | Status of the last Modbus error (see \ref sec_data_statusModbus)                                                       |
| `CommonLastErrorTimestamp`     | `Timestamp`    | (PC) time of the last failed Modbus message                                                                            |
| `CommonLastErrorText`          | `String`       | Text representation of the last Modbus error                                                                           |
| `PokeGoodCount`                | `UInt32`       | Number of successful simple Modbus Write messages                                                                       |
| `PokeBadCount`                 | `UInt32`       | Number of failed simple Modbus Write messages                                                                           |
| `PokeStatus`                   | `ModbusStatus` | Status of the last simple Modbus Write message (see \ref sec_data_statusModbus)                                       |
| `PokeTimestamp`                | `Timestamp`    | (PC) time of the last simple Modbus Write message                                                                       |
| `PokeLastSuccessTimestamp`     | `Timestamp`    | (PC) time of the last successful simple Modbus Write message                                                            |
| `PokeLastErrorStatus`          | `ModbusStatus` | Status of the last simple Modbus Write error (see \ref sec_data_statusModbus)                                          |
| `PokeLastErrorTimestamp`       | `Timestamp`    | (PC) time of the last failed simple Modbus Write message                                                                |
| `PokeLastErrorText`            | `String`       | Text representation of the last simple Modbus Write error                                                               |
| `PollGoodCount`                | `UInt32`       | Number of successful simple Modbus Read messages                                                                        |
| `PollBadCount`                 | `UInt32`       | Number of failed simple Modbus Read messages                                                                            |
| `PollStatus`                   | `ModbusStatus` | Status of the last simple Modbus Read message (see \ref sec_data_statusModbus)                                         |
| `PollTimestamp`                | `Timestamp`    | (PC) time of the last simple Modbus Read message                                                                        |
| `PollLastSuccessTimestamp`     | `Timestamp`    | (PC) time of the last successful simple Modbus Read message                                                             |
| `PollLastErrorStatus`          | `ModbusStatus` | Status of the last simple Modbus Read error (see \ref sec_data_statusModbus)                                           |
| `PollLastErrorTimestamp`       | `Timestamp`    | (PC) time of the last failed simple Modbus Read message                                                                 |
| `PollLastErrorText`            | `String`       | Text representation of the last simple Modbus Read error                                                                |
