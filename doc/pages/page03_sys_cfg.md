# Конфігурація системи modbusua {#sec_sys_cfg}

Використовується для зчитування конфігураційних параметрів роботи драйвера. Дані `ItemReference` є ReadOnly.

| `ItemReference`       | Тип даних | Призначення                                                              
|:----------------------|:----------|:-------------------------------------------------------------------------
| `LogFlags`            | `String`  | Список прапорів, що задають рівень логування
| `LogOutput`           | `String`  | Список типів журналювання для виводу
| `LogDefaultFormat`    | `String`  | Формат одного повідомлення за замовчуванням ([див](#sec_journal_common))
| `LogDefaultTimeformat`| `String`  | Формат часу одного повідомлення за замовчуванням ([див](#sec_journal_common))
| `LogSystemFormat`     | `String`  | Формат одного повідомлення для системного журналу ([див](#sec_journal_common))
| `LogSystemTimeformat` | `String`  | Формат часу одного повідомлення для системного журналу ([див](#sec_journal_common))
| `LogConsoleFormat`    | `String`  | Формат одного повідомлення для виводу у консоль ([див](#sec_journal_common))
| `LogConsoleTimeformat`| `String`  | Формат часу одного повідомлення для виводу у консоль ([див](#sec_journal_common))
| `LogFileFormat`       | `String`  | Формат одного повідомлення для виводу у файл ([див](#sec_journal_common))
| `LogFileTimeformat`   | `String`  | Формат часу одного повідомлення для виводу у файл ([див](#sec_journal_common))
| `LogFilePath`         | `String`  | Розташування файлу, у який будуть виводитись повідомлення
| `LogFileMaxCount`     | `Int32`   | Максимальна кількість файлів для ротації журналу
| `LogFileMaxSize`      | `Int32`   | Максимальний розмір файла ротації журналу (в байтах)
