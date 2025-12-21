# Статистика/діагностика пристрою/ПЛК {#sec_device_stat}

Використовується для діагностики роботи gateway. Дані `ItemReference` є ReadOnly.

| `ItemReference`                      | Тип даних         | Призначення                                                                                       |
|:-------------------------------------|:------------------|:--------------------------------------------------------------------------------------------------|
| `State`                              | `UInt32`          | Масив бітів поточного стану Gateway
| `StateEnableDevice`                  | `Bool`            | Є дозвіл комунікації (відповідає біту 0 Stat.State)
| `StateConnectionAlive`               | `Bool`            | Є зв'язок з ПЛК (відповідає біту 2 Stat.State)
| `StateIncluded`                      | `Bool`            | Даний ПЛК включений в ланцюг виконання. Він може бути виключений якщо відбулась помилка зв’язку і таймаут відновлення (RestoreTimeout) ще не завершений (відповідає біту 10 Stat.State)
| `CommonGoodCount`                    | `UInt32`          | Загальна кількість успішних Modbus-посилок
| `CommonBadCount`                     | `UInt32`          | Загальна кількість неуспішних Modbus-посилок
| `CommonStatus`                       | `ModbusStatus`    | Статус останньої Modbus-посилки (\ref sec_data_statusModbus)
| `CommonTimestamp`                    | `Timestamp`       | Час (PC) останньої Modbus-посилки
| `CommonLastSuccessTimestamp`         | `Timestamp`       | Час (PC) останньої успішної Modbus-посилки
| `CommonLastErrorStatus`              | `ModbusStatus`    | Статус останньої помилки Modbus-посилки (\ref sec_data_statusModbus)
| `CommonLastErrorTimestamp`           | `Timestamp`       | Час (PC) останньої неуспішної Modbus-посилки
| `CommonLastErrorText`                | `String`          | Текстове представлення останньої помилки Modbus-посилки
| `PokeGoodCount`                      | `UInt32`          | Кількість успішних простих Modbus-Write-посилок
| `PokeBadCount`                       | `UInt32`          | Кількість неуспішних простих Modbus-Write-посилок
| `PokeStatus`                         | `ModbusStatus`    | Статус останньої простої Modbus-Write-посилки (\ref sec_data_statusModbus)
| `PokeTimestamp`                      | `Timestamp`       | Час (PC) останньої простої Modbus-Write-посилки
| `PokeLastSuccessTimestamp`           | `Timestamp`       | Час (PC) останньої успішної простої Modbus-Write-посилки
| `PokeLastErrorStatus`                | `ModbusStatus`    | Статус останньої помилки простої Modbus-Write-посилки (\ref sec_data_statusModbus)
| `PokeLastErrorTimestamp`             | `Timestamp`       | Час (PC) останньої неуспішної простої Modbus-Write-посилки
| `PokeLastErrorText`                  | `String`          | Текстове представлення останньої помилки простої Modbus-Write-посилки
| `PollGoodCount`                      | `UInt32`          | Кількість успішних простих Modbus-Read-посилок
| `PollBadCount`                       | `UInt32`          | Кількість неуспішних простих Modbus-Read-посилок
| `PollStatus`                         | `ModbusStatus`    | Статус останньої простої Modbus-Read-посилки (\ref sec_data_statusModbus)
| `PollTimestamp`                      | `Timestamp`       | Час (PC) останньої простої Modbus-Read-посилки
| `PollLastSuccessTimestamp`           | `Timestamp`       | Час (PC) останньої успішної простої Modbus-Read-посилки
| `PollLastErrorStatus`                | `ModbusStatus`    | Статус останньої помилки  простої Modbus-Read-посилки (\ref sec_data_statusModbus)
| `PollLastErrorTimestamp`             | `Timestamp`       | Час (PC) останньої неуспішної простої Modbus-Read-посилки
| `PollLastErrorText`                  | `String`          | Текстове представлення останньої помилки простої Modbus-Read-посилки
