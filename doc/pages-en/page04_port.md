# Port Object {#sec_port_common}

A port stores network settings for access to a remote Modbus device/PLC, or multiple PLCs within one network.
Access is organized sequentially: one Modbus request is sent to the first device, then the second, third, etc.

On startup, the gateway allocates a dedicated thread for each port. Each exchange cycle with a remote device runs in its
own context so network delays do not affect exchanges of other ports in the server.

The following sections describe access to port elements (data structures) via `ItemReference`, which allow reading
port status and changing its behavior.

\subpage sec_port_cfg  
\subpage sec_port_cmd  
\subpage sec_port_set  
\subpage sec_port_stat 
