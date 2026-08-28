---
name: cpp-style
description: >-
  Когда использовать: любой C++/заголовки в SungearEngine — модули SGCore
  (ECS, Render, Serde, UI, Physics и др.), SGEntry, плагины редактора, тесты.
  Соглашение о кодировании Pixelfield: именование файлов и типов
  (UpperCamelCase), функций и локальных переменных (lowerCamelCase), префиксы
  m_/s_ у членов, lower_snake_case для constexpr/using/typedef, правила
  пробелов и скобок, порядок членов структуры, инклуды <> vs "", запрет
  using namespace, смарт-поинтеры и RAII, C++23.
  Triggers: naming convention, UpperCamelCase, lowerCamelCase, m_ prefix,
  s_ prefix, snake_case, brace style, member order, include style, using
  namespace, smart pointers, header file naming, CodingConvention, C++ style,
  clang-format.
---

# C++: соглашение о кодировании Pixelfield

Стиль кода для всего C++ в репозитории. Первоисточник —
[`CodingConvention.md`](../../../CodingConvention.md) в корне; при конфликте
этого скилла с ним выигрывает первоисточник, а скилл нужно поправить.

⚠️ `.clang-format` в репозитории нет — стиль автоматикой не проверяется,
соблюдение целиком на авторе и ревью.

---

