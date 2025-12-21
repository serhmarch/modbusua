# Основні типи даних програми gateway {#sec_data_common}

В даному розділі описані типи даних gateway та доступ до них через механізм `ItemReference`.
`ItemReference` представляє собою символьний рядок, що задає посиланням на певну структуру даних. 
Всі посилання є незалежними від регістра символів, тобто рядок `400001 s` еквівалентний `400001 S`.

Структури даних можуть відноситись як до об’єкта-порту так і для об’єкта-пристрою/ПЛК.
Кожен із цих об’єктів має окремі структури даних.

В наступній таблиці представлені типи даних `Gateway` і їх відповідні аналоги для стандарту `OPC UA`,
які використовуються сервером вводу/виводу для передачі даних через протокол OPC UA:

| Тип даних `Gateway` | Тип даних `OPC UA` 
|:--------------------|:--------------------
| `Bool`              | `Boolean`             
| `Int8`              | `SByte`              
| `UInt8`             | `Byte`              
| `Int16`             | `Int16`              
| `UInt16`            | `UInt16`              
| `Int32`             | `Int32`              
| `UInt32`            | `UInt32`              
| `Int64`             | `Int64`              
| `UInt64`            | `UInt64`              
| `Int`               | `Int32`              
| `UInt`              | `UInt32`             
| `Float`             | `Float`               
| `Double`            | `Double`               
| `String`            | `String`             
| `Timestamp`         | `DateTime`             
| `DateTime`          | `DateTime`             
| `ModbusStatus`      | `Int32`              
| `GatewayStatus`     | `Int32`              

Тип даних `Timestamp` предаствляє собою мітку часу у форматі UTC (кількість 100-наносек починаючи із 01.01.1601).

\subpage sec_data_suffix        \n
\subpage sec_data_statusModbus  \n
\subpage sec_data_statusGateway \n
