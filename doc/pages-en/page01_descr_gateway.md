# I/O Server Implementation Overview {#sec_descr_gateway}

[TOC]

## General Principles {#sec_descr_gateway_common}

The `modbusua` server implements an intermediate layer between existing industrial controllers/PLCs
that support Modbus, and SCADA systems or other upper‑level software that supports OPC UA.

The primary goal of `modbusua` is to collect process data from connected devices via Modbus,
process it, and forward it through the universal OPC UA protocol. The `modbusua` server is implemented
as a cross‑platform (Windows, Linux) console application/service (daemon in Unix terminology) with open‑source C++ code.

The `modbusua` server supports different device/PLC types using Modbus TCP/RTU/ASCII and provides reliable,
efficient data exchange (read/write) between these devices and OPC UA clients.

Connection parameters, timeouts, and retry policies are centrally configured via a configuration file:
for each port and/or device you define the host/address, port number and transport type (TCP/RTU/ASCII),
dedicated timeouts for establishing a connection and for read/write operations, queue limits; retry policy
includes number of attempts, interval between attempts, overall transaction timeout, and thresholds for
disconnect/reinitialize.

The `modbusua` server supports hot configuration reload without stopping:
changes from `modbusua.conf` can be applied via the OPC UA command `ReloadConfig` or by sending a `SIGHUP`
(via `systemd` `ExecReload`); new parameters are validated and atomically applied per area (global settings,
ports, devices) without breaking active TCP/IP connections; the exchange loop briefly synchronizes only the
components affected by changes, data and status buffers are preserved, and the operation status/results are
logged in detail; if validation errors occur, the changes are rejected and the server continues operating with
previous configuration.

Built‑in diagnostics and monitoring provide real‑time visibility: detailed leveled logging with rotation,
counters for successful/failed operations, last exchange time, connection status, average/max response time,
error codes; these metrics are exposed via `Stat` OPC UA nodes at server, port, and device levels, with
commands like `StatClear` to reset counters; console/syslog output and structured logs are supported, simplifying
incident diagnostics and proactive detection of channel degradation.

Data type support converts Modbus registers into standard OPC UA values: the server correctly interprets
bits (discretes), signed/unsigned integers (8/16/32/64 bit), floating‑point (IEEE‑754 32/64), accounts for
word order (word swap) and combines registers into multi‑word types.

## I/O Server General Description {#sec_descr_gateway_server}

The gateway implements an intermediate server that reads data from existing controllers using Modbus TCP, RTU or ASCII,
and then provides this data using standard SCADA nodes, in this case via an OPC UA client supplied by any modern SCADA.

This server supports exchanging with a configured number of existing controllers and is fully configurable
via the `modbusua.conf` configuration file.

The configuration consists of two main object types: the port object and the device/PLC object.
The port object includes network parameters such as device address, port number, etc., and corresponds to a single TCP/IP connection.
The device object includes settings for a specific device/PLC. Many device objects can be attached to a single port,
and exchanges with each device are performed sequentially. With one port and multiple devices, you can form a standard Modbus network.

During operation, the server writes logs of its activity. All actions exchanging with controllers, and client connections,
are written to the OS log, console, or a file, which allows monitoring server operation and diagnosing configuration/connection errors.
The logging level (amount of informational messages) and output targets are set in the same `modbusua.conf` file.

## Modbus Client Protocol Implementation {#sec_descr_gateway_protocol}

Modbus is a standard industrial protocol for data exchange between automation devices. The `modbusua` server implements
Modbus client functionality to interact with various controllers/PLCs that support the protocol. Transports include
Modbus TCP, Modbus RTU, and Modbus ASCII, enabling flexible connectivity.

Supported Modbus memory reads:
- `FC01. READ_COILS` — read Coils (`0x` memory, discrete outputs)
- `FC02. READ_DISCRETE_INPUTS` — read Discrete Inputs (`1x` memory)
- `FC03. READ_HOLDING_REGISTERS` — read Holding Registers (`4x` memory)
- `FC04. READ_INPUT_REGISTERS` — read Input Registers (`3x` memory)

Supported writes:
- `FC15. WRITE_MULTIPLE_COILS`
- `FC16. WRITE_MULTIPLE_REGISTERS`

Direct polling (read) Modbus messages have the lowest priority. Such messages are placed in a queue and processed in
idle gaps if there are no pending write messages. If the PLC connection breaks, all queued messages receive an error
status and the queue is cleared. Otherwise, they remain queued until processing becomes possible.

Direct write messages (poke) have higher priority than controller/device read messages.

## OPC UA Server Protocol Implementation {#sec_descr_opcua_protocol}

OPC UA is a modern industrial standard for secure and reliable data exchange. Unlike legacy OPC based on COM/DCOM,
OPC UA is platform‑independent, supports multiple transports, and embeds security. It follows a client‑server architecture
with a structured address space of objects, variables, and methods.

In `modbusua`, OPC UA is the primary interface for delivering process data from controllers and other Modbus devices
(e.g., industrial sensors and meters) to SCADA and other applications. Data gathered via Modbus is organized into a
standardized OPC UA address space. The address space includes branches for different types of events: general signals,
valve states, analog measurements, and system messages. OPC UA clients can subscribe to node changes and receive real‑time
notifications, providing timely delivery of process information.

