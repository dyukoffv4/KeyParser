# KeyParser - Фреймворк для работы с входными параметрами приложения C++

## **Содержание**

    1. Общие сведения
    2. Использование
      2.1 Сборка и подключение динамической библиотеки
      2.2 Использование и функционал фреймворка
      2.3 Примеры кода
    3. Тестирование

---
## **1. Общие сведения**
Фреймворк *KeyParser* позволяет настроить обработку поступающих параметров в приложение написанное на C++ через `argc` и `argv` переменные.

*KeyParser* поддерживает короткие и длинные имена ключей, вложенные ключи, имеет специальные функции обработки данных, такие как обработка данных, введенных без ключа.
*KeyParser* бесопасен по памяти (не требует дополнительного выделения или очистки памяти), обработка данных производится за счет простых функций и не требует написания виртуальных классов и методов к ним.
Также у *KeyParser* имеется система отслеживания пути возникновения исключений, позволяющая быстро понять причину возникновения ошибок в работе приложения.

Сборка проекта настроенна только для UNIX систем, сборку под Windows и MacOS следует настраивать самостоятельно.

---
## **2. Использование**

### **2.1. Сборка и подключение динамической библиотеки**

Для работы с содержимым проекта был создан bash-скрипт `controller`.
Для сборки `.deb` пакета следует запустить скрипт с командой `build`. Созданный пакет будет находится в директории "out".
Для удаления бинарных файлов и промежуточных файлов следует запустить скрипт с командой `clean`.

    KeyParser$ bash controller build
    KeyParser$ bash controller clean

### **2.2. Использование и функционал фреймворка**
Для использования элементов *KeyParser* нужно подключить заголовочный файл `terminal.hpp`:

    #include <keyparser/terminal.hpp>

Все элементы Фреймворка расположены в пространстве имен `KP`. К использованию рекомендуются только два класса: `Terminal` и `Key`.

**`Key`** - класс, который хранит информацию о ключе и имеет необходимый функционал для его использовании в `Terminal`. Ниже представлен список методов и структур:

**Структуры:**
* *enum State{E, S, L, A}* - Перечисления состояний ключа.
* *enum zoneState{ZS_L, ZS_I, ZS_H}* - Перечисления отношений чисел по отношению к диапозону ключа.

**Конструкторы:**
* *explicit Key(const char& s_data, int f_num = -1, int s_num = -1)* - Конструктор для задания ключа с коротким именем.
* *explicit Key(const std::string& l_data, int f_num = -1, int s_num = -1)* - Конструктор для задания ключа с длинным именем.
* *explicit Key(const char& s_data, const std::string& l_data, int f_num = -1, int s_num = -1)* - Конструктор для задания ключа с полным именем.

*Опционально можно задать диапазон числа параметров (`num` и `hnum`). По умолчанию будут браться все возможные параметры, если указан только первый параметр диапазон будет взят от 0 до первого параметра*

**Методы:**
* *State getState() const* - Метод для получения состояния ключа: E - нулевой ключ, S - короткий ключ, L - длинный ключ, A - полный ключ.
* *bool operator<(const Key& key) const* - Оператор сравнения ключей (Используется в `std::map`).
* *bool operator==(const Key& key) const* - Оператор равенства ключей.
* *zoneState operator[](const int& num) const* - Оператор определения попадания значения в диапазон параметров ключа: ZS_L - меньше диапазона, ZS_I - внутри диапазона, ZS_H - больше диапазона.
* *char sname() const* - Метод для получения короткого имени ключа.
* *std::string lname() const* - Метод для получения длинного имени ключа.
* *std::string fname() const* - Метод для получения полного имени ключа.

**`Terminal`** - класс, который занимается разбором поступающих параметров и их обработкой в соответствии с настройкой соответствующих ключей. Ниже представлен список методов и структур:

**Структуры:**
* *enum rootState{RS_F, RS_S, RS_A}* - Перечисления способов обработки корневых параметров.
    *RS_F* - обработка параметров только до первого ключа (первые в очереди).
    *RS_S* - обработка всех поступивших параметров вместе (последние в очереди).
    *RS_A* - обработка каждых поступивших параметров по отдельности, как у обычных ключей (в порядке очереди).

**Конструкторы:**
* *Terminal(rootState state = RS_F)* - Стандартный конструктор.

**Методы:**
* *Terminal& operator=(const Terminal& term)* - Оператор присваивания Терминала. 
* *void setKey(Key key, void (\*lnr)(Args) = nullptr)* - Метод для установки ключа, вторым параметром передается ссылка на функцию вида `void F(KP::Args)`.
* *void delKey(Key key)* - Метод для удаления указанного ключа.
* *void setRootRange(int f_num, int s_num)* - Установка диапазона числа параметров для корневых параметров.
* *void setRootState(rootState state)* - Установка способа обработки корневых параметров.
* *void setRoot(void (\*lnr)(Args))* - Метод для установки обработчика для корневых параметров.
* *void delRoot()* - Метод удаления обработчика для корневых параметров.
* *void setFinal(void (\*lnr)(Args))* - Метод для установки финальной функции.
* *void delFinal()* - Метод для удаления финальной функции.
* *void cleanBinds()* - Метод для удаления всех установленных ключей и функций.
* *void execute(int argc, char\* argv[])* - Метод для обработки переданных параметров и исполнения функций-обработчиков над ними. Принимает на вход аргументы функции `main`. Игнорирует первый параметр (имя исполняемого файла).
* *void execute(Args input)* - Метод для обработки переданных параметров и исполнения функций-обработчиков над ними. Принимает на вход вектор параметров (строк).

### **2.3. Примеры использования**

Первый пример использования **KeyParser**: https://github.com/dyukoffv4/KustKust/tree/master/KeyParser-examples/paint

---
## **3. Тестирование**

Тестовые функции располагаются в директории "test/src". Для их сборки предусмотрен Makefile. Пример сборки и запуска тестов:

    KeyParser$ make -C test/
    KeyParser$ test/out/libtest

Тесты для исполнения записываются в файл "test/src/main.cpp", в функцию `main()`. При необходимости можно отредактировать файл "test/src/main.cpp", добавить новые тестовые файлы и добавить зависимости в Makefile.
