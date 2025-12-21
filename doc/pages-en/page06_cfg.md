# Gateway Configuration {#sec_cfg_common}

This server supports exchanges with a configured number of devices/PLCs and is fully configurable.

Configuration is provided via a conf/ini‑style configuration file. It may consist of multiple files.
Additional files can be included via the `include` key in the `SYSTEM` section:
```ini
[SYSTEM]
include=modbusua2.conf
```

Detailed configuration per section:
* \subpage sec_cfg_sys 
* \subpage sec_cfg_port
* \subpage sec_cfg_device 

Example configuration:
```ini
[SYSTEM]
Name="test"
include="modbusua2.conf"          
Log.flags="Error|Warning|Info|Trace|TraceDetails|CtorDtor|EntryExit|ThreadStartStop|Connection|Item|Message|Protocol|Receive|Send"           
#Log.flags=Error|Warning|Info 

Log.output="file console"
Log.default.format="[--- %time ---] %text"
Log.default.timeformat="%Y-%M-%D %h:%m:%s.%f"
#Log.concole.format=
#Log.console.timeformat=
Log.file.format="%time;%cat;%text"
#Log.file.timeformat=
Log.file.path="log.csv"

[PORT]
Name=PORT1
Enable=1
RepeatCount=2
Type="TCP" # TCP RTU ASC
Host="127.0.0.1"
Port=502
Timeout=2000
SerialPortName="COM1"
BaudRate=9600       
DataBits=8     
Parity="No" # No Even Odd Space Mark 
StopBits="1" # 1 1.5 2      
FlowControl="No" # No Hard Soft
TimeoutFirstByte=1000
TimeoutInterByte=50

[DEVICE]
PortName=PORT1
Name=PLC1
EnableDevice=1
ModbusUnit=1
RestoreTimeout=10000
MaxReadCoils=2000,
MaxWriteMultipleCoils=2000
MaxReadDiscreteInputs=2000
MaxReadInputRegisters=120
MaxReadHoldingRegisters=120
MaxWriteMultipleRegisters=120
DefaultPeriod=500
RequestTimeout=5000

itemfile="PLC.csv"

i:Test_Int16="400001 S"
```
