# 0.4.1

* Fixed a bug that prevented log files from being created
* Added the `--logdir` parameter to `modbusua.exe`, which specifies the directory for log files
* The `modbusuaservice.bat` script has been renamed to `modbusuas.bat`.
  Added script parameters that allow you to start/stop a service with a specified name,
  as well as set command-line parameters for the service (see `modbusuas.bat --help`)
* Added statistics for the port object: `Stat.State` and `Stat.StateEnablePort` (see documentation)
* Added the `SinceTimestamp` statistic (the time when statistics began) for Port/Device
* Added the `changelog.md` file to the installer's `doc` folder                                                   
* Updated documentation