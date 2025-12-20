# modbusua Gateway

## Overview

`modbusua Gateway` is a cross‑platform (Windows, Linux) I/O server that bridges data
between the Modbus protocol and OPC UA (Open Platform Communications Unified Architecture).
It is implemented in C++ as an open‑source server and can run as a console application
or a background service/daemon.

The gateway collects data from existing industrial PLCs or other Modbus devices
(industrial sensors, meters, etc.) via Modbus, processes it, and exposes it through the
widely adopted OPC UA protocol.

The project is built on two open‑source libraries:
- Modbus (client side): https://github.com/serhmarch/ModbusLib
- OPC UA (server side): https://github.com/open62541/open62541

## Key Features

- Modern C++ implementation (C++17 and above)
- Cross‑platform support (Windows, Linux)
- Configuration via a plain CONF/INI file
- Logging to file with rotation and to the OS system log (Windows/Linux)
- Runs as a console app or Windows service/Linux daemon
- Multithreaded architecture for performance
- Modular design for easy extensibility
- Built‑in OPC UA server with configurable address space
- Modbus TCP/RTU/ASCII support for a wide range of devices
- Connection parameters, timeouts, retries configurable via the config file
- Hot configuration reload without stopping the server
- Support for common data types (bits, integers, floating‑point)
- Scalable to many devices and tags
- Built‑in diagnostics and monitoring of server and connections
- Robust error handling and reconnection logic
- Easy integration with existing SCADA systems and other upper‑level software

## System Architecture

```
+-------------------+       Modbus     +------------------+    OPC UA    +-----------------+
|    PLC/Device     | <--------------> | modbusua Gateway | <--------->  |   SCADA Client  |
|                   |                  |                  |              |                 |
| * Modbus registers|                  | * Modbus client  |              | * Data ingest   |
|                   |                  | * OPC UA server  |              | * Archiving     |
|                   |                  |                  |              | * Visualization |
|                   |                  |                  |              |                 |
+-------------------+                  +------------------+              +-----------------+
```

## Build and Run

### Build From Source

#### Requirements
- **CMake** 3.20 or newer
- **C++17** compliant compiler
- **Git** to clone the repository

#### Windows Build

```cmd
git clone --recursive https://github.com/serhmarch/modbusua.git
cd modbusua
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### Linux Build

```bash
git clone --recursive https://github.com/serhmarch/modbusua.git
cd modbusua
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Windows Installation

#### As a Windows Service

```cmd
:: Install as a service (requires Administrator)
modbusuaservice.bat -i -n modbusuaService

:: Uninstall the service
modbusuaservice.bat -u -n modbusuaService
```

#### MSI Installer
Build an MSI installer using the WiX Toolset:
```cmd
cd install/windows
cmake --build . --target modbusua-msi
```

### Linux Installation

#### systemd Service
```bash
# Copy files
sudo cp modbusua /usr/bin/
sudo cp modbusua.conf /etc/modbusua/
sudo cp modbusua.service /etc/systemd/system/

# Enable the service
sudo systemctl enable modbusua
sudo systemctl start modbusua

# Check status
sudo systemctl status modbusua
```

#### DEB/RPM Packages
```bash
# Build a DEB package
cmake --build . --target modbusua-deb

# Build an RPM package
cmake --build . --target modbusua-rpm
```

## Configuration

Configuration is done via the `modbusua.conf` file. Main configuration objects:

### Port Object
```ini
[Port]
Type=TCP
Host=192.168.1.100
Port=502
Timeout=5000
```

### Device Object
```ini
[Device]
Port=Port1
Unit=1
```

### Logging Settings
```ini
[Log]
Log.flags=Error|Warning|Info
Log.file.path=modbusua.log
Log.file.maxsize=10MB
```

## Command‑Line Options

| Option             | Short | Parameter    | Description                                    |
|:-------------------|:------|:-------------|:-----------------------------------------------|
| --version          | -v    | -            | Show program version                           |
| --help             | -?    | -            | Show help                                       |
| --file             | -f    | <filename>   | Configuration file (default: modbusua.conf)    |

## Usage Examples

### Basic Configuration
```ini
# modbusua.conf
[System]
Name=modbusua Gateway
Description=OPC UA to Modbus Gateway

[Port.MainPort]
Name=MainPort
Type=TCP
Host=192.168.1.10
Port=502
Timeout=3000

[Device.Station1]
PortName=MainPort
Name=PLC1
ModbusUnit=1

[OpcUa]
Port=4840
```

### Connecting to a Controller
```bash
# Run with a configuration file
./modbusua -f /etc/modbusua/modbusua.conf

# Check connectivity
tail -f /var/log/modbusua.log
```

## Logging and Diagnostics

### Logging Levels

| Category          | Purpose                                                                            |
|:------------------|:------------------------------------------------------------------------------------|
| Error             | Critical failures that may cause stoppage or incorrect operation                    |
| Warning           | Non‑critical issues that do not stop the program                                    |
| Info              | General diagnostic messages                                                          |
| Trace             | Detailed diagnostic messages                                                          |
| TraceDetails      | Extra‑detailed diagnostic messages                                                    |
| CtorDtor          | Creation/destruction of key program components                                       |
| EntryExit         | Entry/exit of major functions                                                         |
| ThreadStartStop   | Start/stop of system threads                                                          |
| Connection        | Connect/disconnect events to remote objects over the network                          |
| Item              | Creation/execution of individual ItemReference variables                               |
| Message           | Creation/execution of message objects (packets)                                      |
| Protocol          | Modbus‑specific protocol messages                                                     |
| Receive           | Incoming Modbus network frames                                                        |
| Send              | Outgoing Modbus network frames                                                        |

### Monitoring
```bash
# Linux - systemd logs
journalctl -u modbusua -f

# Windows - Event Viewer
# Applications and Services Logs → modbusua

# Log file
tail -f modbusua.log | grep Error
```

## Development and Contribution

### Project Structure
```
modbusua/
├── src/                   # Source code
│   ├── core/              # Common classes: string, variant, thread, etc.
│   ├── ua/                # OPC UA server
│   └── device/            # Modbus devices
├── install/               # Installation scripts
│   ├── windows/           # WiX MSI installer
│   └── linux/             # DEB/RPM packages
├── doc/                   # Documentation
├── examples/              # Configuration examples
└── tests/                 # Unit tests
```

### Testing
```bash
# Run tests
cmake --build . --target test
ctest --output-on-failure

# Code coverage (enable tests)
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCN_TESTS_ENABLED=ON
```

## License

This project is licensed under the [MIT License](LICENSE).

```
MIT License

Copyright (c) 2025 serhmarch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

## Links

- Source code: https://github.com/serhmarch/modbusua
- Documentation: doc/pages/
- Issue tracking: https://github.com/serhmarch/modbusua/issues
- Modbus library: https://github.com/serhmarch/ModbusLib
- OPC UA library: https://github.com/open62541/open62541
- OPC UA standard: https://opcfoundation.org/

## Author

**serhmarch** — development and project support.

Homepage: https://github.com/serhmarch

---
modbusua Gateway — OPC UA to Modbus Gateway