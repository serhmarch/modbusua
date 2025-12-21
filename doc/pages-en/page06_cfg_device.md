# Device/PLC Object Configuration {#sec_cfg_device}

The device/PLC object is responsible for interacting with the corresponding device/PLC.
Here you set all parameters required for Modbus protocol configuration and other device/PLC‑specific settings.

The device/PLC configuration is provided in the `[DEVICE]` section of `modbusua.conf`.

Main configuration options for the device/PLC object:

| Name                        | Description                              |
|:--------------------------- |:-----------------------------------------|
| `PortName`                  | Name of the port this device/PLC belongs to
| `Name`                      | Device/PLC name
| `EnableDevice`              | Availability of this device/PLC in the execution chain (`1`‑on, `0`‑off)
| `ModbusUnit`                | Modbus address of the device/PLC
| `RestoreTimeout`            | Timeout to restore connection after error (exclusion from execution chain) (ms)
| `MaxReadCoils`              | Max number of Coils to read in one message
| `MaxWriteMultipleCoils`     | Max number of Coils to write in one message
| `MaxReadDiscreteInputs`     | Max number of DiscreteInputs to read in one message
| `MaxReadInputRegisters`     | Max number of InputRegisters to read in one message
| `MaxReadHoldingRegisters`   | Max number of HoldingRegisters to read in one message
| `MaxWriteMultipleRegisters` | Max number of HoldingRegisters to write in one message
| `DefaultPeriod`             | Default polling period for each Modbus Item
| `RequestTimeout`            | Request timeout for each Modbus Item, after which polling stops
| `Log.flags`                 | Logging flags
| `Log.output`                | Logging output types (see [Logging](#sec_journal_common))
| `Log.file.format`           | File message format (see [Logging](#sec_journal_common))
| `Log.file.timeformat`       | Time format for file messages (see [Logging](#sec_journal_common))
| `Log.file.path`             | Path to log file
| `Log.file.maxcount`         | Max number of rotated log files
| `Log.file.maxsize`          | Max size of a rotated log file (bytes). Suffixes supported: `KB`, `MB`, `GB` (e.g., `5 MB`)

Additional configuration options:

| Name                         | Description                              |
|:-----------------------------|:-----------------------------------------|
| `i:<item>=<itemreference>`   | Defines a single data item named `<item>` with reference `<itemreference>`
| `itemfile`                   | Name of a file that lists data items separately

Example of single data items in the `[DEVICE]` section:

```ini
[DEVICE]
...
i:TestStatusBits ="VS.0.0,AS.0.0,TS.0.0,N.7,TS.0.33"
i:Test_Int16     ="400001 S"
i:Test_UInt16    ="400001 R"
i:Test_Int32     ="400001 I"
i:Test_Float     ="400001 F"
i:Test_Double    ="400001 LF"
i:Test_SwInt32   ="400001 IS"
i:Test_SwDouble  ="400001 LFS"
i:Test_ByteArray ="400001-400010 B"
...
```

The `itemfile` stores data items in the `<item>;<itemreference>` format. Example:

```csv
...
"Analog001.Tune_Tmr_CtrlParamDown";"416605 L"
"Analog001.Tune_Tmr_CtrlParamDrop";"416603 L"
...
```

The `<itemreference>` format is defined in [Modbus Registers](#sec_device_mb) and subtopics.