OPC UA address space structure in `modbusua`:

```
Root
\-- Objects
    +-- Server
    \-- modbusua
        +-- Cfg
        |   +-- LogConsoleFormat
        |   +-- LogConsoleTimeformat
        |   \-- ...
        +-- Cmd
        |   \-- ReloadConfig
        \-- Ports
            +-- Port_1
            |   +-- Cfg
            |   |   +-- ...
            |   |   +-- Host
            |   |   +-- Port
            |   |   \-- ...
            |   +-- Cmd
            |   |   \-- StatClear
            |   +-- Set
            |   |   \-- Enable
            |   +-- Stat
            |   |   +-- ...
            |   |   +-- BadCount
            |   |   +-- GoodCount
            |   |   \-- ...
            |   \-- Devices
            |       |-- Device_1
            |       |   +-- Cfg
            |       |   |   +-- ...
            |       |   |   +-- DefaultPeriod
            |       |   |   \-- ...
            |       |   +-- Cmd
            |       |   |   +-- StatClear
            |       |   |   \-- ...
            |       |   +-- Set
            |       |   |   \-- Enable
            |       |   +-- Stat
            |       |   |   +-- ...
            |       |   |   +-- BadCount
            |       |   |   +-- GoodCount
            |       |   |   \-- ...
            |       |   |-- Items
            |       |   |   +-- ...
            |       |   |   \-- ...
            |       |   \-- Messages
            |       |       +-- ...
            |       ...     \-- ...
            \-- Port_2
                |
                ...

```

*This hierarchical structure organizes data from the system level down to specific process parameters,
allowing easy address‑space navigation and convenient OPC UA client access for monitoring and control.

The OPC UA server address space in `modbusua` is organized as a hierarchical tree that provides 
a logical arrangement of all process data and system parameters. The address space is built in
accordance with OPC UA standards and includes both standard system objects and nodes specific to `modbusua`.

### Root Level (Root)

The standard OPC UA address space starts with the root node `Root`, which contains
the standard `Objects` object — the main container for all application objects in the OPC UA server.

### System Level (Objects/Server)

The `Server` node contains standard system information about the OPC UA server, including server status,
start time, current time, and service level. Clients use this data to monitor the server state.

### Main Application Object (Objects/modbusua)

The main `modbusua` object is the container for all application functionality and includes three primary sections:

- **Cfg (Configuration)** — contains server‑wide configuration parameters such as console output format
(`LogConsoleFormat`), time format for logging (`LogConsoleTimeformat`), and other global system settings.
- **Cmd (Commands)** — contains system commands, notably `ReloadConfig` to reload the server configuration
without stopping operation.
- **Ports** — a container for all communication ports used to connect to industrial controllers and
other devices that support the Modbus protocol.

### Port Level (modbusua/Ports/Port_N)

Each port represents a separate communication line (TCP/IP connection) and contains four main sections:

