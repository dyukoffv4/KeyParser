# KeyParser - Библиотека для работы с входными параметрами приложения на C++

## **Содержание**

    1. Общие сведения
    2. Использование
      2.1 Сборка и подключение динамической библиотеки
      2.2 Использование и функционал фреймворка
      2.3 Примеры кода

---
## **1. Общие сведения**
Библиотека *KeyParser* позволяет настроить пользователю критерии для парсинга аргументов программы.

*KeyParser* поддерживает короткие и длинные имена флагов, вложенные флаги. Содержит систему отслеживания пути возникновения исключений, которая позволяет быстро понять причину возникновения ошибок в работе приложения. На данный момент *KeyParser* способен проанализировать входящие аргументы и организовать их в Задачи.

На данный момент проект собирается исключительно для ОС Linux на основе Debian.

---
## **2. Использование**

### **2.1. Сборка и подключение динамической библиотеки**

Для работы с содержимым проекта был создан bash-скрипт `manager.sh`.

Для сборки проекта и создания библиотеки следует запустить скрипт с командой `cmake`. Результаты будут храниться в директории "build/cmake".

Для сборки пакета следует запустить скрипт с командой `build`. Созданный пакет будет храниться в директории "build".

Для удаления бинарных файлов и промежуточных файлов следует запустить скрипт с командой `clean`.

    $ bash manager.sh cmake
    $ bash manager.sh build
    $ bash manager.sh clean [cmake, build]

Для установки пакета в систему выполните следующие команды:

    $ sudo apt install ./build/keyparser.deb
    $ sudo ldconfig

Для сборки программы компилятором g++ с использованием установленной библиотеки включите `-lkeyparser` в команду:

    $ g++ main.cpp -lkeyparser

### **2.2. Использование и функционал фреймворка**
Для использования элементов *KeyParser* нужно подключить заголовочный файл `parser.hpp`:

    #include <keyparser/parser.hpp>

Все элементы Фреймворка расположены в пространстве имен `keyparser`. Используется класс `Parser` и `Key`. На выход предоставляется объект класса `Tasks`.

#### **Классы:**

*`Key`* - класс, который хранит информацию о ключе и имеет необходимый функционал для его использования в `Parser`.

*`Parser`* - класс, который занимается разбором поступающих параметров и их обработкой в соответствии с настройкой соответствующих ключей.

*`Task`* - класс, который хранит разобранные ключи и параметры, предназначенные для дальнейшего выполнения на их основе работ.

***Указание диапазонов**. По умолчанию берутся все возможные аргументы. Установка параметра "-1" означает отсутствие границы диапазона слева или справа, в зависимости от номера параметра. При указании только одного параметра, диапазон будет заменен на точное число аргументов.*

### **2.3. Примеры использования**

Первый пример использования **KeyParser**: https://github.com/dyukoffv4/KustKust/tree/master/KeyParser-examples/paint
