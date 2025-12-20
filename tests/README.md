# Юніт-тести modbusua

## Призначення

Цей каталог містить юніт-тести для проєкту `modbusua`, написані з використанням GoogleTest/GoogleMock. 
Тести перевіряють роботу ядра (core), пристроїв (device), портів (port), OPC UA-шару (ua) та допоміжних модулів.

## Попередні вимоги
- CMake 3.13+
- Компилятор C++17
- Наявні вихідні залежності, що збираються разом із проєктом (ModbusLib, open62541)

> Примітка: У кореневому `CMakeLists.txt` уже інтегровано CTest і ввімкнено авто-реєстрацію тестів через `gtest_discover_tests`.

## Варіант A -- запуск через CTest (рекомендовано)

### Windows (MSVC + Presets)
1) Налаштувати та зібрати тести (Debug):
```powershell
cmake --preset Win64-MSVC-Tests
cmake --build --preset Win64-MSVC-Debug-Tests
```

2) Запустити всі тести:
```powershell
ctest --preset Win64-Tests-Debug
```
3) Запустити підмножину (фільтр за іменем тесту/фікстури):
```powershell
ctest --preset Win64-Tests-Debug -R CnFileInfoTest
```
4) Переглянути список зареєстрованих тестів:
```powershell
ctest --preset Win64-Tests-Debug -N
```

### Linux (GCC/Clang + Presets)
1) Налаштувати та зібрати тести (Debug):
```bash
cmake --preset Linux-Debug-Tests
cmake --build --preset Linux-Debug-Tests
```
2) Запустити всі тести:
```bash
ctest --preset Linux-Tests-Debug
```
3) Запустити підмножину:
```bash
ctest --preset Linux-Tests-Debug -R CnFileInfoTest
```
4) Переглянути список тестів:
```bash
ctest --preset Linux-Tests-Debug -N
```

## Варіант B — прямий запуск виконуваного файлу тестів

Виконавчий файл тестів генерується як `testmodbusua`.

### Поширені параметри командного рядка (GoogleTest)
Найуживаніші ключі для керування запуском тестів:

- `--gtest_list_tests`: показати список усіх доступних тестів/фікстур (без запуску).
- `--gtest_filter=PATTERN`: запуск лише вибраних тестів. Підтримує `*` і множинні шаблони
через `:` та виключення через `-`.
	- Приклади: `CnFileInfoTest.*`, `CnDirTest.*:CnFileTest.*`, `* -CnDirTest.flaky_case`.
- `--gtest_repeat=N`: повторити запуск N разів (наприклад, для виявлення нестабільних тестів).
Використовуйте `--gtest_break_on_failure` для зупинки при першій помилці.
- `--gtest_break_on_failure`: зупинити виконання при першому падінні.
- `--gtest_shuffle`: перетасувати порядок тестів; опційно `--gtest_random_seed=SEED`.
- `--gtest_output=xml[:FILE]`: зберегти результати у форматі JUnit XML для CI.
Якщо `FILE` не задано — створюється за замовчуванням у поточній теці.
- `--gtest_color=yes|no|auto`: керування кольорами в консолі.
- `--gtest_also_run_disabled_tests`: також запускати тестові кейси, позначені як відключені.
- `--gtest_brief`: коротший вивід (менше подробиць про успішні тести).


### Windows
- Шлях за замовчуванням (Debug):
```powershell
$env:USERPROFILE\tmp\cmake\modbusua\bin\Win64-Tests\Debug\testmodbusua.exe
```
- Приклади запуску:
```powershell
# Усі тести
your\path\to\testmodbusua.exe

# Фільтрація GoogleTest
your\path\to\testmodbusua.exe --gtest_filter="CnFileInfoTest.*"

your\path\to\testmodbusua.exe --gtest_filter="CnDirTest.absoluteFilePath_test"

# Список тестів (без запуску)
your\path\to\testmodbusua.exe --gtest_list_tests

# Повторний запуск і зупинка на першій помилці
your\path\to\testmodbusua.exe --gtest_repeat=10 --gtest_break_on_failure

# Перемішати порядок тестів із фіксованим зерном
your\path\to\testmodbusua.exe --gtest_shuffle --gtest_random_seed=123

# Експорт результатів у JUnit XML (для CI)
your\path\to\testmodbusua.exe --gtest_output=xml:results\gtest-report.xml
```

