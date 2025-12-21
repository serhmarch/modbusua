# Gateway Status {#sec_data_statusGateway}

This data type indicates the status of `modbusua` data.

 Label                  | Value                     | Description
:-----------------------|:--------------------------|:------------------------------------------------------------
 `Status_Processing`    | `0x80000000 (-2147483648)`| Operation not finished; further processing required
 `Status_Bad`           | `0x40000000 (1073741824)` | Operation ended with error
 `Status_BadOutOfService`| `0x40000001 (1073741825)`| Error. Data unavailable
 `Status_BadNotConnected`| `0x40000002 (1073741826)`| Error. No connection
 `Status_BadConfigure`  | `0x40000003 (1073741827)` | Configuration error
 `Status_BadNoService`  | `0x40000004 (1073741828)` | Error. Cannot start service
 `Status_Uncertain`     | `0x20000000 (536870912)`  | Operation status is uncertain
 `Status_Good`          | `0x00000000 (0)`          | Operation succeeded
 `Status_GoodNoData`    | `0x00000001 (1)`          | Operation succeeded. No new data
