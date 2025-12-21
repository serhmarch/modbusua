# ModbusStatus of messages {#sec_data_statusModbus}

This data type is used to diagnose results of Modbus exchanges.

 Label                             | Value                  | Description
:----------------------------------|:-----------------------|:---------------------------------------------------------------------------
 `Status_Good`                     | `0x00000000 (0)`       | Successful result
 `Status_Bad`                      | `0x01000000 (16777216)`| Error. General
 `Status_BadIllegalFunction`       | `0x01000001 (16777217)`| Standard error. Function not supported (device response)
 `Status_BadIllegalDataAddress`    | `0x01000002 (16777218)`| Standard error. Invalid data address (device response)
 `Status_BadIllegalDataValue`      | `0x01000003 (16777219)`| Standard error. Invalid data value (device response)
 `Status_BadSlaveDeviceFailure`    | `0x01000004 (16777220)`| Standard error. Failure during operation (device response)
 `Status_BadAcknowledge`           | `0x01000005 (16777221)`| Standard error. Server accepted request and is processing; will take time
 `Status_BadSlaveDeviceBusy`       | `0x01000006 (16777222)`| Standard error. Server busy with long command; retry later
 `Status_BadNegativeAcknowledge`   | `0x01000007 (16777223)`| Standard error. Programming function cannot be executed
 `Status_BadMemoryParityError`     | `0x01000008 (16777224)`| Standard error. Memory parity error detected when reading record
 `Status_BadEmptyResponse`         | `0x01000101 (16777473)`| Error. Empty request/response body
 `Status_BadNotCorrectRequest`     | `0x01000102 (16777474)`| Error. Incorrect request
 `Status_BadNotCorrectResponse`    | `0x01000103 (16777475)`| Error. Incorrect response
 `Status_BadWriteBufferOverflow`   | `0x01000104 (16777476)`| Error. Write buffer overflow
 `Status_BadReadBufferOverflow`    | `0x01000105 (16777477)`| Error. Receive buffer overflow
 `Status_BadTcpCreate`             | `0x01000501 (16778497)`| Error. Cannot create TCP socket
 `Status_BadTcpConnect`            | `0x01000502 (16778498)`| Error. Cannot establish TCP connection
 `Status_BadTcpWrite`              | `0x01000503 (16778499)`| Error. Cannot send TCP message
 `Status_BadTcpRead`               | `0x01000504 (16778500)`| Error receiving TCP message