### Linux
- Типовий шлях (Debug):
```bash
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua
```
- Приклади запуску:
```bash
# Усі тести
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua

# Фільтрація GoogleTest
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_filter="CnFileInfoTest.*"

# Список тестів (без запуску)
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_list_tests

# Повторний запуск і зупинка на першій помилці
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_repeat=10 --gtest_break_on_failure

# Перемішати порядок тестів із фіксованим зерном
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_shuffle --gtest_random_seed=123

# Експорт результатів у JUnit XML (для CI)
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_output=xml:results/gtest-report.xml
```

### Приклади використання (швидкий старт)

#### Windows (PowerShell)
```powershell
# 1) Один конкретний тест (точне ім'я Suite.Test)
$exe = "$env:USERPROFILE\tmp\cmake\modbusua\bin\Win64-Tests\Debug\testmodbusua.exe"
& $exe --gtest_filter="CnDirTest.filePath_test"

# 2) Кілька груп тестів (через двокрапку) та виключення (через мінус)
& $exe --gtest_filter="CnDirTest.*:CnFileInfoTest.*-*flaky*"

# 3) Повторити 50 разів і зупинитись на першій помилці
& $exe --gtest_repeat=50 --gtest_break_on_failure

# 4) Перетасувати порядок тестів із фіксованим зерном
& $exe --gtest_shuffle --gtest_random_seed=42

# 5) Експорт звіту у JUnit XML
if (!(Test-Path results)) { New-Item -ItemType Directory results | Out-Null }
& $exe --gtest_output=xml:results\gtest-report.xml
```

#### Linux (bash)
```bash
# 1) Один конкретний тест (точне ім'я Suite.Test)
exe="$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua"
"$exe" --gtest_filter="CnDirTest.filePath_test"

# 2) Кілька груп тестів та виключення
"$exe" --gtest_filter="CnDirTest.*:CnFileInfoTest.*-*flaky*"

# 3) Повторити 50 разів і зупинитись на першій помилці
"$exe" --gtest_repeat=50 --gtest_break_on_failure

# 4) Перетасувати порядок тестів із фіксованим зерном
"$exe" --gtest_shuffle --gtest_random_seed=42

# 5) Експорт звіту у JUnit XML
mkdir -p results
"$exe" --gtest_output=xml:results/gtest-report.xml
```

## Поради та діагностика
- Щоб увімкнути збірку тестів вручну, використовуйте `-DCN_TESTS_ENABLED=ON` (у пресетах це вже зроблено).
- Для детальнішого логування GoogleTest додайте
`--gtest_repeat=1 --gtest_break_on_failure` або `--gtest_also_run_disabled_tests`.
- Список тестів усередині виконуваного файлу:
```bash
./testmodbusua --gtest_list_tests
```
- Деякі тести файлової системи працюють відносно поточної теки.
Фікстури тестів створюють необхідні тимчасові каталоги й, за потреби,
тимчасово перемикають поточну робочу теку під час виконання тестів.

## Структура цього каталогу
```
./tests
├── core/      # Тести для ядра (рядки, час, ФС тощо)
├── device/    # Тести для пристроїв і їхніх елементів
├── port/      # Тести для портів
├── ua/        # Тести OPC UA-шару
├── include/   # Допоміжні заголовки для тестів
├── CMakeLists.txt
└── README.md  # Цей файл
```

## Типові сценарії
- Повна перебудова тестів:
```powershell
# Windows
cmake --preset Win64-MSVC-Tests
cmake --build --preset Win64-MSVC-Debug-Tests
ctest --preset Win64-Tests-Debug --output-on-failure
```
```bash
# Linux
cmake --preset Linux-Debug-Tests
cmake --build --preset Linux-Debug-Tests
ctest --preset Linux-Tests-Debug --output-on-failure
```

---
Докладніше про збірку і запуск — див. кореневий `README.md`.