- **Cfg** — port configuration parameters, including host network address (`Host`), port number (`Port`),
timeouts, and other connection settings (see [Port Configuration](#sec_port_cfg)).
- **Cmd** — port control commands, notably `StatClear` to clear port statistics
(see [Port Commands](#sec_port_cmd)).
- **Set** — current port settings, including the `Enable` parameter to enable/disable the port
(see [Port Settings](#sec_port_set)).
- **Stat** — statistical information about port operation, including counters for successful (`GoodCount`)
and failed (`BadCount`) exchanges (see [Port Statistics/Diagnostics](#sec_port_stat)).

### Device Level (Ports/Port_N/Devices/Device_N)

Each device is an individual industrial controller or PLC connected via the corresponding port.
Its structure mirrors the port and includes:

- **Cfg** — device configuration with parameters such as default polling period (`DefaultPeriod`),
Modbus addresses, buffer sizes, etc. (see [Device/PLC Configuration](#sec_device_cfg)).
- **Cmd** — device control commands, including initialization (`Init`), `StatClear`,
  database synchronization, setting time, and other system operations (see [Device/PLC Commands](#sec_device_cmd)).
- **Set** — current device settings, particularly `Enable` to enable/disable exchange
with the device (see [Device/PLC Settings](#sec_device_set)).
- **Stat** — device operation statistics with counters for successful and failed operations,
last exchange time, connection status, etc. (see [Device/PLC Statistics/Diagnostics](#sec_device_stat)).
- **Items** — contains direct Modbus data (discrete inputs/outputs, analog inputs/outputs)
read cyclically according to the configuration (see [Modbus Registers](#sec_device_mb)).
- **Messages** — contains the structure of internal Modbus messages, allowing diagnosis
of all internal Modbus frames generated for direct read/write of Modbus data.

This hierarchical structure provides a logical organization of data from the general system level down
to specific process parameters, enables easy navigation of the address space, and
gives OPC UA clients convenient access to all the information needed for monitoring and
controlling industrial processes.

## Command‑Line Options {#sec_descr_gateway_cmdoption}

Available command‑line options for the `modbusua` executable:

| Option            | Short     | Param         | Description                                   
|:------------------|:----------|:--------------|:----------------------------------------------
| `--version`       | `-v`      | `-`           | Program version                               
| `--help`          | `-h/-?`   | `-`           | Help                                          
| `--file`          | `-f`      | `<filename>`  | Specify configuration file (default `modbusua.conf`)
| `--service-name`  | `-s`      | `<name>`      | Specify service name (default `modbusua`)
| `--logdir`        |           | `<dir>`       | Specify log directory (default `log`)

## Windows Installation {#sec_descr_gateway_win}

Install `modbusua` using the `modbusua_<version>_<arch>.msi` installer. It creates all required files in the
installation folder (default `C:\Program Files\modbusua`), adds `modbusua` to Windows programs, and registers
it as a Windows service.

Additionally, you can use the helper `modbusuas.bat` to install/uninstall the Windows service.
Run it from an Administrator command prompt with options `-i`/`-u` respectively (`-n` sets an alternative
service name, `-?` or `--help` shows help).

Example:
```console
> modbusuas.bat -i My_modbusua
> modbusuas.bat -u My_modbusua
```

Installation folder layout:
```
C:\Program Files\modbusua\
+-- modbusua.exe      # I/O server executable
+-- modbusuas.bat     # Service install/uninstall utility
+-- conf\             #
|   +-- modbusua.conf # Configuration file
+-- doc\              #
|   +-- modbusua.chm  # Documentation
|   +-- changelog.md  # List of changes
\-- logs\             # Folder for log files
```

## Linux Installation {#sec_descr_gateway_linux}

For Linux, use `modbusua_<version>_<arch>.deb` (Debian/Ubuntu via `dpkg`) or
`modbusua-<version>.<arch>.rpm` (RedHat/Fedora/CentOS).

The `modbusua.service` file defines parameters to run the gateway as a service under Linux, for `systemd`.
Place it under `/etc/systemd/system`.

Key parameters in `modbusua.service`:
* `Type` — should be `forking` since the gateway daemonizes (creates a child as the main process and the parent exits).
* `ExecStart` — executable and arguments. Default `/usr/bin/modbusua -f /etc/modbusua/modbusua.conf`.
* `ExecReload` — command for config reload. Default `kill -HUP $MAINPID`.
* `WorkingDirectory` — working directory for relative paths. Default `/var/lib/modbusua`.

Example `modbusua.service` content:
```conf
[Unit]
Description=OPC UA - Modbus Gateway
After=network.target

[Service]
Type=forking
ExecStart=/usr/bin/modbusua -f /etc/modbusua/modbusua.conf
ExecReload=kill -HUP $MAINPID
WorkingDirectory=/var/lib/modbusua

[Install]
WantedBy=multi-user.target
```

Ensure all files are in place and required directories (e.g., `WorkingDirectory`) exist.

### Additional script for installing/removing the `modbusuas.sh` service {#sec_descr_driver_linux_svc}

Additionally, to install/remove `modbusua` from the list of systemd services, you can use
the auxiliary utility `modbusuas.sh`, which must be run from the command line
in administrator mode and use the command-line options `-i`/`-u` respectively
(you can specify an alternative name for the service, `-h`, `-?`, or `--help` to display help).

For example:
```bash
$ sudo modbusuas.sh -i MyModbusUa
$ sudo modbusuas.sh -u MyModbusUa
```

You can specify additional parameters for the service, such as the log directory `--logdir <dir>`
or the configuration file `--file <filename>`, which will be passed to the `modbusua` executable
when the service starts. To do this, you must specify a special separator `--`,
which separates the script parameters (on the left) from the program parameters (on the right), for example:

```bash
$ sudo modbusuas.sh -i modbusua2 -- -f /etc/modbusua/modbusua2.conf --logdir /tmp
```

### Basic systemd commands (Linux) {#sec_descr_gateway_linux_cmd}

* List services:
```console
$ systemctl list-units
```
* Show service status:
```console
$ systemctl status modbusua
```
* Show unit file:
```console
$ systemctl cat modbusua
$ systemctl --no-pager cat modbusua
```
* Start service:
```console
$ sudo systemctl start modbusua
```
* Stop service:
```console
$ sudo systemctl stop modbusua
```
* Reload configuration (after `modbusua.conf` change):
```console
$ sudo systemctl reload modbusua
```
* Reload systemd (after `modbusua.service` change):
```console
$ sudo systemctl daemon-reload
```
* Enable at boot:
```console
$ sudo systemctl enable modbusua
```
* Disable at boot:
```console
$ sudo systemctl disable modbusua
```

Typical Linux filesystem layout:
```
/                                # Root filesystem
+-- usr/
|   +-- bin/
|       +-- modbusua             # I/O server executable
+-- etc/
|   +-- modbusua.conf            # Configuration file
|   +-- systemd/
|       +-- system/
|           +-- modbusua.service # systemd unit file
\-- var/
    +-- lib/
    |   +-- modbusua/            # Working directory
    +-- log/
        +-- modbusua/            # Log files
```
