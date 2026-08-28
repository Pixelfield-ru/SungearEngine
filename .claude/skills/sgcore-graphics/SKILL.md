---
name: sgcore-graphics
description: >-
  Когда использовать: любая работа с графическим слоем SGCore — Graphics/API
  (IRenderer, IFrameBuffer, ITexture2D, IShader, GAPIType, GAPISelector),
  GL-бэкенды (GL4Renderer/GL46Renderer, GL4FrameBuffer, GL4Texture2D), рендер-проходы
  (Render/PBRRP, CSM, Batching), шейдеры SGSL (Resources/sg_shaders, SGSLETranslator),
  readback/скриншоты, смоук-тест SGSmokeTest, выбор бэкенда через SG_GAPI, ECS-компоненты
  рендера (Camera3D, LayeredFrameReceiver, CSMTarget, Controllable3D, Atmosphere, Mesh).
  Проверенные факты и грабли: форматы attachment'ов (не предполагать RGBA8), кто отбрасывает
  тени (только Batch), Controllable3D перезаписывает поворот камеры, размер фреймбуфера
  = размер монитора, GL46Renderer сломан, GAPIType/SGG*-enum'ы глобальные, protected-поля
  IMaterial, компоненты хранятся по значению.
  Triggers: framebuffer, attachment, glReadPixels, readback, screenshot, texture format,
  SGGColorFormat, SGGDataType, IRenderer, GAPIType, SG_GAPI, GL46, shadows, CSM, Batch,
  smoke test, SGSL, sgshader, uniform, sampler, LayeredFrameReceiver, Controllable3D,
  RHI, Vulkan backend, DX12 backend.
---

# SGCore: графический слой — проверенные факты и грабли

Скилл накапливает то, что **проверено кодом, сборкой или запуском** (даты — когда
проверено). Догадка о поведении API здесь стоит дороже одного чтения: если факта
нет — смотреть исходники, а не предполагать. Дизайн целевого RHI —
[`docs/RHI_DESIGN.md`](../../../docs/RHI_DESIGN.md), аудит —
[`docs/RHI_AUDIT.md`](../../../docs/RHI_AUDIT.md).

