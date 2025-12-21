# Core Data Types {#sec_data_common}

This section describes the gateway data types and access via the `ItemReference` mechanism.
`ItemReference` is a symbolic string that references a specific data structure.
References are case‑insensitive, i.e. `400001 s` is equivalent to `400001 S`.

Data structures may belong to a port object or to a device/PLC object; each has its own structures.

The table below shows the `Gateway` data types and their corresponding `OPC UA` types used by the I/O server:

| Gateway Type       | OPC UA Type   |
|:-------------------|:--------------|
| `Bool`             | `Boolean`     |
| `Int8`             | `SByte`       |
| `UInt8`            | `Byte`        |
| `Int16`            | `Int16`       |
| `UInt16`           | `UInt16`      |
| `Int32`            | `Int32`       |
| `UInt32`           | `UInt32`      |
| `Int64`            | `Int64`       |
| `UInt64`           | `UInt64`      |
| `Int`              | `Int32`       |
| `UInt`             | `UInt32`      |
| `Float`            | `Float`       |
| `Double`           | `Double`      |
| `String`           | `String`      |
| `Timestamp`        | `DateTime`    |
| `DateTime`         | `DateTime`    |
| `ModbusStatus`     | `Int32`       |
| `GatewayStatus`    | `Int32`       |

The `Timestamp` type represents a UTC timestamp (count of 100‑nanoseconds since 01.01.1601).

\subpage sec_data_suffix        
\subpage sec_data_statusModbus  
\subpage sec_data_statusGateway 
