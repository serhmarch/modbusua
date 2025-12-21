# Device/PLC Object {#sec_device_common}

A device object contains settings for a specific device/PLC. It is tied to a specific remote PLC. The device is configured
for Modbus network operation: standard Modbus address, maximum number of bits/registers per single message, etc.

Many device objects can be attached to a single port; exchanges with each device are performed sequentially. Using one port
and multiple devices, you can form a standard Modbus network.

The following sections describe access to device elements (data structures) via `ItemReference`, which allow reading device
status and changing its behavior.

\subpage sec_device_cfg    
\subpage sec_device_cmd    
\subpage sec_device_set    
\subpage sec_device_stat   
\subpage sec_device_mb     