## 📋 Структура документа
- [Именование](#именование)
- [Пробелы и скобки](#пробелы-и-скобки)
- [Порядок членов структуры](#порядок-членов-структуры)
- [Инклуды](#инклуды)
- [Практики](#практики)
- [Как это ложится на Sungear Engine](#как-это-ложится-на-sungear-engine)

---

## Именование

| Что | Стиль | Пример |
|-----|-------|--------|
| Файлы C++ (.h/.cpp) | UpperCamelCase, по главной структуре файла | `Position.h` / `Position.cpp` |
| Файл без главной структуры | имя по назначению | `DataTypes.h` |
| Типы: struct/class/enum/union/namespace | UpperCamelCase | `struct MyStruct`, `namespace MyNamespace` |
| Функции | lowerCamelCase | `void moveTo(...)` |
| Локальные переменные | lowerCamelCase | `float myLocalVariable { };` |
| Нестатические члены | префикс `m_` + lowerCamelCase | `float m_x { };` |
| Статические члены | префикс `s_` + lowerCamelCase | `static inline float s_counter { };` |
| События и коллбеки | lowerCamelCase **без префикса** | `Event<void()> onClicked;` |
| Шаблонные параметры | UpperCamelCase; `T`-префикс, если имя совпадает с именем возможной переменной | `template<typename SomeType, FormatType TFormatType>` |
| Макросы | UPPER_SNAKE_CASE или lower_snake_case | `#define MY_MACRO_0` |
| constexpr, using, typedef | lower_snake_case | `using my_type = T;`, `static constexpr inline std::size_t num_dimensions = 3;` |

### Аббревиатуры

- В **начале** имени — строчными: `m_aabbMember`, `void aabbFunc()`.
- В **середине/конце** — прописными: `m_parentAABB`, `void resizeAABB(AABB& aabb)`.

---

## Пробелы и скобки

- После `(` и перед `)` пробелов нет: `doSomething(float a)`.
- Перед `;` пробела нет, после — есть: `for(int i = 0; i < 3; ++i)`.
  Обратите внимание: **между `for`/`if`/`while`/`catch` и `(` пробела нет** —
  так во всех примерах конвенции.
- Бинарные операторы (`+ - * / = == !=`) — по одному пробелу с обеих сторон.
- `[`, `]`, `<`, `>` — без внутренних пробелов: `std::unordered_map<int, int>`.
- Запятая: пробела до нет, после — есть.
- Двоеточие — пробел до и после: `for(const auto& v : values)`.
- Инициализация фигурными скобками — пробел внутри: `auto myVar = { 3, 4, 5 };`,
  пустая инициализация — `float m_x { };`.
- **Скобки тела** функций, классов, структур, неймспейсов, юнионов —
  **на новой строке**:

```cpp
namespace MyNamespace
{
    struct MyString
    {
        void doSomething()
        {
        }
    };
}
```

- Лямбды — скобка на той же строке или на новой, обе формы допустимы;
  однострочная лямбда — пробелы внутри скобок: `auto f = []() { return 2 + 2; };`
- Обращение к неймспейсам/статике — без пробелов вокруг `::`:
  `Core::Internal::doSomethingOther();`

---

## Порядок членов структуры

Публичные члены — **сверху**, приватные — **внизу**. Внутри каждого блока:

1. Вложенные типы (под-структуры, enum'ы).
2. Переменные, using'и, typedef'ы.
3. Функции.

```cpp
struct MyStruct
{
    struct MySubStruct { };

    using type = float;
    static constexpr int my_constexpr_var = 4;
    float m_myVar = 3.14f;

    void doSomething();

private:
    float m_myInternalVariable = 9.8f;

    void doSomethingInternal();
};
```

---

## Инклуды

- Сторонние библиотеки — треугольные скобки: `#include <SomeLibrary/Test.h>`.
- Файлы текущего проекта — кавычки: `#include "MyProject/Test.h"`.

В движке встречаются оба стиля для SGCore-заголовков (`"SGCore/..."` внутри
ядра, `<SGCore/...>` из внешних потребителей вроде плагинов) — это соответствует
правилу: для плагина SGCore — сторонний проект.

---

## Практики

1. **Не использовать `using namespace`.**
2. **Смарт-поинтеры** — при разделяемом владении или когда нужен RAII;
   следить за циклическими ссылками (`weak_ptr` для обратных связей).
3. **Избегать итерации по map'ам** (`std::map`, `std::unordered_map` и любым
   другим) — это правило конвенции; горячие пути строить на векторах/пулах
   (в ECS данные и так лежат в пулах EnTT).
4. Стандарт — **C++23** (`CMAKE_CXX_STANDARD 23`, REQUIRED): корутины,
   концепты и прочие возможности стандарта использовать можно, ломать сборку
   более старым стилем «на всякий случай» не нужно.

---

## Как это ложится на Sungear Engine

Раздел для проектной специфики — дополняется по мере разбора граблей.

- **Ядро — разделяемая библиотека.** Публичные классы SGCore экспортируются
  через макросы из `Sources/sgcore_export.h`; новый публичный класс ядра без
  экспорта не будет виден из SGEntry/плагинов на Windows.
- **Платформенные ветки** — только через макросы вида `SG_PLATFORM_OS_WINDOWS`
  (в коде) и `SG_TARGET_OS_*` (в CMake, из `cmake/platform.cmake`).
- **Debug-код** — под `SUNGEAR_DEBUG` (определяется при `CMAKE_BUILD_TYPE=Debug`).
- **Не удалять дефайны-костыли** из корневого `CMakeLists.txt` (`NOGDI`,
  `NOMINMAX`, `WIN32_LEAN_AND_MEAN`): они закрывают конфликт WinAPI-макросов
  с ANTLR4 (`ParseTreeType::ERROR`) и `std::min/max`.
- **Инициализация членов** — фигурными скобками с пробелами: `float m_x { };`
  (см. примеры в конвенции); не оставлять члены неинициализированными.
- **Компоненты ECS** — данные без тяжёлой логики; логика — в системах,
  обходящих `Registry`. Визиторы компонентов — `SGCore/ECS/Visitors.h`.
- Новые файлы кладутся в модуль по подсистеме (`Sources/SGCore/<Module>/`);
  список модулей и их назначение — в
  [`docs/PROJECT_STRUCTURE.md`](../../../docs/PROJECT_STRUCTURE.md#ядро-движка-sgcore).