## 📋 Структура документа
- [Форматы, enum'ы, кастеры](#форматы-enumы-кастеры)
- [Attachment'ы и readback](#attachmentы-и-readback)
- [Выбор бэкенда](#выбор-бэкенда)
- [Состояние бэкендов](#состояние-бэкендов)
- [ECS-компоненты рендера: что где перезаписывается](#ecs-компоненты-рендера-что-где-перезаписывается)
- [Тени и батчинг](#тени-и-батчинг)
- [Шейдеры SGSL](#шейдеры-sgsl)
- [RHI (новый слой, 2026-08-17)](#rhi-новый-слой-2026-08-17)
- [Vulkan-бэкенд (2026-08-17)](#vulkan-бэкенд-2026-08-17)
- [Смоук-тест](#смоук-тест)
- [Мелкие грабли API](#мелкие-грабли-api)

---

## Форматы, enum'ы, кастеры

- `SGGColorFormat`, `SGGColorInternalFormat`, `SGGDataType`, `SGFrameBufferAttachmentType`
  и прочие `SGG*` объявлены **глобально** в `Graphics/API/GraphicsDataTypes.h`, а не в
  `namespace SGCore`. `SGCore::SGGColorFormat` — ошибка компиляции (проверено 2026-08-17).
- `GAPIType` — unscoped enum в `SGCore` (`SG_API_TYPE_GL4/GL46/GLES2/GLES3/VULKAN/DX12`);
  хелперы `isOpenGLAPI()`, `isExplicitAPI()`, `gapiTypeToString()`, `gapiTypeFromString()`
  там же (`Graphics/API/GAPIType.h`). Сравнения диапазонов enum'а (`>= GL4 && <= GLES3`)
  не писать — использовать хелперы.
- Перевод в GL — только через `GLGraphicsTypesCaster` (`Graphics/API/GL/GLGraphicsTypesCaster.h`):
  `sggFormatToGL`, `sggDataTypeToGL`, `sggInternalFormatToGL`. Размеры — хелперы в
  `GraphicsDataTypes.h`: `getSGGFormatChannelsCount(format)`,
  `getSGGDataTypeSizeInBytes(dataType)`, `getSGGInternalFormatChannelsSizeInBytes(internalFormat)`.
- Все 244 прямых `gl*`-вызова живут внутри `Graphics/API/GL/` (аудит 2026-08-16); вне
  бэкенда GL не вызывать.

## Attachment'ы и readback

- **У attachment'а свой формат.** `ITexture2D` хранит `m_format` (R/RG/RGB/RGBA/BGR/BGRA,
  `*_INTEGER`, depth/stencil), `m_internalFormat`, `m_dataType`, `m_channelsCount`.
  Attachment может быть `R`, `RG`, integer (ID пикинга) или float. **Никогда не
  предполагать RGBA8** — замечание Ilya 2026-08-16 к первой версии readback'а.
- Полное чтение attachment'а: `IFrameBuffer::readAttachmentPixels(type, AttachmentReadback&)`
  — читает **в родном формате** (`glReadPixels` с `sggFormatToGL(m_format)` /
  `sggDataTypeToGL(m_dataType)`), возвращает байты + раскладку. Приведение к RGBA8 —
  на вызывающей стороне (пример: `Tests/Smoke/ImageCompare.cpp::toRGBA8`). Для integer
  attachment'ов чтение как `GL_RGBA/GL_UNSIGNED_BYTE` = `GL_INVALID_OPERATION`.
- Однопиксельное чтение под мышью — `readPixelsFromAttachment(mousePos, type)` (пикинг),
  возвращает `glm::vec3` через `GL_FLOAT`.
- Строки в readback'е GL идут **снизу вверх** — для PNG нужен флип
  (`isOpenGLAPI(...) → flipVertically`).
- **Аплоад текстуры на Vulkan тоже обязан конвертировать формат** (исправлено 2026-08-18) — зеркало
  правила про readback ниже. `glTexImage2D` приводит CPU-буфер к внутреннему формату на лету, а
  `vkCmdCopyBufferToImage` копирует байты как есть, поэтому `VkTexture2D::uploadRegion` строит staging
  в раскладке образа (`VulkanTypesCaster::formatLayout`), декодируя канал за каналом (`float32→float16`
  и т. п.), и оставляет memcpy только при точном совпадении раскладок. Без этого аплоад отклонялся —
  так шумовая текстура SSAO оставалась пустой и `normalize(vec3(0))` = NaN выключал весь эффект.
- **На Vulkan readback обязан конвертировать формат сам.** `glReadPixels` приводит данные к
  запрошенному `(format, dataType)`, а `vkCmdCopyImageToBuffer` — сырое копирование памяти: в staging
  ложится раскладка **образа**. А объявленный `m_dataType` часто с ней расходится: attachment'ы 4–6
  `LayeredFrameReceiver` объявлены `SGG_RGB16_FLOAT` + `SGG_FLOAT` (4 байта), тогда как реальный
  `VkFormat` — `R16G16B16A16_SFLOAT` (2 байта на канал, +расширение до RGBA). Если считать размер по
  `m_dataType`, staging заполняется наполовину и декодируется со сдвигом — G-буфер выглядит чёрным с
  одной полосой (потеряно время 2026-08-18 на сравнение мусора). Теперь `VkFrameBuffer::readAttachmentPixels`
  берёт раскладку образа через `VulkanTypesCaster::formatLayout(VkFormat)` и декодирует канал за каналом
  (half→float и т. п.), сохраняя быстрый memcpy, когда раскладки совпадают точно.
- **Размер фреймбуферов `LayeredFrameReceiver` = разрешение основного монитора**
  (`LayeredFrameReceiver.cpp:47,142`), не окна. Скриншот на 1080p-мониторе — 1920×1080
  независимо от размера окна; менять размер окна ради размера кадра бесполезно.
- Финальный кадр камеры — `LayeredFrameReceiver::m_layersFXFrameBuffer`, attachment
  `SGG_COLOR_ATTACHMENT7` (`BasicApp::m_attachmentToDisplay`).

## Выбор бэкенда

- Бэкенд выбирает `GAPISelector::selectRenderer()` (`Graphics/API/GAPISelector.h`,
  вызывается из `CoreMain::init()`): переменная окружения `SG_GAPI` (`gl4`, `gl46`,
  `vulkan`, `dx12`…) → список предпочтения (`setPreference()` до `init()`, платформенный
  дефолт). Недоступные в сборке бэкенды пропускаются с логом; при откате — предупреждение.
- Рантайм-откат с пересозданием окна пока не реализован (задача 1.3a плана):
  `GL4Renderer::init()` при провале `confirmSupport()` вызывает `setShouldClose(true)`.
- Окно: для `isExplicitAPI` ставится `GLFW_CLIENT_API = GLFW_NO_API` (`Main/Window.cpp`).
- Проверить выбор из теста: `SGSmokeTest --gapi gl46` — использует `setPreference({type})`.

## Состояние бэкендов

- **`GL46Renderer` — основной GL-бэкенд** (решение 2026-08-17): GL4-реализация на
  контексте 4.6, `#version 460 core` + дефайн `SG_GLSL4`; переопределяет только
  `confirmSupport`/`createShader`. `GL4Renderer` (`#version 400 core`) — второй в списке.
  Смоук на gl46 и gl4 совпадает попиксельно (общий эталон `smoke_gl_1920x1080.png`).
- Грабли, из-за которых GL46 был «сломан» до 2026-08-17: (1) `createShader()` без
  `addDefine("SG_GLSL4")` — весь код шейдеров под `#if defined(SG_GLSL4)` исчезает,
  стадии пустые → «No input primitive type» и чёрный кадр; **любой новый GL-бэкенд
  обязан добавлять этот дефайн**; (2) `GL46Texture2D` — заглушка 2023 г.
  (`glTextureParameteri(GL_GENERATE_MIPMAP)` = `GL_INVALID_ENUM`, захардкоженный
  `GL_UNSIGNED_BYTE`, нет buffer/3D/compressed-путей) — удалена, DSA возвращать
  пообъектно под смоук-контролем. `GL46FrameBuffer`/`GL46UniformBuffer` не используются
  (`GL4Renderer` создаёт GL4-варианты).
- Первым симптомом «шейдер пустой» в логе будет ошибка компиляции геометрической стадии
  про примитивы, а не про сам код — проверять дефайны раньше, чем GLSL.
- **Vulkan (`Graphics/API/Vulkan/`, этап 2, 2026-08-17)**: RHI работает (`SGRHITest --gapi
  vulkan` PASS, ноль ошибок validation), legacy-фасады частично — см. раздел «Vulkan-бэкенд»
  ниже. Vulkan **вне** дефолтного списка предпочтения `GAPISelector`, пока смоук на нём не идёт
  (только `SG_GAPI=vulkan` / `setPreference`).
- ImGui-бэкенд захардкожен на `imgui_impl_opengl3` (`ImGuiWrap/ImGuiLayer.cpp`).

## ECS-компоненты рендера: что где перезаписывается

- Компоненты хранятся **по значению** (`reg_t` = сам тип): `registry->get<T>(e)` — ссылка,
  `tryGet<T>(e)` — сырой указатель, `emplace/remove/allOf` — как у EnTT.
  `tryGet(entt::null)` в Debug ассертит — проверять сущность на `entt::null`.
- **`Controllable3D` каждый кадр перезаписывает `Transform::m_localTransform.m_rotation`**
  из своих `m_pitchYawRoll` (`Controllables3DUpdater.cpp:44`), а те скачут от дельт
  курсора на первых кадрах. Для фиксированной позы камеры компонент снять
  (`registry->remove<Controllable3D>(camera)`) либо `m_localTransform.m_blockRotation = true`.
- Камера: `Transform + Camera3D + RenderingBase + LayeredFrameReceiver`
  (+ `Controllable3D` для управления, + `CSMTarget` для теней) — см. `BasicApp::initImpl`.
- Конвенции осей: `MathUtils::forward3 = (0,0,-1)`, `up3 = (0,1,0)`, `right3 = (-1,0,0)`.
- `Atmosphere` — компонент на скайбокс-меше; солнце задаётся `m_sunRotation` (градусы,
  `AtmosphereUpdater` вращает по Z, затем по Y); `{0, 35, 40}` даёт низкое солнце.
- `BasicApp::start(true)` создаёт: пайплайн PBRRP, сцену, скайбокс+`Atmosphere`
  (`m_atmosphereEntity` = `skyboxEntities[2]`, может остаться `entt::null`, если не
  загрузился `cube_model`), камеру с SSAO. Конфиг `SungearEngineConfig.json` — не
  обязателен (ошибка в логе, работает дальше).
- Импорт модели: `ModelAsset::m_rootNode->addOnScene(scene)` возвращает сущности,
  `[0]` — корень (его `Transform` двигать/масштабировать), меши — дочерние с `Mesh`,
  `Transform`, `Pickable`, `EnableMeshPass` и тегом `OpaqueEntityTag`/`TransparentEntityTag`
  **по материалу меша на момент импорта** (`IMeshData.cpp:229`). Сменил материал на
  `MAT_BLEND` — перетегируй руками.

## Тени и батчинг

- **`SunShadowsPass` рендерит в каскады только `view<Batch, ShadowCaster>`**
  (`Render/BaseRenderPasses/SunShadowsPass.cpp`) — обычные меши с `ShadowCaster` теней
  не отбрасывают. `CSMTarget` на камере + `ShadowCaster` на мешах недостаточно; нужен
  `Batch` (`Batch::insertEntities`). Батчинг — отдельная подсистема (атлас, TBO), меши
  в батче продолжают рендериться и обычным проходом, пока не снят `EnableMeshPass`.
  Правильный путь для теней от обычных мешей — открытый вопрос (1.6a плана).
- CSM читается в `PBRRPOpaqueMeshesPass` через `CSMTarget::bindUniformsToShader`.

## Шейдеры SGSL

- SGSL (`Utils/SGSL/SGSLETranslator`) — препроцессор над GLSL: `#include`,
  `#vertex/#fragment/#geometry/#compute/#tess_control/#tess_eval … #end`,
  `#subpass[Name]` (обязателен), `#attribute[name][value]` (становятся `#define`).
  Код вне `#…#end` — общий для всех субшейдеров. Транслятор **не** генерирует
  `#version` и биндинги — это делает GL-шейдер при компиляции
  (`GL46Shader::compileSubShader`: `#version <m_version>` + дефайны + код).
- Корпус (2026-08-17): 24 `.sgshader` + 59 `.glsl` в `Resources/sg_shaders/`;
  **140 свободных не-opaque `uniform`** вне блоков, **100 сэмплеров без `binding`**,
  8 UBO (`layout(std140)`) без `binding`, 9 `samplerBuffer` (батчинг через TBO),
  10 `gl_FragColor`, 8 `gl_VertexID`, 17 файлов с юниформами под `#if`. Всё это
  несовместимо с Vulkan-GLSL напрямую — нужен проход «вулканизации» (задача 1.4).
- Include-корень: `SGSLETranslator::includeDirectory(<root>/Resources)`; общие файлы —
  `sg_shaders/impl/glsl4/{defines,structs_decl,uniform_bufs_decl}.glsl`.
- Runtime-перекомпиляция: `IShader::m_autoRecompile`.
- ⚠️ **Две ловушки препроцессора SGSL, обе стоили полной пересборки** (2026-08-22):
  (1) `#ifndef` не переживает трансляцию — на выходе остаётся голый «#», GL-драйвер отвечает
  «syntax error, unexpected $undefined at token "#"», а guarded-блок бесследно исчезает. Писать
  `#ifdef … #else … #endif`, как весь остальной корпус. (2) **Хвостовой `//`-комментарий склеивает
  строку со следующей**: комментарии вырезаются, строки объединяются, и директива уезжает в конец
  предыдущего выражения (`layout(location = 0) out vec4 outColor; #else`). Комментарий перед
  директивой — только отдельной строкой. Симптом у обеих: «undefined variable» на переменную,
  объявление которой на глаз присутствует. Смотреть дамп в `SGSLETranslatorOutputDebug/`, а не
  исходник.
- **Варианты одной программы делаются дефайном**, а не копией файла: `addDefine` + `recompile()` на
  ассете, загруженном через `loadAssetWithAlias` (иначе получишь тот же объект). Так сделаны
  инстансинг (`SG_INSTANCED_RENDERING` поверх `StandardMeshShader`), декали и контур
  (`SG_OUTLINE_COMBINE`). Отдельный `.sgshader` под вариант — лишняя сущность: так и появился
  мёртвый `pbr/instancing.sgshader` с несуществующим инклудом.
- Каждый `.sgshader` обёрнут в guard варианта `#if defined(SG_GLSL4) || defined(SG_GLES32)`
  (ветка `SG_HLSL` — `#error`), внутри — `#include` реализации из `impl/glsl4/`.
  SGSL-препроцессор разворачивает `#include` **не глядя на `#if`** — условия
  остаются в тексте для GLSL-компилятора.
- `SGSLETranslator::processCode` работает без окна/GPU (инклуды через `AssetManager`
  как `TextFileAsset`), но **всегда** пишет дамп в `SGSLETranslatorOutputDebug/` и
  `logs/` в текущий каталог (флаг `m_useOutputDebug` не проверяется) — запускать
  инструменты из build-dir.
- **Вулканизация** (`Utils/SGSL/SGSLEVulkanizer`, 2026-08-17): переводит стадии
  программы в Vulkan-GLSL — свободные юниформы стадии → std140-блок
  `SGLegacyUniforms_<stage>` с экземпляром `sg_SGLegacyUniforms_<stage>`,
  обращения переписаны в `instance.member`; `set/binding` сэмплерам/UBO/TBO
  (явные сохраняются), `#if`-условия членов сохраняются с вырезанным общим
  guard'ом файла, блок ставится не раньше объявления `struct`-типов членов,
  `gl_FragColor`→`sgFragColor` + `out`, `gl_VertexID`→`gl_VertexIndex`.
  Обрабатывать **всю программу разом** — биндинги должны совпасть между стадиями.
  Дефайны приписывать до вулканизации. Массивы сэмплеров с макро-размером
  (`[SG_SPOT_LIGHTS_MAX_COUNT]`) дают предупреждение и count = 1 — брать из
  рефлексии. Тест: `Tests/Shaders` (`--corpus`, `--spirv`).
- **Почему блок пер-стадийный, а не общий** (грабли 2026-08-17): (1) стадия видит
  только свои `struct`-типы — `screen.glsl`/`div.glsl` инклудят
  `uniform_bufs_decl.glsl` только в вершинной стадии, общий блок с
  `ObjectTransform` не компилировался во фрагментной; (2) члены **анонимных**
  блоков глобальны для программы — glslang не линкует две стадии, где одно имя
  принадлежит разным анонимным блокам («Anonymous member name used for global
  variable or other anonymous member»). Отсюда именованный экземпляр и
  переписывание обращений. Фасад `useX("имя")` пишет во все блоки с таким членом.
- **SPIR-V + рефлексия** (`Graphics/SPIRV/SPIRVCompiler`, `ShaderReflection`):
  glslang 15.1, `EShClientVulkan`/`EShTargetVulkan_1_3`/`EShTargetSpv_1_6`,
  `setAutoMapLocations(true)` + `program.mapIO()` — авто-локации in/out
  согласованы между стадиями (шейдеры движка их не пишут); SPIRV-Reflect даёт
  set/binding, тип дескриптора, count, члены блоков (offset/size/paddedSize),
  вершинные входы, push constants; блоки именуются **именем типа блока**, не
  экземпляра. Проверено: 23/23 программ корпуса компилируются.
  vcpkg-цели: `glslang::glslang`, `glslang::SPIRV`,
  `glslang::glslang-default-resource-limits`, `unofficial::spirv-reflect`
  (не `unofficial::spirv-reflect::spirv-reflect`).
- `features/pbr/instancing.sgshader` — мёртвый: инклудит несуществующий
  `impl/glsl4/pbr/instancing.glsl` (0 стадий при трансляции).

## RHI (новый слой, 2026-08-17)

- Интерфейсы — `Graphics/RHI/` (`IDevice`, `ICommandList`, `ISwapchain`, `IGPUBuffer`,
  `IShaderProgram`, `IPipelineState`/`PipelineStateDesc`, `IDescriptorSet`, `RHITypes.h`);
  доступ — `CoreMain::getRenderer()->getDevice()` (nullptr у бэкендов без RHI; у GL46 —
  после `init()`). Дизайн — `docs/RHI_DESIGN.md`.
- GL46-реализация — `Graphics/API/GL/GL46/RHI/`: команды исполняются немедленно;
  `bindPipeline` применяет `RenderState`/`BlendingState`/`MeshRenderState` через
  кеширующие `GL4Renderer::use*` (общее «текущее состояние» с legacy-проходами),
  `glUseProgram`, `glBindVertexArray`; VAO принадлежит PSO и хранит только формат
  (DSA `glVertexArrayAttribFormat`), буферы — `bindVertexBuffer` → `glVertexArrayVertexBuffer`
  (нужен **после** `bindPipeline`). Дескрипторный набор = список `glBindBufferBase`/
  `glBindTextureUnit`, номер набора игнорируется (плоское пространство GL).
- **Единый шейдерный путь**: `SGSLEVulkanizer::Target::OPENGL` — GLSL с
  `layout(binding=N)` без `set`, `gl_VertexID` сохранён; GL 4.6 принимает как есть.
  `GL46ShaderProgram` компилирует его (`#version 460 core` приписывается, если нет) и
  **рефлектит через program interface query** (`GL_UNIFORM_BLOCK`/`GL_UNIFORM`/
  `GL_PROGRAM_INPUT`) — на GL это источник истины, glslang не нужен. Грабли: члены
  именованного блока GL отдаёт как `Block.member` — префикс срезается; массивы — `name[0]`.
- Push constants на GL не реализованы (предупреждение) — фасад заменит мини-UBO;
  `transition` — no-op; `destroyDeferred` — просто сброс ссылки; `waitIdle` = `glFinish`.
- Проверка — `Tests/RHI` (`SGRHITest --gapi gl46`): точные пиксели после readback.
- **Загрузка шейдера в RHI** — `RHIShaderLoader::load(device, "${enginePath}/…/x.sgshader",
  defines)`: `ShaderAnalyzedFile` через `AssetManager` → дефайны бэкенда (`SG_GLSL4` /
  `SG_GLES32` по `DeviceProperties::m_apiType`) + `#attribute` шейдера → вулканизатор
  под диалект → `createShaderProgram` (для explicit API ещё `SPIRVCompiler`). Обращаться
  к юниформам **только через рефлексию** (`findBinding`, `findMember`), имена блоков —
  `SGLegacyUniforms_<stage>`.
- **Первый RHI-проход — `Graphics/RHI/ScreenBlit`** (вывод текстуры на экран, вместо
  legacy-квада `IRenderer::renderTextureOnScreen`): `GL46Renderer` переопределяет
  `renderTextureOnScreen`, инициализирует `ScreenBlit` лениво при первом вызове (нужен
  `AssetManager`), при провале откатывается на legacy с ошибкой в логе. Legacy-текстуры
  в дескрипторный набор передаются как `Ref` с пустым делитером (не владеем).
  `IRenderer::readScreenPixels()` читает backbuffer (GL: `glReadPixels` с FB 0, RGBA8,
  строки снизу вверх). `AttachmentReadback` — в `Graphics/API/AttachmentReadback.h`.
- **`GL46Shader` в RHI-режиме** (`m_useRHIUniforms`, ставит `GL46Renderer::createShader`): код
  стадий проходит вулканизатор (OPENGL, `m_firstBinding = 8` — точки 1–4 у legacy-`IUniformBuffer`,
  которые перекрывают `layout(binding)` через `glUniformBlockBinding`), после линковки —
  `GL46ShaderProgram::reflectProgram`, UBO на каждый `SGLegacyUniforms_<stage>` (нули +
  дефолты из `Report::m_defaults`), `useX("имя")` → `glNamedBufferSubData` по offset/stride
  (`name[i]` — элемент массива, stride из `BlockMember::m_paddedSize`; юниформ в нескольких
  стадиях пишется во все блоки). Сэмплеры (`useTextureBlock`, `useTexture`) — по-прежнему
  `glUniform1i`: `layout(binding)` у сэмплера — лишь начальное значение, glUniform его перекрывает.
  `bind()` = `glUseProgram` + `glBindBufferBase` legacy-блоков. GL4Renderer этот режим не включает.
- **Меши через RHI** (шаг 3): `IVertexBuffer::addAttribute` (7 арг.) теперь не виртуальный —
  записывает `AttributeDesc` и зовёт `addAttributeImpl` (это переопределяют GL/Vk-буферы);
  `IMeshData::m_rhi` (буферы, `VertexInputDesc`, `m_prepared`) сбрасывается в `prepare()`/`destroy()`;
  `GL46Shader::bind()` регистрирует себя в `GL46Renderer::setCurrentLegacyShader`, а
  `renderMeshData` строит PSO из `shader->getRHIProgram()` (`GL46LegacyProgram`, не владеет
  хендлом) + `m_cachedRenderState` + `meshRenderState` + `m_rhi.m_vertexInput`. Слот 0 — `Vertex`
  (stride `sizeof(Vertex)`), слоты 1.. — наборы цветов. Меши без привязанного `GL46Shader`
  (или на GL4) рисуются legacy VAO-путём.
- **Legacy vertex arrays через RHI** (шаг 4): `renderArray`/`renderArrayInstanced` оборачивают
  буферы `IVertexArray` не владеющими `GL46GPUBuffer` (`getNativeHandle()` у `IVertexBuffer`/
  `IIndexBuffer`; кеш `m_wrappedBuffers` по хендлу), слоты — по буферам, отсортированным по хендлу
  (стабильный ключ кеша PSO), `perInstance` — если у атрибута divisor > 0. Данные динамических
  буферов (subData) остаются в тех же GL-объектах — обёртка это не ломает.
- **Фреймбуферы через RHI** (шаг 5): `GL46Renderer::createFrameBuffer` отдаёт `GL46FrameBuffer`;
  его `bind()` = `beginRenderPass` (LOAD ops, draw buffers = последний `bindAttachmentsToDrawIn`),
  `bindAttachmentsToDrawIn` под биндом = re-begin, `clearAttachment` = `clearColorAttachment(индекс
  attachment'а, m_clearColor)` / `clearDepthStencil`, `unbind()` = `endRenderPass` (+ viewport окна).
  `GL46CommandList::beginRenderPass` биндит FBO по `getNativeHandle()`, ставит draw buffers через
  `glNamedFramebufferDrawBuffers`, viewport: явный размер → поля `m_viewport*` фреймбуфера → окно.
  Все GL46-фреймбуферы движка (в т.ч. `LayeredFrameReceiver`) теперь такие; `IFrameBuffer` без
  RHI (GL4) — legacy.
- **Текстуры: юнит-модель проходов оставлена** (решение 2026-08-17). Три паттерна:
  `useTextureBlock(name, unit)` + `texture->bind(unit)`; + `frameBuffer->bindAttachment(type, unit)`;
  + TBO (`u_bonesMatricesUniformBuffer`). Юниты чейнятся через offset'ы хелперов `IShader`
  (`bindMaterialTextures`/`bindTextures`/`bindTextureBindings` возвращают следующий offset).
  **Не смешивать** юнит-модель с биндингом по рефлексии в одном шейдере — юниты столкнутся.
  Для Vulkan — фасад с таблицей юнитов (RHI_DESIGN), не правка проходов.
- Правило миграции: проход перенесён, только когда `SGSmokeTest --gapi gl46 --reference`
  даёт 0 %; legacy-путь остаётся откатом до конца этапа.

## Vulkan-бэкенд (2026-08-17)

Файлы: `Graphics/API/Vulkan/VulkanCommon.h` (`SG_VK_CHECK`), `VulkanTypesCaster` (форматы,
состояния), `RHI/VulkanContext` (instance/surface/device/VMA; `VMA_IMPLEMENTATION` — только в
`VulkanContext.cpp`), `RHI/VulkanDevice` (IDevice + сервисы: `acquireCommandBuffer`,
`submitRaw` → id, `waitForSubmission`, `immediateSubmit`, транзиентные descriptor set'ы,
`createStagingBuffer`, `retire`), `RHI/VulkanSwapchain`, `RHI/VulkanTexture` (image+view+
sampler+layout-трекер), `RHI/VulkanGPUBuffer`, `RHI/VulkanShaderProgram`, `RHI/VulkanPipelineState`,
`RHI/VulkanDescriptorSet`, `RHI/VulkanCommandList`; фасады `VkRenderer`, `VkTexture2D`,
`VkFrameBuffer`, `VkShader`, `VkUniformBuffer`, `VkVertexArray`, `VkVertexBuffer`, `VkIndexBuffer`,
`VkCubemapTexture`. Подробное «почему» — RHI_DESIGN, «Vulkan-бэкенд».

Состояние на 2026-08-18: **Vulkan рисует финальный кадр смоук-сцены** — небо, тела, тени, прозрачная
сфера через стохастику. Против эталона GL46 — **7.138 %** (max diff 77), и это уже не шум: разница в
форме SSAO-затемнения вокруг объектов + дизеринг прозрачной сферы. Причина остатка — шумовая
текстура SSAO (см. «Мелкие грабли»: `ITexture2D::create<float>` не выставляет `m_dataType`); GL
заливает биты float как байты, Vulkan аплоад отклоняет и текстура пустая. Vulkan-кадр детерминирован
от прогона к прогону (0.000 %). Геометрический проход — 0.000 % против GL46 по всем вложениям (3-е —
0.108 %, сам дизеринг). `SGRHITest` — PASS на gl46 и vulkan; `SGSmokeTest --gapi gl46 --reference` —
0.000 %. **Валидация на смоук-сцене: 43 сообщения → 9** (все — «write is unused» из шейдера контура,
см. ниже), ошибок нет вовсе.

**Как искался шум финального кадра (2026-08-18) — три дефекта, спрятанные друг под другом**:
1. Шум порождал `OutlinePass`: проход 3 всегда рисует полноэкранный квад во вложение 7 и там, где
   контура нет, **не присваивал `outColor`** — на GL вложение сохраняло цвет сцены, на Vulkan
   писалось неопределённое значение. Диагностика, которая это вскрыла: принудительно открытый гейт FX
   не менял картинку → пишет кто-то после FX.
2. Под шумом — чёрный кадр: FX-дро на Vulkan **не записывались**. Квады `PostProcessPass` — 6 индексов
   и **ноль вершин** (вершинный шейдер FX синтезирует квад из `gl_VertexID`), а `prepareMeshRHI`
   отвергал меш без вершин. Диагностика: `fragColor = magenta; return;` в начале FX-шейдера — на GL
   маджента, на Vulkan чёрный → дро не доходит; лог ранних выходов `renderMeshData` → «mesh not
   prepared».
3. Под чёрным — белый: `PostProcessPass` задаёт сэмплеры своих вложений через
   `useInteger(name, unit)` (на GL сэмплер — это int-юниформ), а `VkShader::useInteger` знал только
   legacy-блок → сэмплеры `SG_SSAO_*` читали dummy-белую текстуру. Теперь `useInteger` для имени, которое
   рефлексия знает как сэмплер, идёт в таблицу юнитов.
Ни один из трёх не виден по ошибкам валидации: пропущенное дро не валидируется, а неопределённый
выход фрагмента — законен.

Грабли:

- **Флип только в окно.** Offscreen-проходы без флипа viewport'а (память = GL, readback без
  переворота), проходы в swapchain — отрицательная высота viewport'а + инверсия front face
  (`vkCmdSetFrontFace`, dynamic state). `readScreenPixels` возвращает строки снизу вверх.
  Не добавлять флипов в шейдеры/тесты — они совпадут с GL как есть.
- **`PipelineStateDesc::m_renderTargets` не заполняется** — не полагаться на него: VkPipeline
  создаётся лениво на первый draw под форматы активного прохода (`VulkanPassFormats`).
- **Барьеры внутри dynamic rendering запрещены**: все переходы layout'ов — в
  `beginRenderPass`/`endRenderPass`/вне прохода. Offscreen-текстуры «отдыхают» в
  `SHADER_READ_ONLY_OPTIMAL` (после endRenderPass, после аплоада, сразу после
  `createAsFrameBufferAttachment`). Текстура не в этом layout'е при bind внутри прохода → warning
  в лог и мусор/ошибка валидации.
- **`uploadData` внутри прохода** пишется во второй (transfer) командный буфер списка, который
  сабмитится перед основным. Host-visible буферы пишутся сразу memcpy'ем (как GL46).
- **Descriptor set = CPU-таблица**, материализуется на draw'е под layout программы; биндинги, не
  существующие в программе, пропускаются молча. Set index в `bindDescriptorSet` ≤ 3.
- **Пул дескрипторов** должен содержать все типы, что встречаются в шейдерах движка: экранный
  шейдер имеет `samplerBuffer` (UNIFORM_TEXEL_BUFFER) — без него validation warning.
- **Завершение — три отдельные грабли, все дают одно и то же окно «Debug Error! abort() has been
  called» уже ПОСЛЕ `PASS` теста** (падение в статической деструкции, не в рендере; на GL46 их не
  видно, потому что там некому ругаться). Порядок разбора: смотреть `PROBLEMATIC FRAME INFO` в
  выводе `HwExceptionHandler`, там точный кадр.
  1. `IRenderer::shutdown()` (конец `CoreMain::startCycle`) → `VkRenderer::shutdown` →
     `VulkanContext::destroy` освобождает GPU-часть всех ещё живых `VulkanTexture`/`VulkanGPUBuffer`
     (реестр `registerResource`) — иначе `vmaDestroyAllocator` в Debug делает `abort()`, потому что
     `VkTexture2D` в AssetManager переживают рендерер.
  2. **Фасады не должны ходить через `VkRenderer::getInstance()`** — только через
     `VkRenderer::getLiveDevice()` (статический указатель, обнуляемый в `shutdown()`). Деструкторы
     ассетов (`IMaterial` → `AssetRef<ITexture2D>` → `VkTexture2D::destroyOnGPU`) выполняются в
     статической деструкции, когда сам синглтон уже разрушен: `getInstance()` вернёт непустой
     `shared_ptr` на мёртвый объект, и `getVulkanDevice()` прочитает мусор → запись в
     `m_deferredDestroy` разрушенного `VulkanDevice`.
  3. Не-Vulkan соседи по тому же порядку: `AudioDevice::shutdown()` (иначе `~AudioDevice` в
     static-деструкторе зовёт AL-ошибку → `SG_LOG_E` → уже разрушенный логгер) и
     `FontsManager::~FontsManager` (сначала `m_fontsAssetsManager->clear()`, иначе `FT_Done_Face`
     по освобождённой `FT_Library`). Контекст — `shared_ptr`, ресурсы держат его.
- **RHI-объект, который может пережить устройство, обязан держать `shared_ptr<VulkanContext>` и
  регистрироваться в реестре, а не хранить `VulkanDevice&`.** Так сделаны `VulkanTexture`,
  `VulkanGPUBuffer`, `VulkanShaderProgram`, `VulkanPipelineState`. Проверено 2026-08-17:
  `~VulkanShaderProgram` с `m_device.getContext()` падал при выходе, потому что `VkShader` —
  ассет и умирает после рендерера.
- **`ShaderReflection::BlockMember::m_paddedSize` = страйд ОДНОГО элемента массива**, не размер
  всего массива. GL даёт `GL_ARRAY_STRIDE`, а `padded_size` из SPIRV-Reflect — размер всего члена,
  поэтому SPIR-V-путь берёт `array.stride` (иначе `name[i]` уезжает за пределы блока: 2026-08-17
  на смоуке — 280 ошибок «write of 12 bytes at offset 8416 exceeds buffer size 4320»).
- **Readback attachment'а отдаёт строки в порядке render target'а на ВСЕХ бэкендах** (строка 0 =
  NDC y = −1): Vulkan намеренно не флипает viewport в offscreen-проходах, чтобы раскладка памяти
  совпадала с GL. Значит переворот для PNG/сравнения нужен одинаковый для всех API — флип «только
  для GL» давал перевёрнутый кадр на Vulkan (2026-08-17).
- **Аплоад текстуры сайзится по формату изображения**, а не по `m_channelsCount`/`m_dataType`:
  `vkCmdCopyBufferToImage` считает объём по VkFormat. Есть текстуры движка, где эти два не
  сходятся (RGBA8-данные при 8-байтовом формате) — `VkTexture2D::uploadRegion` такой аплоад
  отклоняет с сообщением, а не выдаёт невалидную копию (`VulkanTypesCaster::formatTexelSize`).
- **Общие UBO движка привязываются ПО ИМЕНИ БЛОКА, а не по `setLayoutLocation`.** `CameraData`,
  `ProgramDataBlock`, `SpotLightsBlock`, `AtmosphereBlock` объявлены в шейдерах без явного
  `layout(binding)`, поэтому на Vulkan биндинг им раздаёт вулканизатор, и номера 1–4 из
  `setLayoutLocation` ничего не значат. `VkUniformBuffer::prepare()` регистрируется в
  `RHI/VulkanSharedUniformBuffers` по `m_blockName`, `VkShader::buildDescriptorSet()` ищет там
  каждый отражённый UBO, кроме своих `SGLegacyUniforms_*`. UBO без `m_blockName` — предупреждение.
- **Сами общие UBO создаются в `IRenderer::init()`** (перенесены из `GL4Renderer::init()`
  2026-08-17), там же `IRenderer::prepareUniformBuffers` — это данные движка, не GL. Новый бэкенд
  получает их бесплатно; проверено, что GL46/GL4 после переноса дают смоук 0.000 %.
- **Таблица юнитов** (`RHI/VulkanTextureUnits` + `VkShader::m_samplerUnits`): `useTextureBlock(name, U)`
  пишет половину «сэмплер → юнит», `VkTexture2D::bind(U)` (и `VkFrameBuffer::bindAttachment`,
  который делегирует в неё) — половину «юнит → текстура»; `VkShader::buildDescriptorSet()`
  соединяет их по рефлексии перед draw'ом. Проходы не меняются.
- **Draw'ы обязаны идти в тот же командный список, где открыт проход.** На GL46 команды
  немедленные, поэтому `renderMeshData` работал на отдельном списке; на Vulkan проход живёт внутри
  одного командного буфера, так что `VkRenderer::renderMeshData` пишет в
  `m_frameBufferCommandList` (тот, что открыл `VkFrameBuffer::bind()`) и молча выходит, если тот не
  в состоянии записи.
- **Не переопределять `IMeshData::prepare()` в бэкенде.** GL этого не делает —
  `createMeshData()` возвращает базовый `IMeshData`. Заглушка `VkMeshData` 2023 г. переопределяла
  `prepare()` пустым телом, из-за чего у **каждого** меша не было буферов вершин и Vulkan рисовал
  чёрный кадр при нуле ошибок; класс удалён 2026-08-17.
- **Массивы сэмплеров**: проходы адресуют элементы (`mat_diffuseSamplers[0]`), а рефлексия отдаёт
  одно биндинг-имя без индекса — `VkShader::buildDescriptorSet` перебирает `[i]` до `m_count`.
  **`isUniformExists` обязан снимать индекс перед обращением к рефлексии**: `bindMaterialTextures`
  идёт по `name[i]` и **прерывает цикл** на первом несуществующем имени, поэтому «строгий» ответ
  false молча отвязывает все текстуры материала (2026-08-17).
- **Незаписанный дескриптор = draw молча ничего не рисует.** Это оказалось причиной пустой сцены на
  Vulkan (2026-08-17): проходы движка оставляют часть объявленных шейдером дескрипторов непривязанными
  (на GL сэмплер просто читает юнит 0, а TBO — ничего), и NVIDIA-драйвер отбрасывал такой draw целиком,
  без ошибок валидации в тех же местах. Заполняются заглушками **все** незанятые:
  `VkRenderer::getDummyTexture()` — 1x1 белая картинка для сэмплеров,
  `VkRenderer::getDummyTexelBufferView()` — одноэлементный uniform texel buffer для `samplerBuffer`
  (`u_bonesMatricesUniformBuffer` у неанимированных мешей). Проверять по
  «`set N of '<pso>': K writes, missing: [...]`» — список должен быть пуст.
- **Копирование `SGCore.dll` рядом с тестом молча падает, если висит старый процесс теста** — прогон
  тогда идёт со старой сборкой, и правки «не работают». Перед копированием убивать
  `SGSmokeTest`/`SGRHITest` (потеряно много времени 2026-08-17: диагностика не печаталась именно
  поэтому).
- **`VulkanCommandList::begin()` на уже пишущем списке РАНЬШЕ молча терял работу**: legacy-фасады
  делят один список, а проходы биндят фреймбуфер, не отвязав предыдущий, — `vkBeginCommandBuffer`
  сбрасывает буфер. Теперь `begin()` доводит и **сабмитит** незавершённую работу
  (`flushRecordedWork`). Правило: любой общий список нельзя перезапускать без submit'а.
- **SPIR-V-рефлексия обязана разворачивать вложенные структуры в листовые имена** (`objectTransform.modelMatrix`):
  GL перечисляет `GL_UNIFORM`-ресурсы, а это всегда листья, а SPIRV-Reflect отдаёт член верхнего уровня
  со своими вложенными `members`. Без разворачивания любая запись во вложенный uniform молча промахивается —
  а движок так передаёт данные объекта (`useMatrix("objectTransform.modelMatrix", …)`). Смещения листьев берутся
  из `absolute_offset` (относительно блока), не из `offset` (относительно родителя). Исправлено 2026-08-17,
  покрыто срезом в `SGRHITest` (шейдер со `struct SGTestTransform`).
- **Очистка адресует draw-буферы прохода, а не слоты attachment'ов** (`glClearBufferfv` на GL,
  `vkCmdClearAttachments` на Vulkan): без `bindAttachmentsToDrawIn` перед `clear()` на GL46 очистка
  просто не происходит (проверено 2026-08-17 — тест поймал это на GL46, не на Vulkan, где пустой
  draw-набор трактуется как «все attachment'ы»). Порядок в проходах движка: bind →
  bindAttachmentsToDrawIn → clear.
- **Последовательность кадра на фасадах покрыта тестом и НЕ является причиной пустой сцены**:
  срез `runFacadeSequenceSlice` в `SGRHITest` повторяет форму кадра (2 фреймбуфера, 8 и 4
  attachment'а, перекрывающиеся bind'ы, очистка, повторный bind с draw-набором) — очистки доживают
  до readback на обоих бэкендах. Значит искать надо в самих draw'ах, а не в обвязке проходов.
- **Юниформы, различающиеся по draw'ам, обязаны сниматься по draw'у.** На GL `glUniform*` попадает
  в программу немедленно, а здесь draw только **записывается** и исполняется позже, поэтому один
  буфер, перезаписываемый на месте, даёт всем draw'ам прохода значения **последнего** из них: сцена
  схлопывается на трансформ и материал последнего объекта (симптом 2026-08-18 — в G-буфере оставался
  только пол, а прозрачная сфера выживала лишь потому, что в своём проходе была одна). Решение:
  `VkShader` держит блок в CPU-копии (`LegacyBlock::m_values`), а `buildDescriptorSet()` (вызывается
  один раз на draw) копирует его в срез арены устройства — `VulkanDevice::allocateTransientUniforms()`
  — и биндит этот offset. Арена: bump-аллокатор по чанкам, регионы ротируются в `prepareFrame()`
  (`rotateUniformArena()`), поэтому регион переписывается только через несколько кадров. Привязки
  дескрипторов снимать отдельно не нужно: `VulkanCommandList` материализует transient `VkDescriptorSet`
  на записи команды по версии набора, а буферы попадают в `m_keepAlive` сабмита.
- **`isUniformExists` на Vulkan НЕ повторяет GL для объявленного, но неиспользуемого ресурса.** GL-линкер
  выбрасывает такой юниформ (`glGetUniformLocation` → −1), а в SPIR-V ресурс с явным `layout(binding)`
  остаётся, и **оба** сигнала SPIRV-Reflect врут в эту сторону: `SpvReflectDescriptorBinding::accessed`
  и `SpvReflectEntryPoint::used_uniforms` считают его используемым (проверено 2026-08-18 на
  `mat_skyboxSamplers`, чьё единственное обращение закомментировано; SPIR-V 1.6 перечисляет все
  глобальные переменные в интерфейсе `OpEntryPoint`). Вывести GL-семантику из SPIR-V этим способом
  нельзя — не тратить на это время. Следствие для проходов: `bindMaterialTextures` считает
  `<name>_CURRENT_COUNT` по числу привязанных текстур, и на Vulkan он выходит больше, чем на GL, если
  шейдер объявил сэмплер «на будущее». Шейдеры не должны зависеть от того, что линкер что-то выбросит.
- **`IShader::bindTextures` замораживал имена юниформов в function-local `static`** (исправлено
  2026-08-18): `static const std::string textureUniform = ...(slotType)` вычисляется на первом вызове,
  после чего любой слот пишется под именем первого. Кого затрёт — зависит от порядка первого вызова,
  а он у бэкендов разный, поэтому баг проявляется несимметрично.
- **Attachment'ы фреймбуфера создаются с `GL_NEAREST` на GL** (`GL4Texture2D.cpp`), и это не косметика:
  проходы сэмплируют их тексель в тексель и сравнивают результат **точным `==`** (гейт слоя в
  `layer_default_fx.glsl`: `texture(SGPP_LayersVolumes, uv).rgb == calculatePPLayerVolume(...)`).
  С линейной фильтрацией интерполированный сэмпл не равен хранимому текселю почти нигде. На Vulkan
  вложения создавались с `VK_FILTER_LINEAR` — исправлено 2026-08-18 на `VK_FILTER_NEAREST`.
- **Свежий `VkImage` содержит неопределённые данные, а GL-текстура без данных читается нулями.**
  Движок на это опирается: все проходы открываются с `LoadOp::SGG_LOAD`, а attachment'ы
  FX-фреймбуфера не очищаются вообще. `VkTexture2D::createAsFrameBufferAttachment` теперь очищает
  образ при создании (цвет — в нули, глубина — в 1.0). Это **не** было причиной шума в финальном
  кадре, но закрывает реальный разрыв семантики.
- **Attachment, привязанный как сэмплер, пока проход в него же рисует.** Проходы биндят *все*
  attachment'ы фреймбуфера как текстуры заранее и только потом выбирают цель отрисовки
  (`PostProcessPass` так и делает), поэтому дескриптор указывает на изображение, которое сейчас —
  render target. На GL это UB, на Vulkan — ошибка валидации, а не записать дескриптор нельзя (draw
  отбросится). `VulkanCommandList::materializeSet` подменяет такой дескриптор dummy-текстурой
  (`isPassAttachment`). Ни один шейдер движка не читает то, что пишет; если появится такой — прочитает
  белое, искать здесь. Dummy живёт в `VulkanDevice::getDummyTexture()` (нужен и командному списку, и
  `VkShader`), `VkRenderer::getDummyTexture()` — делегат.
- **Вершинный layout меша ≠ то, что читает программа.** Меш несёт все атрибуты движка, а конкретный
  шейдер читает часть (контур — позиции и кости, постпроцесс — вообще ничего). Объявлять непотребляемый
  атрибут законно, но валидация ругается на каждый пайплайн, поэтому `VulkanPipelineState` пересекает
  раскладку меша с `getReflection().m_vertexInputs` программы (программа входит в ключ кеша PSO).
- **Текстура с отклонённым аплоадом обязана получить определённое содержимое и layout**: проходы всё
  равно её биндят и сэмплируют, а образ остаётся в `UNDEFINED` — это ошибка валидации поверх уже
  сообщённой проблемы аплоада. `VkTexture2D::uploadRegion` при отказе очищает образ и переводит в
  `SHADER_READ_ONLY`.
- **Незаписанный `out` ругается только если attachment под этой location привязан.** Обратное тоже
  верно: запись в location, под которой attachment'а нет, даёт «this write is unused» — безобидно, но
  шумно. Шейдер контура мультиплексирует три прохода через одну программу, и в пайплайне прохода 3
  привязан один attachment, поэтому 9 таких предупреждений остаются и убираются только разделением
  шейдера (статический анализ не смотрит на ветку по `vs_pass`).
- **Меш только с индексами — законный случай**: постпроцесс-квады (`PostProcessPass`) держат 6 индексов
  и ноль вершин, вершинный шейдер FX строит квад из `gl_VertexID`/`gl_VertexIndex`
  (`primitives.glsl`). На Vulkan такой меш готовится без вершинного буфера и с пустым vertex input
  (нулевые счётчики в `VkPipelineVertexInputStateCreateInfo` валидны), рисуется `drawIndexed`.
  Квад `OutlinePass` при этом с вершинами — не путать.
- **`useInteger` на имя сэмплера = выбор юнита**, как `glUniform1i` на GL. Проходы задают сэмплеры обоими
  способами (`useTextureBlock` и `useInteger` — второй в `PostProcessPass` для вложений эффекта), и
  фасад Vulkan обязан принимать оба. Проверять по рефлексии (`isSamplerBinding`), а не по имени.
- **Незаписанный выход фрагмента — неопределённое значение, и бэкенды расходятся именно на нём.**
  `calculateStochasticTransparencyComponents` присваивала `outputSTColor`/`outputLayerColor` не на
  всех путях, а вызывающие не делают `discard` (он закомментирован). GL оставлял attachment как есть,
  Vulkan писал туда цвет фрагмента: прозрачные объекты протекали в непрозрачный слой (вложение 1), а
  весь стохастический слой (вложение 3) заполнялся непрозрачной геометрией, из чего композит делал
  шум. Ни один бэкенд не был неправ — шейдер полагался на UB. Правило: **каждый `out` шейдера должен
  быть присвоен на каждом пути**; чтобы «не трогать» attachment при включённом `SrcAlpha`-блендинге,
  пишется alpha 0, а не отсутствие записи. Исправлено 2026-08-18: вложение 3 — 100 % → 0.108 %.
- **Небо пропадало из-за неинициализированной альфы в шейдере скайбокса** (исправлено 2026-08-18):
  ветка `if(mat_skyboxSamplers_CURRENT_COUNT > 0)` задавала только `skyboxCol.rgb`, оставляя alpha от
  `vec4(0.0)`. На GL ветка не выбиралась (см. выше), на Vulkan — выбиралась, и фрагмент с alpha 0
  исчезал. Обе ветки и так дают одинаковый цвет (сэмплирование кубмапы закомментировано).
- **Диагностика через сравнение attachment'ов работает только после того, как readback честен**:
  до 2026-08-18 RGBA16F-вложения читались как мусор (см. «Attachment'ы и readback»), и сравнение
  Vulkan/GL46 по ним давало ложные выводы. Сначала проверять, что читаемый формат декодирован верно.
- **Путь legacy-фасада фреймбуфера покрыт тестом**: срез в `SGRHITest` делает `bind` → `clearAttachment`
  → `unbind` → `readAttachmentPixels` и требует точный цвет. Проходит на gl46 и vulkan — то есть сам
  фасад и сабмит его прохода исправны; если сцена пустая, причина в другом.
- **Как отлаживать «рисуется, но кадра нет»** (потрачено много времени 2026-08-17): смоук умеет
  `--geometry-pass` и `--attachment N` — снимать промежуточный буфер, а не финальный. Полезная
  цепочка проверок: draw'ы доходят до `vkCmdDraw*` → в проходе с нужным числа color-attachment'ов →
  пишут в тот же `VkImage`, что читает readback. **Осторожно с лимитами в одноразовых логах**:
  проходы теней идут раньше и съедают счётчик, из-за чего кажется, что основной проход не рисует.
- **Loader — DLL** (`vulkan-1.dll` из vcpkg bin или системный) — не трогать Vulkan в статических
  деструкторах.
- **Отладочный инструментарий Vulkan**: слои валидации (в Debug автоматически, принудительно —
  `SG_VK_VALIDATION=1/0`, работает и в Release); имена объектов через `VK_EXT_debug_utils`
  (`VulkanContext::setObjectName` — изображения, буферы, модули, пайплайны); **метки команд**
  (`beginDebugLabel`/`endDebugLabel`/`insertDebugLabel`) — каждый render pass открывает именованный
  регион («framebuffer pass 1920x1080, colors 8, depth»), каждый draw помечается именем PSO, поэтому
  захват RenderDoc/NSight читается по проходам; `SG_VK_CHECK` логирует выражение, `VkResult` и место.
- Шум в логе `[Vulkan validation] loader_get_json … Bandicam/EOSOverlay`, `Removing layer
  VK_LAYER_OBS_HOOK` — сторонние implicit-слои системы, не ошибки движка.
- Тест запускать из корня репозитория: `${enginePath}` = `./Resources`, иначе экранный шейдер
  не грузится и screen blit проваливается.

## DX12-бэкенд (этап 3, начат 2026-08-18; RHI — 2026-08-20)

Файлы: `Graphics/API/DX12/DX12Common.{h,cpp}` (`SG_DX_CHECK`, расшифровка `HRESULT`),
`DX12TypesCaster` (форматы, состояния, топологии), `DX12Renderer.{h,cpp}`;
`DX12/RHI/`: `DX12Context` (фабрика, адаптер, device, direct-очередь, слой отладки, CPU-кучи
RTV/DSV), `DX12DescriptorHeap` (free-list + кольцо), `DX12Device` (IDevice + сервисы),
`DX12Swapchain`, `DX12Texture`, `DX12GPUBuffer`, `DX12CommandList`, `DX12DescriptorSet`,
`DX12PipelineState`, `DX12ShaderProgram`. Весь код под `#if defined(_WIN32)`.

Состояние 2026-08-22: **все три API проходят смоук против одного эталона** `smoke_gl_1920x1080.png`
с настройками по умолчанию — gl46 и gl4 0.000 %, DX12 0.000 % (max channel diff 2 на весь кадр),
Vulkan 0.096 % (max 52, весь остаток во вложении 3 — стохастическая прозрачность). У DX12 было
54.303 %, у обоих explicit-бэкендов 46.17 % до правки SSAO (см. ниже).
Геометрические вложения 1/4/5/6 у DX12 — 0.000 % против GL46. `SGRHITest --gapi dx12` — PASS, gl46 и vulkan без
регрессий, GL46 против эталона — 0.000 %. Ошибок слоя отладки нет, только предупреждения.
⚠️ **Флаги смоука читаются наоборот**: `--threshold` — допуск **на канал** (по умолчанию 8),
`--max-diff` — **доля** различающихся пикселей (по умолчанию 0.01). Перепутанные местами, они дают
ложные «0.000 % расхождений» и весь замер становится бессмысленным (поймано 2026-08-22).
Не сделано: три шейдера с геометрической стадией (`pbr/batching`, `pbr/terrain`,
`shadows_generator/batching`) не транслируются spirv-cross'ом — на смоук-сцену не влияют.
⚠️ **Зависание при выходе — не только у DX12** (поймано и на Vulkan 2026-08-22). Сигнатура: процесс
с `HasExited = True`, но живым объектом; `taskkill` отвечает «no running instance», а файл держится.
Он блокирует свои же `SGSmokeTest.exe` и `SGCore.dll`, из-за чего следующая сборка либо не линкуется,
либо **молча копирует старую DLL** и даёт ABI-мешанину («Debug Error! abort() has been called» на
ровном месте). Лечение без перезагрузки: переименовать залоченные файлы (Windows это разрешает) и
положить рядом свежие. Запускать под таймаутом и проверять `Copy-Item` на ошибку, а не вслепую.

**Флип вьюпорта у D3D — ЗЕРКАЛО правила Vulkan, а не копия** (стоило 54 % расхождения, 2026-08-22).
Vulkan клипует с +y вниз, поэтому его NDC y = −1 попадает в строку 0 ровно как у GL — offscreen
флипать не надо, флипается только окно. D3D клипует с +y **вверх**, но строка 0 у него верхняя,
значит в строку 0 попадает NDC y = **+1** — противоположно GL. Поэтому на DX12 флипать надо
**offscreen** (чтобы раскладка памяти совпадала с GL: на неё опираются и readback, и каждый uv,
который движок считает руками), а окно — **нет** (свопчейн и так презентует строку 0 сверху).
Правило скопировали у Vulkan дословно, и **все** offscreen-буферы выходили перевёрнутыми. Следствия:
- Симптом был не «картинка вверх ногами», а «SSAO не сходится»: FX-проходы читают источник тем же
  `vs_UVAttribute`, которым и пишут, поэтому зеркало у passthrough-прохода **самосокращается** —
  финальный кадр выглядел правильным. Ломается только тот, кто мешает экранный uv с координатой,
  посчитанной отдельно: SSAO проецирует `camera.projectionMatrix` вручную и получает uv в GL-конвенции.
- **Winding идёт следом**: флип вьюпорта инвертирует обход, и `FrontCounterClockwise` пришлось
  инвертировать вместе с ним. Знак **замерен пробником** (`fragColor = gl_FrontFacing ? ... ` →
  сравнение с GL46 дало 48.7 % расхождения), а не выведен: рассуждение «у D3D screen space y вниз,
  значит …» легко даёт неверный знак в обе стороны. Ключ варианта PSO переименован
  `m_windowFlipped` → `m_viewportFlipped`.
- **Пересчёт прямоугольника ≠ зеркало.** Вьюпорт и scissor приходят по-GL от нижнего края, а D3D
  ждёт от верхнего — этот пересчёт (`TopLeftY = H - y - height`) нужен **всем** проходам, включая
  окно; зеркало (`Height = -height`) — только offscreen. Их слияние в одну ветку убивает screen blit:
  он уезжает наверх окна и `readScreenPixels` читает нули (ровно так падал срез `SGRHITest`).
- Диагностическая цепочка, которая это вскрыла (повторять в таком порядке): пробник в шейдере пишет
  промежуточное значение во вложение → сравнение GL46/DX12 по `--attachment N`. `randomVec` (шум) —
  62 % → шум по фиксированному uv — 0.000 % (значит текстура и сэмплинг целы) → `finalUV` — 60 %
  зеркально (нашли флип) → `getNormal` — 54 % → `gl_FrontFacing` — 48.7 % (нашли winding).
- ⚠️ **«Геометрический проход = 0.000 %» из прошлых замеров было ложным**: сравнивалось вложение 0
  геометрического буфера, а оно **полностью чёрное** на обоих бэкендах. Сверять содержательные
  вложения (1 — цвет, 4 — мировая позиция, 5/6 — нормали) и глазами смотреть на PNG, а не только на
  проценты.

**Дамп сгенерированного HLSL**: `SG_DX_DUMP_HLSL=1` → `DX12HLSLOutputDebug/<shader>_<stage>.hlsl`
рядом с рабочим каталогом. Единственный способ увидеть, что реально отдал spirv-cross: packoffset'ы
блоков (они честно проставляются, HLSL-упаковка std140 не ломает), номера регистров, имена
семантик вершинных входов (`TEXCOORD<location>`), `mul(vec, mat)` с `row_major`.

Более раннее состояние: **`SGRHITest --gapi dx12` — PASS, выход 0, ноль сообщений слоя отладки.**
Срез рисует треугольник в окно через полный путь: GLSL → вулканизатор → SPIR-V (glslang) → HLSL
(spirv-cross) → байткод (`d3dcompiler`, SM 5.1) → PSO + root signature из рефлексии; юниформы
legacy-блока доезжают через CBV-таблицу (центр `(0,127,0,255)` = цвет вершин × tint), фон —
цвет очистки. Соответствие концепций Vulkan↔DX12 — в `docs/RHI_DESIGN.md`, раздел «DX12-бэкенд».
Не сделано: SRV текстур и sampler-таблицы, offscreen-проходы (нужны legacy-фасады, 3.5), root
constants, флип viewport'а для окна (решать в 3.5, когда кадр станет видимым).

**Фасады (3.5, 2026-08-20)**: `DX12Texture2D`, `DX12CubemapTexture`, `DX12FrameBuffer`, `DX12Shader`
плюс фабрики и пути отрисовки в `DX12Renderer`. **Движок стартует на DX12 и досчитывает смоук до
60-го кадра** (`SGSmokeTest --gapi dx12`, выход 0), но шейдеры корпуса не компилируются — см. ниже.
⚠️ **Общее вынесено в `Graphics/RHI/`, не дублировать в бэкенде**: `PixelConversion`, `TextureUnits`,
`SharedUniformBuffers`, `LiveDevice`, `RHIVertexBuffer`/`RHIIndexBuffer`/`RHIVertexArray`/
`RHIUniformBuffer`, `RHILegacyShader`, `RHILegacyDraw`. Бэкенд добавляет только хуки
(`allocateTransientUniforms`, `getDummyBackendTexture`, `setBackendTexture`, `setAsCurrentShader`,
`fillBackendDescriptors`). Vulkan переведён на них — смоук не изменился (46.171 %), GL46 — 0.000 %.

**Шейдерный путь (3.4, готов 2026-08-20)**: `DX12ShaderCompiler` — `spirv_cross::CompilerHLSL`
(`shader_model = 60`) → **DXC** (`IDxcCompiler3`, цели `vs_6_0`/`ps_6_0`/…). Проверенные факты:
- **DXC не требует пакета vcpkg**: `dxcapi.h` + `dxcompiler.lib` есть в Windows SDK 10.0.26100;
  линкуется просто `dxcompiler` рядом с `d3d12 dxgi dxguid`. Из vcpkg нужен только `spirv-cross`;
  ⚠️ `find_package(spirv_cross_hlsl)` **молча ставит `_FOUND FALSE`** без `spirv_cross_glsl` — искать
  и линковать оба.
- ⚠️ **`dxcompiler.dll` класть рядом с exe** (как `SGCore.dll`): иначе через PATH подхватывается
  `dxcompiler.dll` из Vulkan SDK — другой билд, другой результат. `dxil.dll` **не нужен**: DXC из SDK
  подписывает DXIL внутри себя (замерено: с `-Vd` дайджест нулевой и рантайм отвергает пайплайн с
  `E_INVALIDARG`, без `-Vd` — принимает). Никогда не передавать `-Vd`.
- **FXC (`d3dcompiler`, SM 5.1) корпус не тянет**: `error X3511: unable to unroll loop` в
  `volumetric_fog` и `terrain`. `[loop]` spirv-cross ставит только из loop control `DontUnroll`
  в SPIR-V, опции для этого нет — поэтому SM 6/DXC, а не «обойдёмся SDK-компилятором».
- ⚠️ **Регистры HLSL назначает движок**, `DX12ShaderProgram::assignRegisters` → счётчики b/t/u/s на
  каждый space, каждый ресурс занимает `m_count` регистров → `add_hlsl_resource_binding` на каждую
  стадию, и **та же карта** идёт в root signature. Иначе «SRV binding ranges overlap»: spirv-cross
  печатает ровно тот номер, что дали, а массив сэмплеров занимает N регистров подряд.
- **Глубина**: `commonOptions.vertex.fixup_clipspace = true` переводит GL `[-w; w]` в D3D `[0; w]`
  прямо в HLSL; проекции движка не трогаем, `m_depthZeroToOne` остаётся false.
- ⚠️ **Геометрические шейдеры HLSL-бэкенд spirv-cross не умеет** — «Unsupported execution model»
  (поддержаны vertex/fragment/compute/mesh/task). В корпусе их три: `pbr/batching`, `pbr/terrain`,
  `shadows_generator/batching`. Нужен либо отказ от GS в этих проходах, либо отдельный HLSL-исходник.
- Вершинные входы HLSL называются `TEXCOORD<location>`, выходы — `SV_Target<n>`.
- **Флип окна — как на Vulkan**: viewport окна с отрицательной высотой (`TopLeftY = H - y`,
  `Height = -h`), offscreen без флипа; инверсия winding'а при этом не динамическая, поэтому
  `m_windowFlipped` входит в ключ варианта PSO. Проверено: без флипа screen blit уезжает наверх окна
  и readback читает нули.

**Грабли, стоившие времени 2026-08-20:**

- ⚠️ **`Present` не покрывается фенсом, который засигналили ДО него.** Симптом: тест печатает
  `PASS`, а потом процесс падает с `0xC0000409` в `~DX12Swapchain` на `m_swapchain.Reset()`, и
  **ни одного сообщения** слоя отладки при этом нет. Причина: `waitIdle()` ждёт значение фенса,
  засигналенное перед `Present`, то есть сам презент остаётся «в полёте», и свопчейн разрушается
  под ним. Лечение: после `Present` делать ещё один пустой `submitRaw({ })` и запоминать его id как
  `m_lastSubmissions[buffer]` — тогда «дождаться этого буфера» действительно означает «презент
  завершён». Проверять бисекцией: остановить срез до readback / до present — падает только версия
  с презентом.
- **Закрытый командный список держит ссылки на всё, что записал**, и фенс их не отпускает. Пока
  список не сброшен (`allocator->Reset()` + `list->Reset()`), бэкбуферы свопчейна считаются
  занятыми, и `ResizeBuffers`/разрушение свопчейна на это ругаются. `DX12Device::retire()` теперь
  сбрасывает список сразу после фенса.
- **Не ставить `SetBreakOnSeverity`**: у слоя отладки нет колбэка по умолчанию, поэтому брейк без
  подключённого отладчика = `RaiseException`, который никто не ловит, — процесс умирает внутри COM-
  вызова, а сообщение остаётся неслитым в очереди. Вместо этого: **колбэк**
  `ID3D12InfoQueue1::RegisterMessageCallback` (Windows 10 2004+) — сообщения логируются в момент
  появления, как у `VK_EXT_debug_utils`.
- **У DXGI своя очередь сообщений** (`IDXGIInfoQueue` через `DXGIGetDebugInterface1`): всё про
  фабрику и свопчейн идёт туда и никогда не попадает в очередь D3D12. Сливается тем же
  `drainDebugMessages()`.
- **Буферам трекер состояний не нужен**: D3D12 сам продвигает буфер из `COMMON` в нужное состояние
  и возвращает обратно — но **возврат происходит только в конце `ExecuteCommandLists`**. Список
  аплоадов сабмитится вместе с основным, поэтому после `CopyBufferRegion` буфер остаётся в
  `COPY_DEST`, и использование его как вершинного в том же сабмите — ошибка валидации
  («Resource state (0x400: COPY_DEST) … is invalid for use as a vertex buffer»). `uploadData`
  ставит переход в `GENERIC_READ` сразу после копии.
- **`ID3D12Resource` держит device живым через COM**, поэтому у DX12 нет проблемы Vulkan с
  освобождением GPU-части в статической деструкции (реестр ресурсов не нужен). Но слоты
  дескрипторов принадлежат кучам `DX12Context`, поэтому текстура держит `shared_ptr<DX12Context>`.
- **Глубина [0; 1]**: аналога `VK_EXT_depth_clip_control` у D3D12 нет, `m_depthZeroToOne = true` —
  единственное расхождение свойств устройства с Vulkan-бэкендом.
- Двигать `SGCore.dll` к тесту и убивать старые процессы теста — то же правило, что и на Vulkan.

- **Зависимостей vcpkg не нужно для device/свопчейна**: `d3d12`, `dxgi`, `dxguid` идут из Windows SDK,
  подключены веткой `if(SG_TARGET_OS_WINDOWS)` в `Sources/SGCore/CMakeLists.txt`. Пакеты понадобятся
  только шейдерному пути (задача 3.4): `spirv-cross` (SPIR-V → HLSL) и `directx-dxc` (HLSL → DXIL).
- **Слой отладки включается ДО создания device** (`D3D12GetDebugInterface` + `EnableDebugLayer`),
  иначе device создастся без него. Требует установленного компонента Windows «Graphics Tools» —
  если его нет, это не фатально, но валидации не будет (в лог идёт предупреждение).
  Переключатель `SG_DX_VALIDATION=1/0`, двойник `SG_VK_VALIDATION`.
- **Сообщения слоя отладки**: колбэк `ID3D12InfoQueue1` (см. выше) плюс слив очередей D3D12 и DXGI
  в `DX12Context::drainDebugMessages()` — вызывается из `checkForErrors()`, из `submitRaw` и при
  разрушении контекста.
- **Выбор адаптера**: `EnumAdapterByGpuPreference(HIGH_PERFORMANCE)` — на гибридных машинах это даёт
  дискретную GPU; адаптеры с флагом `SOFTWARE` (WARP) отбрасываются, чтобы программный растеризатор
  не съедал кадр незаметно; требуется FL 12.0 (решение зафиксировано в RHI_DESIGN).
- ⚠️ **DX12 стоял ПЕРВЫМ в дефолтном списке `GAPISelector` на Windows** и «пропускался», только пока
  `createRenderer` отдавал `nullptr`. Как только бэкенд начинает создаваться, он становится дефолтом
  и отдаёт пустой кадр. Убран из списка до готовности — доступ через `SG_GAPI=dx12`, как у Vulkan.
- **`confirmSupport()` каждый бэкенд вызывает сам из `init()`** (так делают `GL4Renderer` и
  `VkRenderer`); `CoreMain` его не зовёт.
- **`CoreMain::init()` не останавливался при провале рендерера** — `setShouldClose(true)` влияет
  только на главный цикл, а инициализация шла дальше создавать ассеты и падала с `abort()` на первом
  `nullptr` из фабрики. Исправлено 2026-08-18: после `m_renderer->init()` проверяется
  `m_window.shouldClose()`. Это лечит и латентный тот же путь у GL4.

## Батчинг (Render/Batching, приведён в рабочее состояние 2026-08-22)

- **Батч рисуется инстансингом, а не геометрической стадией**: `renderArrayInstanced(vao, state, 3, 0,
  trianglesCount)` — три вершины на инстанс, один инстанс на треугольник; per-triangle данные
  (`BatchTriangle`) идут тем же вершинным буфером, но со всеми атрибутами при `divisor = 1`, а угол
  треугольника берётся из `gl_VertexID`. Сам буфер не изменился. Так убрана единственная в корпусе
  геометрическая стадия (её не умеет транслировать в HLSL spirv-cross), и заодно снята медленная
  стадия на GL и Vulkan.
- ⚠️ **Ничего из батчинга не вызывается движком**: `Batch::insertEntities` не имеет вызывающих в
  репозитории, `Terrain`-компонент никто не создаёт. Единственное покрытие — `SGSmokeTest --batching`
  (свой эталон `smoke_gl_batching_1920x1080.png`, существующий не трогает). Любая правка здесь без
  этого флага непроверяема.
- **Для отрисовки батча нужны ДВА компонента**: `PBRRPBatchingPass` ходит по `view<Batch,
  EnableBatchingPass>`. С одним только `Batch` батч исправно строится, заливается в буферы — и молча
  не рисуется.
- **Меши батча надо лишать `EnableMeshPass`**, иначе они рисуются ещё и обычным проходом.
- Грабли, стоившие времени 2026-08-22 (все были в коде, ни одна не в новом инстансинге):
  - `PBRRPBatchingPass` грузил `StandardTerrainShader`, а `PBRRPTerrainsPass` — `BatchingShader`.
    Копипаста в `create()`; проход батчинга рисовал точки тесселяционным шейдером. **Пробник в шейдере
    бессмыслен, пока не проверено, что проход грузит именно этот шейдер.**
  - `Batch::bind` падал с `abort()` на `m_atlas.getTexture()->bind(4)`, когда в атлас ничего не
    упаковано, — а батч из нетекстурированных мешей это обычный случай.
  - Фрагментная стадия `pbr/batching.glsl` обнуляла `diffuseColor` и делала `discard` по alpha
    **безусловно**, поэтому нетекстурированный батч не давал ни одного фрагмента. Теперь цвет по
    умолчанию берётся из материала инстанса, а alpha-cutout работает только там, где текстура
    действительно сэмплирована.
  - Индексный texel-буфер создавался `SGG_RGB32_UNSIGNED_INT`, а шейдеры читают его через
    `isamplerBuffer`. GL переинтерпретирует молча, Vulkan/DX12 — нет.
  - Атрибуты `uvOffsets` объявлялись `SGG_INT` при `uvec4` в шейдере, и их регистрировалось
    `texture_types_count / 2` = 11 вместо нужных 12: `texture_types_count` = **23**, нечётный, и
    последний полувектор оставался ненакормленным (хвост ложится в `BatchTriangle::padding0` —
    именно для этого паддинг там и есть).
  - `GPUDeviceInfo::getMaxTextureBufferSize()` возвращал **0** на Vulkan и DX12 (ветки `break` без
    реализации), из-за чего `hasSpaceForEntity` отвергал первый же меш с «batch ran out of space at
    entity 0». Лимит добавлен в `DeviceProperties::m_maxTexelBufferElements`.
- **`SGTextureType::SG_TEXTURE_BUFFER` — настоящий texel buffer на всех бэкендах** (сделано
  2026-08-22, до этого тип понимал только GL). Vulkan: `VulkanGPUBuffer::createTexelView` даёт
  `VkBufferView`, бит `UNIFORM_TEXEL_BUFFER` приезжает вместе с `SGG_STORAGE_BUFFER`. DX12:
  `DX12GPUBuffer::setTexelFormat` + типизированный buffer SRV. Фасады текстур при этом типе создают
  host-visible буфер вместо образа и кладут в `TextureUnits` именно его, а `setBackendTexture` у
  обоих дескрипторных наборов больше не кастует слепо в текстуру — сначала пробует буфер.
- ⚠️ **Формат texel-буфера нельзя расширять RGB→RGBA.** Для образов расширение правильное
  (3-канальные образы почти нигде не поддержаны), но данные texture buffer лежат плотно, и
  расширенный формат читает каждый тексель с шагом 16 байт вместо 12 — геометрия приезжает
  «нарезанной», прогрессивно уезжая от начала буфера. Отдельные кастеры:
  `VulkanTypesCaster::sggInternalFormatToVkTexel`, `DX12TypesCaster::sggInternalFormatToDXGITexel`.
  В DXGI трёхканальными бывают только 32-битные форматы — для остальных эквивалента нет вовсе.
- ⚠️ **`UNIFORM_TEXEL_BUFFER` должен участвовать в join-цикле таблицы юнитов** в
  `RHILegacyShader::buildDescriptorSet`. Пока его там не было, любой `samplerBuffer` на explicit-
  бэкендах получал только заглушечный view из `fillBackendDescriptors` и читал пустышку — включая
  буфер матриц костей. Заглушка остаётся фолбэком, но белую 1x1 картинку в texel-дескриптор писать
  нельзя: это разные виды дескрипторов.
- **Валидация Vulkan — самый быстрый способ найти такие расхождения**: она назвала и знаковость
  индексов, и несовпадение форматов атрибутов, и отсутствующий location 12, и превышение лимита
  ширины образа. На GL всё это молча работает «как-то».

## Инстансинг (Render/Instancing, впервые запущен 2026-08-22)

- **Инстансингу не нужен свой шейдер**: `PBRRPInstancingPass` берёт `StandardMeshShader` и компилирует
  его с дефайном `SG_INSTANCED_RENDERING` через `loadAssetWithAlias` + `addDefine` + `recompile()`;
  дефайн сдвигает локации вершинных атрибутов (см. `impl/glsl4/vertex_attributes_layout.glsl` — это
  таблица локаций, а не шейдер, несмотря на прежнее имя `instancing.glsl`). Отдельный `.sgshader` под
  вариант — лишняя сущность; так и появился мёртвый `pbr/instancing.sgshader` (удалён).
- ⚠️ **Ничего из инстансинга движок не включает**: `AutoInstancing::addEntity` не вызывается нигде.
  Единственное покрытие — `SGSmokeTest --instancing` (свой эталон `smoke_gl_instancing_1920x1080.png`).
- **Нужны ДВА компонента**, как и у батчинга: проход ходит по
  `view<EntityBaseInfo, Instancing, EnableInstancingPass>`. `addEntity` теперь ставит второй сам.
- ⚠️ **Раскладка вершинных атрибутов — свойство пары «буфер + вершинный массив», а не буфера.**
  Буферы меша живут сразу в двух массивах: своём (локации с 0) и в массиве `Instancing` (те же буферы
  за per-instance трансформами, локации с 7). У GL это хранит VAO, поэтому там ничего не замечали;
  explicit-бэкенды пересобирают `VertexInputDesc` на каждый дро из записанного списка. Раскладка пары
  хранится в `IVertexArray::setBufferAttributes/getBufferAttributes`, её спрашивают `RHILegacyDraw` и
  `GL46Renderer::drawLegacyArrayThroughRHI` **раньше**, чем `IVertexBuffer::getAttributes()`.
- ⚠️ **`addAttribute` только добавляет, `useAttributes()` заливает в VAO ВЕСЬ накопленный список.**
  Поэтому повторная привязка тех же буферов в другой массив без `clearAttributes()` записывала туда
  обе раскладки, и локации 0..6 (трансформы инстансов) затирались вершинными данными — геометрия
  выходила рваной одинаково на gl4 и gl46.
- ⚠️ **`IMeshData::bindBuffersToVertexArray` обязана регистрировать буферы в том массиве, куда
  привязывает** (`addVertexBuffer` + `setIndexBuffer`). GL записывает привязки в VAO, а explicit-путь
  читает `IVertexArray::getVertexBuffers()/getIndexBuffer()`: без регистрации массив инстансинга был
  без геометрии и без индексов, и дро молча ничего не делало. **Валидация Vulkan при этом чиста** —
  `VulkanPipelineState` пересекает раскладку с рефлексией и выбрасывает входы, которые никто не
  кормит, поэтому «пустой дро» выглядит совершенно законно. Не искать такое по валидации.

## Матричные вершинные входы (2026-08-22)

- ⚠️ **`in mat4` занимает по локации на столбец, а рефлексия сообщает ОДНУ запись.** SPIRV-Reflect
  отдаёт матричный вход как одну переменную с `location = N`; реальные локации — N..N+column_count-1.
  Пересечение раскладки меша с рефлексией (`consumesLocation`) поэтому выбрасывало столбцы 1..3
  инстансной модельной матрицы: **Vulkan рисовал рваную геометрию, DX12 не создавал пайплайн**.
  В `ShaderReflection::VertexInput` есть `m_locationsCount` — сравнивать локацию **диапазоном**, а не
  на равенство.
- ⚠️ **DX12: у матрицы семантика первой локации плюс номер столбца.** spirv-cross генерирует
  `float4 m_0 : TEXCOORD0_0; … m_3 : TEXCOORD0_3;`, а D3D разбирает это как `SemanticName =
  "TEXCOORD0_"`, `SemanticIndex = 0..3`. Обычное `TEXCOORD`/индекс = локация даёт `E_INVALIDARG` из
  `CreateGraphicsPipelineState` и сообщение слоя отладки «input signature expects TEXCOORD0_/0».
  Имя семантики — заимствованный указатель, читаемый в момент создания PSO: строку держать живой
  до вызова.
- Первый инструмент при «DX12 не рисует» — `SG_DX_DUMP_HLSL=1` и грепнуть `TEXCOORD` в дампе
  вершинной стадии: там видны точные семантики, которые ждёт подпись входа.

## Смоук-тест

- `Tests/Smoke` → `SGSmokeTest` (`--gapi`, `--frames`, `--output`, `--reference`,
  `--threshold`, `--max-diff`, `--help`; коды выхода 0/1/2). Эталоны —
  `Tests/Smoke/references/smoke_<gapi>_<WxH>.png`, привязаны к разрешению монитора.
- GL4 детерминирован: повторный прогон 0 % расхождений (SSAO и стохастическая
  прозрачность воспроизводимы).
- Запуск — из каталога `Tests/Smoke` внутри build-dir, рядом должна лежать `SGCore.dll`
  (`copy_sgcore_dlls()` в `cmake/utils.cmake` закомментирована — копировать руками).
- Первый кадр после старта — не эталон: захват на 60-м кадре (прогрев шейдеров/CSM).

## Мелкие грабли API

- **`ITexture2D::create<T>(data, …)` не выставляет `m_dataType` и копирует `w*h*размер_internalFormat`
  байт** — для не-8-битных данных это неверно. Пример: шумовая текстура SSAO (`SSAO::generateKernel`)
  создаётся из float-буфера как `RGB16_FLOAT`, но остаётся `UNSIGNED_BYTE`: GL заливает биты float
  как байты (детерминированный мусор — и он **уже в эталоне** `smoke_gl_1920x1080.png`), Vulkan
  аплоад отклоняет и текстура пустая.
  **Измерено 2026-08-18 на полных пересборках (патчи в скрэтчпаде `A_upload_conversion.patch`,
  `B_create_datatype.patch`)** — чинить это сейчас НЕ надо, ни один вариант не сближает бэкенды:
  - HEAD как есть: GL 0.000 %, Vulkan 7.138 %;
  - «A» — конверсия формата при аплоаде на Vulkan (`float32→float16` и т. п., по образцу декодирования
    в `readAttachmentPixels`): отказ аплоада уходит, GL не затронут (0.000 %), но Vulkan **ухудшается
    до 46.171 %** — с загруженной шумовой текстурой SSAO на Vulkan перестаёт давать затенение вовсе,
    хотя оба пути читают одни и те же 48 байт;
  - «B» = A + вывод `m_dataType`/размера из `DataType`: GL-эталон сдвигается на **2.003 %** (max diff
    26 — меняется форма и сила контактных теней SSAO), Vulkan — 51.887 %, и **Vulkan против нового
    GL-кадра всё равно 51.857 %**.
  **Причина найдена 2026-08-18 прогоном самой окклюзии** (вложение 2 FX-буфера, `--attachment 2`):
  на GL там реальное затенение с дизерингом, на Vulkan — **сплошное белое, окклюзии нет вовсе**
  (вложение 2 — 7.764 %, max diff 197). Цепочка доказана пробниками в шейдере:
  1. ядро и счётчик (`SG_SSAO_samples[]`, `SG_SSAO_samplesCount`) доходят **идентично** на обоих
     бэкендах — проверено, это не они;
  2. G-буфер (позиции, нормали) совпадает 0.000 % — тоже не он;
  3. единственный расходящийся вход — шумовая текстура: GL сэмплирует `(0.78, 0.85, …)`,
     Vulkan — `(0, 0, 0)`, потому что её аплоад отклонён и образ очищен в нули;
  4. в `ssao.glsl` идёт `normalize(randomVec - viewNormal * dot(randomVec, viewNormal))`, а
     `normalize(vec3(0))` = **NaN**; NaN отравляет TBN, все сравнения выборок становятся ложными,
     `occlusion` копится нулём и итог `1.0 - 0/count` = 1.0 — затенение исчезает целиком.
  **Починено 2026-08-18 конверсией формата при аплоаде** (`VkTexture2D::uploadRegion` строит staging в
  раскладке образа, декодируя канал за каналом — аплоадный двойник того, что делает
  `readAttachmentPixels`). После этого шум на Vulkan сэмплируется **точь-в-точь** как на GL
  (`(198, 216)` и `(32, 252)` в одних и тех же точках), отказов аплоада ноль, окклюзия считается и
  структурно совпадает с GL.
  **Остаток закрыт 2026-08-22 — это была неопределённая альфа G-буфера, а не математика прохода.**
  `ssao.glsl` умножал на `camera.viewMatrix` **весь vec4** из `u_GBufferWorldPos`, а геометрический
  проход объявляет это вложение как `out vec3` (locations 2, 4, 5, 6 — все vec3): альфа-канал никто не
  пишет, и трансляционный столбец матрицы домножался на мусор. На GL там случайно оказывалась единица,
  на Vulkan/DX12 — нет. Итог: вложение 2 — 39.122 % → **0.938 %**, финальный кадр — 46.17 % → 0.096 %
  (Vulkan) и 0.000 % (DX12), эталон GL при этом **не сдвинулся**. Правка — `vec4(texture(...).xyz, 1.0)`
  в двух местах `ssao.glsl` (сама вид-позиция и `sampleDepth` в цикле).
  **Как нашли, и почему раньше не находили**: пробники сравнивали `.xyz` (совпадало 0.000 %), а
  расхождение сидело в четвёртом канале, которого в пробнике не было. Ловится одним пробником
  `fragColor = vec4(vec3(texture(u_GBufferWorldPos, uv).w), 1.0)` → 100 % расхождения, max 255.
  ⚠️ **Правило**: у вложения, которое шейдер объявляет как `out vec3`, альфа НЕОПРЕДЕЛЕНА — читать
  такое вложение можно только со свизлом. Слой отладки DX12 говорит это прямым текстом
  («Pixel Shader output 'SV_TargetN' is writing to 3 components, while the RenderTarget has 4 —
  undefined contents in the unwritten channels»); у Vulkan аналогичного предупреждения нет.
  ⚠️ **Пробник с `fract()` или другой немонотонной функцией врёт**: `fract(abs(x) * 0.01)` на
  `fragViewPos` давал 100 % расхождения там, где реальная разница — единицы младших бит. Кодировать
  значение линейно (`x * k + 0.5`), тогда max channel diff читается как настоящая величина ошибки.
- `IMaterial`: `m_metallicFactor`/`m_roughnessFactor`/цвета — **protected**; использовать
  `setMetallicFactor()`, `setRoughnessFactor()`, `setDiffuseColor()`. Публичные:
  `m_shaders`, `m_meshRenderState`, `m_transparencyType`.
- `WindowConfig` — поля private (`friend struct Window`), менять через сеттеры `Window`.
- `AssetManager`: `getAsset<T, AssetStorageType::BY_ALIAS>("alias")`,
  `loadAssetWithAlias<T>("alias", "${enginePath}/...")`, `getOrAddAssetByAlias<T>("alias")`.
  Пути с `${enginePath}` резолвятся через `PathInterpolationMarkupSpec` (ключ выставляет
  `CoreMain::init` из `SUNGEAR_SOURCES_ROOT`).
- Логгер: `SG_LOG_I/W/E/C("fmt {}", args...)` — `fmt::format_string`, `std::string_view`
  форматируется; без аргументов работает (`##__VA_ARGS__`).
- `Ref<T>` = `std::shared_ptr<T>` из `Main/CoreGlobals.h` (нет `Utils/Ref.h`).
- Сборка тянет ANTLR-регенерацию по mtime (нужен `java`); она меняет только путь в
  заголовке 12 файлов `UI/ANTLR4CSS3Generated/` — откатывать перед коммитом.
- **После правки заголовков SGCore пересобирать все тестовые exe** и класть рядом свежую
  `SGCore.dll`: старый exe + новая DLL = «Run-Time Check Failure #2 … stack corrupted»
  (2026-08-17: `SGRHITest` со старым размером `SGSLEVulkanizer::Report`). Это ABI, не баг кода.
- **Инкрементальная сборка в этом build-каталоге НЕ отслеживает зависимости от заголовков** — правка
  `.h` пересобирает не всех, кто его включает. Проверено 2026-08-18: правка `ITexture2D.h` дала
  «Building CXX object» ровно **1 раз**, а полная пересборка того же дерева — **300**. Последствие:
  DLL собирается смешанным (часть объектов со старым заголовком, часть с новым) и **даёт другую
  картинку**. Так был получен фантомный «сдвиг GL-эталона на 2.003 %», которого на самом деле нет:
  на полной пересборке того же коммита GL даёт 0.000 %. Правило: **любой замер после правки
  заголовка — только после полной пересборки** (`Remove-Item -Recurse cmake-build-*/Sources/SGCore/CMakeFiles/SGCore.dir`,
  затем сборка; ~300 юнитов, несколько минут). Удаление каталога объектов **проверять** (относительный путь из другого cwd «удаляет» несуществующий каталог и молча возвращает успех — стоило целого ложного прогона 2026-08-22). Признак смешанной сборки — счётчик собранных юнитов
  в логе не бьётся с масштабом правки. Особенно коварно для шаблонов в заголовках
  (`ITexture2D::create<T>`): они инстанцируются во многих TU.
