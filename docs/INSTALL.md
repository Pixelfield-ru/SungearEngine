# ⚙️ INSTALL — развёртывание репозитория

Инструкция для нового участника: как поднять проект с нуля на рабочей станции.

> **Документ живой.** Появляется новая инфраструктурная зависимость или шаг
> сборки — они добавляются сюда сразу же, вместе с кодом, который их использует.
> Порядок описан в [`DEV_RULES.md`](./DEV_RULES.md) и [`USER_RULES.md`](./USER_RULES.md).

## 📋 Оглавление

1. [Что понадобится](#1-что-понадобится)
2. [Клонирование](#2-клонирование)
3. [Bootstrap vcpkg](#3-bootstrap-vcpkg)
4. [Переменная окружения SUNGEAR_SOURCES_ROOT](#4-переменная-окружения-sungear_sources_root)
5. [Конфигурация CMake и пресеты](#5-конфигурация-cmake-и-пресеты)
6. [Сборка ядра](#6-сборка-ядра)
7. [Сборка редактора](#7-сборка-редактора)
8. [Запуск](#8-запуск)
9. [Тесты](#9-тесты)
10. [Диагностика частых проблем](#10-диагностика-частых-проблем)

---

## 1. Что понадобится

| Компонент | Версия | Обязателен | Зачем |
|---|---|---|---|
| CMake | 3.22+ | да | система сборки |
| Компилятор C++23 | MSVC 2022 / Clang / GCC | да | стандарт задан жёстко (`CMAKE_CXX_STANDARD 23`) |
| Git | любая | да | репозиторий + сабмодули (vcpkg, msdf-atlas-gen) |
| Java **JDK** (не только JRE) | 8+, проверено с Temurin 21 | да | Две причины: (1) корневой CMake делает `find_package(JNI REQUIRED)` — нужны заголовки `jni.h`, `JAVA_HOME` должен указывать на JDK; (2) `Sources/SGCore/CMakeLists.txt` регенерирует CSS-парсер командой `java -jar Externals/antlr4/antlr-4.13.1-complete.jar` **при обычной сборке SGCore** — сгенерированные файлы лежат в git, но после свежего клона их mtime старше грамматик, и Ninja вызывает `java`. Проверено 2026-08-17: без `java` в PATH сборка падает на первом же шаге |
| Python | 3.x | да | требуется vcpkg-портами (skia и др.) при сборке зависимостей |
| Ninja | любая | рекомендуется | генератор для пресетов; входит в workload C++ Visual Studio |
| Диск | десятки ГБ | да | vcpkg собирает ~30 зависимостей из исходников (в т.ч. skia и boost) |
| Android NDK | — | только для Android | пресет `arm64-android` |
| Vulkan SDK (LunarG) | 1.3+ | рекомендуется | Заголовки, loader и VMA приезжают из vcpkg (`vulkan-headers`, `vulkan-loader`, `vulkan-memory-allocator`), собрать Vulkan-бэкенд можно и без SDK. SDK нужен ради **validation layers** (`VK_LAYER_KHRONOS_validation`): в Debug движок включает их автоматически, без SDK пишет предупреждение и работает без проверок. Для отладки кадра — RenderDoc |

Первая сборка зависимостей vcpkg — **долгая** (часы на слабой машине из-за
skia/boost/assimp). Это разовая цена: результат кешируется в
`vcpkg/vcpkg_installed/`.

### Установка инструментов одной командой

**Windows** (PowerShell, winget):

```powershell
winget install Microsoft.VisualStudio.2022.Community --override "--add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended --passive"
winget install EclipseAdoptium.Temurin.21.JDK
winget install Python.Python.3.12
winget install Git.Git
```

Workload `NativeDesktop` включает MSVC (C++23), Windows SDK, CMake и Ninja.

**Linux** (Debian/Ubuntu):

```bash
sudo apt install build-essential g++-13 cmake ninja-build git curl zip unzip tar pkg-config \
     python3 default-jdk \
     libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
     libasound2-dev libpulse-dev autoconf automake libtool
```

`g++-13`+ или `clang-17`+ (нужен C++23); `default-jdk` закрывает Java и JNI;
X11/GL/ALSA-dev — системные требования glfw3 и openal-soft из vcpkg.

---

## 2. Клонирование

```bash
git clone --recurse-submodules -j8 https://github.com/Pixelfield-ru/SungearEngine
cd SungearEngine
```

Если репозиторий уже склонирован без сабмодулей (или скачан архивом):

```bash
git submodule update --init --recursive
```

Сабмодули: `vcpkg/` и `Externals/msdf-atlas-gen` (ветка v1.3). Без них
конфигурация CMake не пройдёт.

---

## 3. Bootstrap vcpkg

```powershell
# Windows
.\vcpkg\bootstrap-vcpkg.bat
```

```bash
# Linux/macOS
./vcpkg/bootstrap-vcpkg.sh
```

Зависимости ставить вручную **не нужно**: проект работает в манифестном
режиме (`vcpkg.json` в корне), установка происходит при конфигурации CMake.
Используются оверлей-триплеты из `cmake/vcpkg-triplets/`.

---

## 4. Переменная окружения SUNGEAR_SOURCES_ROOT

Пресеты CMake и подключение движка из плагинов берут пути из системной
переменной `SUNGEAR_SOURCES_ROOT` — она должна указывать на корень checkout'а:

```powershell
# Windows (PowerShell, от имени пользователя)
[Environment]::SetEnvironmentVariable('SUNGEAR_SOURCES_ROOT', 'E:\Github\SungearEngine', 'User')
```

```bash
# Linux (добавить в ~/.bashrc или ~/.profile)
export SUNGEAR_SOURCES_ROOT=$HOME/dev/SungearEngine
```

⚠️ После установки переменной **перезапустите IDE** (README рекомендует
перезагрузить ПК — важно, чтобы переменную увидел процесс IDE/терминала).
Через неё резолвятся `CMAKE_TOOLCHAIN_FILE`, `VCPKG_INSTALLED_DIR`
и `VCPKG_OVERLAY_TRIPLETS` (см. `cmake/presets/base-presets.json`).

---

## 5. Конфигурация CMake и пресеты

Пресеты подключаются корневым `CMakePresets.json` из `cmake/presets/`:

| Пресет | Платформа | Тип |
|--------|-----------|-----|
| `debug-x64-windows-static-md` | Windows x64 | Debug |
| `release-x64-windows-static-md` | Windows x64 | Release |
| `debug-x64-linux` | Linux x64 | Debug |
| `release-x64-linux` | Linux x64 | Release |
| пресеты из `arm64-android.json` | Android arm64 | — |

⚠️ README упоминает имена `debug-host`/`release-host` — фактические имена
пресетов см. в таблице (источник истины — `cmake/presets/*.json`).

```bash
cmake --preset debug-x64-windows-static-md
```

Первый запуск конфигурации соберёт все зависимости vcpkg — см. §1 про время.

Флаги (сейчас захардкожены в `ON` в корневом `CMakeLists.txt`):

- `SG_BUILD_TESTS` — сборка тестовых таргетов (`Tests/`);
- `SG_BUILD_ENTRY` — сборка исполняемого файла `SGEntry`.

---

## 6. Сборка ядра

```bash
cmake --build cmake-build-debug-x64-windows-static-md --target SGCore
```

Либо в IDE: открыть корневой `CMakeLists.txt`, выбрать пресет и таргет.

⚠️ **Windows**: после сборки скопируйте
`<binary-dir>/Sources/SGCore/SGCore.dll` в каталог с исполняемым файлом,
который собираетесь запускать. Функция `copy_sgcore_dlls()` в
`cmake/utils.cmake` сейчас закомментирована, автоматического копирования нет.

После каждой сборки срабатывает таргет `auto_install` — он выполняет
`cmake --install` в `CMAKE_INSTALL_PREFIX`.

---

## 7. Сборка редактора

Редактор — отдельный CMake-проект в `Plugins/SungearEngineEditor/`.

1. Убедиться, что ядро собрано **тем же пресетом**, каким будет собираться
   редактор — иначе бинарная несовместимость.
2. Открыть `Plugins/SungearEngineEditor/` как CMake-проект.
3. Выбрать пресет и таргет, собрать.

Движок редактор находит через `SUNGEAR_SOURCES_ROOT`
(`cmake/SungearEngineInclude.cmake`).

---

## 8. Запуск

1. Собрать CMake-таргет `SungearEngine` (SGEntry).
2. Запустить `<binary-dir>/Sources/SGEntry/SungearEngine` — точка входа
   загрузит плагин редактора через `PluginsManager`.

На Windows не забыть про `SGCore.dll` рядом с exe (§6).

### Выбор графического API

Бэкенд выбирается на старте по списку предпочтения (`GAPISelector`,
см. [RHI_DESIGN.md](./RHI_DESIGN.md#командные-списки-и-кадр)); недоступные
в этой сборке пропускаются с записью в лог. Принудительно задать бэкенд —
переменной окружения `SG_GAPI` (для диагностики драйверных проблем):

```powershell
$env:SG_GAPI = 'gl4'      # Windows, текущая сессия
```

```bash
SG_GAPI=gl46 ./SungearEngine   # Linux
```

Допустимые значения: `gl4`, `gl46`, `gles2`, `gles3`, `vulkan`, `dx12`.
Реально доступны на 2026-08-17: `gl46` (дефолт на ПК), `gl4`, `vulkan` (RHI работает,
`SGRHITest` проходит; legacy-фасады шейдеров/мешей ещё заглушки, поэтому Vulkan **вне**
дефолтного списка предпочтения — только через `SG_GAPI=vulkan` или `--gapi vulkan` в
тестах, полноценная сцена на нём пока не рисуется); `dx12` появится на этапе 3. Если
принудительный бэкенд недоступен, движок откатывается к списку предпочтения и пишет
предупреждение в лог.

---

## 9. Тесты

Тестовые таргеты (`Tests/`, собираются при `SG_BUILD_TESTS`):

```bash
cmake --build <binary-dir> --target SGCoroTest
<binary-dir>/Tests/Coro/SGCoroTest
```

### Шейдерный пайплайн (`SGShadersTest`)

Юнит-проверки прохода вулканизации SGSL (`SGSLEVulkanizer`) на встроенных
снипетах, плюс прогон по всему корпусу `Resources/sg_shaders/**/*.sgshader`
с дампом Vulkan-GLSL для просмотра. Окно и GPU не нужны:

```bash
cmake --build <binary-dir> --target SGShadersTest
cd <binary-dir>/Tests/Shaders          # рядом должна лежать SGCore.dll (§6)
SGShadersTest                                          # только юнит-проверки
SGShadersTest --corpus E:/Github/SungearEngine --out vulkanized          # + корпус
SGShadersTest --corpus E:/Github/SungearEngine --out vulkanized --spirv  # + SPIR-V + рефлексия
```

`--spirv` компилирует каждую программу корпуса в SPIR-V (glslang, Vulkan 1.3)
и печатает сводку рефлексии; `.spv` пишутся рядом с `.glsl`. Код выхода `0` —
юнит-проверки прошли (и корпус обработан); падение SPIR-V-компиляции пишется
в сводку `spirv: N OK, M failed`. Транслятор пишет отладочный дамп
`SGSLETranslatorOutputDebug/` и `logs/` в текущий каталог — запускать из
build-dir, не из корня репозитория. Зависимости `glslang`/`spirv-reflect`
ставятся vcpkg автоматически при конфигурации (добавлены 2026-08-17).

### Вертикальный срез RHI (`SGRHITest`)

Рисует треугольник через новый RHI (`IDevice`/`ICommandList`): вулканизированный
шейдер, UBO по рефлексии, PSO, readback с проверкой точных значений пикселей, вывод
на экран и readback экрана. Один и тот же тест для всех бэкендов. Нужны окно и GPU.

```bash
cmake --build <binary-dir> --target SGRHITest
# запуск из корня репозитория: шейдер экрана грузится по ${enginePath} = ./Resources
<binary-dir>/Tests/RHI/SGRHITest --gapi gl46
<binary-dir>/Tests/RHI/SGRHITest --gapi vulkan
```

Рядом с exe должна лежать `SGCore.dll` (§6). Код выхода `0` — PASS. Проверено
2026-08-17 (NVIDIA GTX 1660 Ti): gl46 и vulkan, пиксели совпадают байт в байт. На
Vulkan в Debug включены validation layers — их сообщения идут в лог как
`[Vulkan validation]`; строки `loader_get_json`/`Removing layer` про сторонние слои
(Bandicam, OBS, EOS overlay) — шум системы, не ошибка движка.

### Смоук-сцена рендера (`SGSmokeTest`)

Эталонная сцена для сравнения графических бэкендов: PBR-тела, CSM-тени,
атмосфера, прозрачный объект, SSAO. Рендерит N кадров, читает кадр обратно
и пишет PNG; с `--reference` сравнивает и возвращает код выхода.

```bash
cmake --build <binary-dir> --target SGSmokeTest
cd <binary-dir>/Tests/Smoke          # рядом должна лежать SGCore.dll (§6)

SGSmokeTest --gapi gl46                         # снять кадр → smoke_gl46.png
SGSmokeTest --gapi gl4 --reference ../../../Tests/Smoke/references/smoke_gl_1920x1080.png   # сравнить с эталоном
SGSmokeTest --help
```

Коды выхода: `0` — кадр снят (и совпал с эталоном), `1` — расхождение
(рядом пишется `*_diff.png`), `2` — снять не удалось. Пороги: `--threshold`
(разница по каналу, по умолчанию 8/255) и `--max-diff` (доля отличающихся
пикселей, по умолчанию 1 %).

Эталоны лежат в `Tests/Smoke/references/` и именуются
`smoke_<gapi>_<ширина>x<высота>.png`; `smoke_gl_…` — общий эталон для GL4 и
GL46 (они обязаны совпадать попиксельно). ⚠️ Размер кадра равен разрешению
**основного монитора** (`LayeredFrameReceiver` создаёт фреймбуферы под него),
поэтому эталон привязан к разрешению; на другом мониторе тест честно
сообщит size mismatch — снимите свой эталон. Эталоны обновляются осознанно,
при намеренном изменении картинки, и коммитятся вместе с изменением.

Проверено 2026-08-17 (NVIDIA, 1920×1080): GL4 и GL46 детерминированы и
совпадают друг с другом (0 % расхождений, max diff 0). Дефолтный выбор
бэкенда без `SG_GAPI` — GL46 (после недоступных DX12/Vulkan).

GTest заявлен в `vcpkg.json`, но в `Tests/Coro/CMakeLists.txt` закомментирован —
тест сейчас обычный исполняемый файл. Состояние тестирования — в
[DEV_RULES.md → Тестирование](./DEV_RULES.md#-тестирование).

---

## 10. Диагностика частых проблем

| Симптом | Причина | Что делать |
|---|---|---|
| CMake не находит toolchain / `CMAKE_TOOLCHAIN_FILE` пустой | Не задана `SUNGEAR_SOURCES_ROOT` или IDE её не видит | §4: задать переменную, перезапустить IDE/ПК |
| Конфигурация падает на пустом `vcpkg/` или `Externals/msdf-atlas-gen/` | Не инициализированы сабмодули | `git submodule update --init --recursive` |
| Сборка SGCore падает на шаге «Generating ANTLR4 parser and lexer» (`java: command not found`) | Нет Java в PATH | Установить JRE 8+ и перезапустить IDE (§1) |
| В логе `No graphics API from the preference list is available` | Ни один бэкенд не создался (напр. `SG_GAPI` указывает на нереализованный, а список предпочтения пуст) | Снять `SG_GAPI` или указать `gl4`; см. §8 |
| exe мгновенно завершается / «не найдена SGCore.dll» | DLL не скопирована к исполняемому файлу | §6: скопировать `SGCore.dll` вручную |
| Плагин редактора не загружается | Ядро и редактор собраны разными пресетами | Пересобрать оба одним пресетом |
| Первая конфигурация «висит» часами | vcpkg собирает skia/boost/assimp из исходников | Это норма для первого раза; кеш в `vcpkg/vcpkg_installed/` |
| Ошибки про макрос `ERROR` при сборке с ANTLR4 на Windows | Конфликт WinAPI (wingdi.h) с `ParseTreeType::ERROR` | Уже закрыто: `NOGDI`, `NOMINMAX`, `WIN32_LEAN_AND_MEAN` определены в корневом CMake — не удалять |
| MSVC: `fatal error C1128` (too many sections) | Большие объектники | Уже закрыто флагом `/bigobj` в корневом CMake |
| Кириллица в консоли выводится мусором | Кодировка терминала Windows | `[Console]::OutputEncoding=[Text.Encoding]::UTF8` |
| Тест падает с диалогом «Run-Time Check Failure #2 — Stack around the variable … was corrupted» | exe теста собран до изменения заголовков SGCore, а `SGCore.dll` — после: расхождение раскладки структур (ABI). Классический случай — struct, возвращаемый по значению из DLL, вырос | Пересобрать **все** тестовые exe после правки заголовков (`--target SGRHITest SGShadersTest SGSmokeTest SGCoroTest`), скопировать свежую `SGCore.dll` рядом с каждым |

---

*Документ дополняется по мере появления новых шагов развёртывания и разобранных проблем.*
