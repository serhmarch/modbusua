\mainpage modbusua Server

# modbusua Gateway

## Overview

`modbusua Gateway` is a cross‑platform (Windows, Linux) I/O server that converts data
between Modbus and OPC UA (Open Platform Communications Unified Architecture).
The program is implemented in C++ as open‑source and runs as a console application/service
(a daemon in Unix terminology).

`modbusua Gateway` is intended to collect information from existing industrial controllers
or other Modbus devices (such as industrial sensors, meters, etc.) via Modbus, then process
and forward it using the widely adopted OPC UA protocol.

The project is based on two open‑source libraries:
* Modbus (client side): https://github.com/serhmarch/ModbusLib
* OPC UA (server side): https://github.com/open62541/open62541

![Address Space Example](modbusua_address_space.png)

## Key Features

## Implementation Highlights

- Implemented in modern C++ (C++11 and above)
- Cross‑platform (Windows, Linux)
- Configuration via a plain CONF/INI text file
- Logging to file with rotation, and to OS system logs (Windows/Linux)
- Runs as a console app or background service/daemon
- Multithreading support for higher performance
- Modular architecture for easy extensibility
- Built‑in OPC UA server with configurable address space
- Modbus TCP/RTU/ASCII support for interaction with various devices
- Connection settings, timeouts, and retries are configurable via the configuration file
- Hot configuration reload without stopping the server
- Support for multiple data types (bits, integers, floating‑point numbers)
- Scales to large numbers of devices and tags
- Built‑in diagnostics and monitoring of server and connection state
- Flexible error handling and automatic reconnection
- Integration with existing SCADA systems and other upper‑level software

## System Architecture

```
+--------------------+      Modbus     +------------------+    OPC UA    +-----------------+
|     PLC/Device     | <-------------> | modbusua Gateway | <----------> |   SCADA Client  |
|                    |                 |                  |              |                 |
| - Modbus registers |                 | - Modbus client  |              | - Data ingest   |
|                    |                 | - OPC UA server  |              | - Archiving     |
|                    |                 |                  |              | - Visualization |
|                    |                 |                  |              |                 |
+--------------------+                 +------------------+              +-----------------+
```
