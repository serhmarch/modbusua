# modbusua Gateway

## Загальний опис

## Загальний опис

`modbusua Gateway` — це кросплатформенний (Windows, Linux) сервер вводу-виводу для перетворення даних
між протоколом Modbus та OPC UA (Open Platform Communications Unified Architecture).
Програма реалізована як сервер з відкритим вихідним кодом на C++ і 
представляє собою консольну програму/сервіс (демон у термінології Unix) .

`modbusua Gateway` призначений для збору інформації з існуючих промислових контролерів
або інших Modbus пристроїв (такі як промислові датчики, лічильники тощо)
через протокол Modbus та подальшої їх обробки і передачі через універсальний
і загальновживаний протокол OPC UA.

Проект заснований на двох бібліотеках із відкритим вихідним кодом:
* Modbus (клієнтська сторона): https://github.com/serhmarch/ModbusLib
* OPC UA ( серверна сторона ): https://github.com/open62541/open62541

## Ключові особливості

- Реалізація на C++ з використанням сучасних бібліотек (C++11 і вище)
- Кросплатформенність (Windows, Linux)
- Конфігурація через текстовий файл у форматі CONF/INI
- Логування у файл з ротацією, у системний журнал ОС Windows/Linux
- Підтримка роботи як консольна програма або як сервіс/демон
- Підтримка багатопоточності для підвищення продуктивності
- Модульна архітектура для легкого розширення функціональності
- Вбудований OPC UA сервер з можливістю налаштування адресного простору
- Підтримка протоколу Modbus TCP/RTU/ASCII для взаємодії з різними пристроями
- Налаштування параметрів з'єднання, таймаутів і повторних спроб через конфігураційний файл
- Підтримка "гарячого" оновлення конфігурації без зупинки роботи сервера
- Підтримка різних типів даних (бітові, цілі числа, числа з плаваючою комою)
- Можливість масштабування для роботи з великою кількістю пристроїв і тегів
- Вбудовані механізми діагностики та моніторингу стану сервера і з'єднань
- Гнучка система обробки помилок і відновлення з'єднань
- Можливість інтеграції з існуючими SCADA-системами та іншими програмами верхнього рівня

## Архітектура системи

```
+-------------------+       Modbus     +------------------+    OPC UA    +-----------------+
|   ПЛК/Пристрій    | <--------------> | modbusua Gateway | <--------->  |   SCADA Client  |
|                   |                  |                  |              |                 | 
| * Modbus регістри |                  | * Modbus клієнт  |              | * Збір даних    |
|                   |                  | * OPC UA сервер  |              | * Архівування   |
|                   |                  |                  |              | * Візуалізація  |
|                   |                  |                  |              |                 |
+-------------------+                  +------------------+              +-----------------+
```

## Встановлення та запуск

### Збірка з вихідних кодів

#### Вимоги
- **CMake** 3.20 або новіша
- **C++17** сумісний компілятор
- **Git** для клонування репозиторію

#### Збірка на Windows

```cmd
git clone --recursive https://github.com/serhmarch/modbusua.git
cd modbusua
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### Збірка на Linux

```bash
git clone --recursive https://github.com/serhmarch/modbusua.git
cd modbusua
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Встановлення на Windows

#### Як Windows Service

```cmd
# Встановити як службу (потрібні права адміністратора)
modbusuaservice.bat -i -n modbusuaService

# Видалити службу
modbusuaservice.bat -u -n modbusuaService
```

#### MSI Installer
Для створення MSI інсталятора використовується WiX Toolset:
```cmd
cd install/windows
cmake --build . --target modbusua-msi
```

### Встановлення на Linux

#### SystemD Service
```bash
# Копіювати файли
sudo cp modbusua /usr/bin/
sudo cp modbusua.conf /etc/modbusua/
sudo cp modbusua.service /etc/systemd/system/

# Увімкнути сервіс
sudo systemctl enable modbusua
sudo systemctl start modbusua

# Перевірити статус
sudo systemctl status modbusua
```

#### Пакети DEB/RPM
```bash
# Створити DEB пакет
cmake --build . --target modbusua-deb

# Створити RPM пакет  
cmake --build . --target modbusua-rpm
```

## Конфігурація

Конфігурація здійснюється через файл `modbusua.conf`. Основні об'єкти конфігурації:

### Об'єкт Port (Порт)
```ini
[Port]
Type=TCP
Host=192.168.1.100
Port=502
Timeout=5000
```

### Об'єкт Device (Пристрій)
```ini
[Device]
Port=Port1
Unit=1
```

