# 📚 Документация проекта Sungear Engine

## 🗂️ Структура документации

| Документ | Описание | Ссылка |
|----------|----------|--------|
| **INSTALL.md** | Развёртывание репозитория: зависимости, vcpkg, переменные окружения, сборка, диагностика | [📄](./INSTALL.md) |
| **PROJECT_STRUCTURE.md** | Устройство проекта, архитектура папок, модули SGCore, взаимосвязи компонентов | [📄](./PROJECT_STRUCTURE.md) |
| **SYSTEM_DESIGN.md** | System Design движка: архитектурные решения, подсистемы, потоки данных | [📄](./SYSTEM_DESIGN.md) |
| **USER_RULES.md** | Личные правила работы с ИИ | [📄](./USER_RULES.md) |
| **DEV_RULES.md** | Правила разработки проекта, роли ИИ, стандарты кода | [📄](./DEV_RULES.md) |
| **SKILLS.md** | Каталог скиллов агента: какой брать под задачу, как дорабатывать и заводить новые | [📄](./SKILLS.md) |
| **IMPLEMENTATION_PLAN.md** | План реализации и текущий статус проекта | [📄](./IMPLEMENTATION_PLAN.md) |
| **RHI_AUDIT.md** | Аудит Graphics/API перед переходом на Vulkan/DX12 (артефакт задачи 1.1) | [📄](./RHI_AUDIT.md) |
| **RHI_DESIGN.md** | Дизайн целевого RHI: IDevice, command lists, PSO, дескрипторы (артефакт задачи 1.2) | [📄](./RHI_DESIGN.md) |

---

## 📁 Быстрые ссылки на основную документацию

| Раздел | Расположение |
|--------|--------------|
| Главный README | [/README.md](../README.md) |
| Соглашение о кодировании Pixelfield | [/CodingConvention.md](../CodingConvention.md) |
| Doxygen-документация (генерируемая) | `/documentation/en/` (`doxyfile-en` в корне) |

---

## 🔗 Навигация по ключевым компонентам

### Исходный код
- **Ядро движка**: `/Sources/SGCore/` — 30+ модулей: ECS, Render, Physics, Audio, UI, Serde и др.
- **Точка входа**: `/Sources/SGEntry/` — исполняемый файл, загружающий плагин редактора
- **Android-приложение**: `/Sources/AndroidApp/`

### Плагины
- **Редактор**: `/Plugins/SungearEngineEditor/` — отдельный CMake-проект
- **Редактор v2**: `/Plugins/SungearEngineEditor-v2/` — новая версия редактора (в разработке)

### Сборка
- **Корневой CMake**: `/CMakeLists.txt`, пресеты — `/cmake/presets/`
- **Зависимости**: `/vcpkg.json` (vcpkg — сабмодуль в `/vcpkg/`)
- **Внешние проекты**: `/Externals/` — msdf-atlas-gen (сабмодуль), rectpack2D, antlr4

### Тесты
- **Тесты**: `/Tests/` — пока только `Coro` (см. `DEV_RULES.md` → Тестирование)

### Скиллы агента
- **Каталог**: [SKILLS.md](./SKILLS.md)
- **Файлы**: `/.claude/skills/cpp-style/`, `/.claude/skills/sgcore-graphics/`

---

*Документ создан для удобной навигации ИИ и разработчиков по проекту*