### Налаштування журналювання
```ini
[Log]
Log.flags=Error|Warning|Info
Log.file.path=modbusua.log
Log.file.maxsize=10MB
```

## Опції командного рядка

| Опція             | Короткий | Параметр    | Опис                                           |
|:------------------|:---------|:------------|:-----------------------------------------------|
| `--version`       | `-v`     | -           | Показати версію програми                       |
| `--help`          | `-?`     | -           | Показати довідку                               |
| `--file`          | `-f`     | `<filename>`| Файл конфігурації (за замовчуванням modbusua.conf)|

## Приклади використання

### Базова конфігурація
```ini
# modbusua.conf
[System]
Name=modbusua Gateway
Description=OPC UA to Modbus Gateway

[Port.MainPort]
Name=MainPort
Type=TCP
Host=192.168.1.10
Port=502
Timeout=3000

[Device.Station1]
PortName=MainPort
Name=PLC1
ModbusUnit=1

[OpcUa]
Port=4840
```

### Підключення до контролера
```bash
# Запуск з конфігураційним файлом
./modbusua -f /etc/modbusua/modbusua.conf

# Перевірка підключення
tail -f /var/log/modbusua.log
```

## Журналювання та діагностика

### Рівні журналювання

| Категорія         | Призначення                                                                          
|:------------------|:-------------------------------------------------------------------------------------
| `Error`           | Критичні помилки, що можуть призвести до зупинки або некоректної роботи програми     
| `Warning`         | Некритичні помилки, що не призводять до зупинки програми                             
| `Info`            | Загальні діагностичні повідомлення                                                   
| `Trace`           | Діагностичні повідомлення для деталізації роботи програми                            
| `TraceDetails`    | Деталізовані діагностичні повідомлення для деталізації роботи програми               
| `CtorDtor`        | Повідомлення про створення/видалення основних елементів програми                     
| `EntryExit`       | Повідомлення про початок/закінчення роботи важливих функцій програми                 
| `ThreadStartStop` | Повідомлення про старт/зупинку окремих програмних потоків системи                    
| `Connection`      | Повідомлення про встановлення/розірвання з’єднання із віддаленими об’єктами по мережі
| `Item`            | Повідомлення про створення/виконання окремих змінних ItemReference                   
| `Message`         | Повідомлення про створення/виконання окремих об’єктів-повідомлень (пакетів)          
| `Protocol`        | Специфічні повідомлення протоколу Modbus
| `Receive`         | Вхідні мережеві посилки протоколу Modbus                                             
| `Send`            | Вихідні мережеві посилки протоколу Modbus                                            

### Моніторинг роботи
```bash
# Linux - systemd logs
journalctl -u modbusua -f

# Windows - Event Viewer
# Applications and Services Logs → modbusua

# Файл журналу
tail -f modbusua.log | grep Error
```

## Розробка та внесок

### Структура проекту
```
modbusua/
├── src/                   # Вихідний код
│   ├── core/              # Класи загального користування: string, variant, thread etc
│   ├── ua/                # OPC UA сервер  
│   └── device/            # Пристрої Modbus
├── install/               # Скрипти встановлення
│   ├── windows/           # WiX MSI installer
│   └── linux/             # DEB/RPM packages
├── doc/                   # Документація
├── examples/              # Приклади конфігурацій
└── tests/                 # Юніт тести
```

### Тестування
```bash
# Запуск тестів
cmake --build . --target test
ctest --output-on-failure

# Покриття коду
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCN_TESTS_ENABLED=ON
```

## Ліцензія

Цей проект ліцензований під [MIT License](LICENSE).

```
MIT License

Copyright (c) 2025 serhmarch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

## Додаткові посилання

- **Вихідний код**: [https://github.com/serhmarch/modbusua](https://github.com/serhmarch/modbusua)
- **Документація**: [doc/pages/](doc/pages/)
- **Звіти про помилки**: [GitHub Issues](https://github.com/serhmarch/modbusua/issues)  
- **Бібліотека Modbus**: [https://github.com/serhmarch/ModbusLib](https://github.com/serhmarch/ModbusLib)
- **Бібліотека OPC UA**: [https://github.com/open62541/open62541](https://github.com/open62541/open62541)
- **Протокол OPC UA**: [https://opcfoundation.org/](https://opcfoundation.org/)

## Автор

**serhmarch** - розробник та підтримка проекту.

Сторінка: https://github.com/serhmarch

---
*modbusua Gateway - OPC UA to Modbus Gateway*