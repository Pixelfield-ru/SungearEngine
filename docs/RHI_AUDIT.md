# 🔬 RHI_AUDIT — аудит Graphics/API перед переходом на Vulkan/DX12

Артефакт задачи **1.1** из [IMPLEMENTATION_PLAN.md](./IMPLEMENTATION_PLAN.md#этап-1-ревизия-rhi-и-шейдерный-пайплайн).
Сверено с кодом на **2026-08-16**. Служит входом для задачи 1.2
(проектирование целевого RHI).

## 📋 Оглавление

1. [Что уже хорошо](#-что-уже-хорошо-фундамент-перехода)
2. [GL-измы, требующие ревизии](#-gl-измы-требующие-ревизии)
3. [Оценка охвата](#-оценка-охвата)
4. [Выводы для задачи 1.2](#-выводы-для-задачи-12)

---

## ✅ Что уже хорошо (фундамент перехода)

1. **Изоляция GL-вызовов — фактически полная.** Все 244 прямых `gl*`-вызова
   в SGCore живут внутри `Graphics/API/GL/` (13 файлов). Единственное
   вхождение вне бэкенда — **закомментированный** `glGetError()` в
   `Render/BaseRenderPasses/SunShadowsPass.cpp:61`. Рендер-код ходит только
   через интерфейсы.
2. **Абстрактные перечисления состояний.** `GraphicsDataTypes.h` — свои типы
   (`SGFaceType`, `SGBlendingFactor`, `SGDepthStencilFunc`, `SGDrawMode`…),
   сырые `GLenum` наружу не торчат; каст в GL — в `GLGraphicsTypesCaster.h`.
3. **Пассовая архитектура рендера.** `IRenderPass` / `IRenderPipeline` /
   `RenderPipelinesManager` — проходы уже перечислимы и упорядочены; это
   готовый каркас для будущей записи командных буферов.
4. **Фабрика ресурсов в `IRenderer`** (`createShader`, `createTexture2D`,
   `createFrameBuffer`…) — точка подмены бэкенда существует, выбор по
   `GAPIType` работает.
5. **Окно готово к Vulkan.** `Main/Window.cpp:36` уже ставит
   `GLFW_CLIENT_API = GLFW_NO_API` для `SG_API_TYPE_VULKAN`.
6. **Свой шейдерный язык SGSL** (`Utils/SGSL/`: `SGSLETranslator`,
   `ShaderAnalyzedFile`, суб-шейдеры) — единая точка, куда добавляется
   SPIR-V-таргет; шейдеры движка не придётся переписывать руками.

---

## ⚠️ GL-измы, требующие ревизии

Нумерация — по убыванию стоимости.

### 1. Поимённые юниформы — самый дорогой пункт

`IShader` (`Graphics/API/IShader.h:85-105`): `useMatrix("name", …)`,
`useFloat`, `useVectorf`, `useTextureBlock`, `getShaderUniformLocation` —
модель «отдельный юниформ по строковому имени», которой в Vulkan **не
существует** (только UBO/SSBO, push constants и дескрипторы). Этими вызовами
пользуются **все** проходы рендера. Целевое состояние: материальные/пассовые
параметры уходят в uniform-блоки и push constants, раскладку даёт рефлексия
шейдера; строковый доступ остаётся только как отладочный слой поверх.

### 2. Глобальная стейт-машина состояний

`RenderState` / `BlendingState` / `MeshRenderState` с методами
`use(force)` и кешем текущего состояния в `IRenderer`
(`m_cachedRenderState`…) — прямое отражение GL. В Vulkan/DX12 бленд, depth,
stencil, cull, topology — **неизменяемая часть PSO**. Целевое состояние: эти
структуры превращаются в поля дескриптора пайплайна, вместо `use()` — выбор
готового PSO из хеш-кеша.

### 3. Немедленное исполнение, нет командных буферов

`IRenderer::renderMeshData` / `renderArray` рисуют сразу в текущий контекст;
кадр завершается `glfwSwapBuffers` (`Main/Window.cpp:494`). Нет понятий
command list, submit, кадров in-flight, синхронизации. Это ядро ревизии 1.2:
интерфейс записи команд + явный begin/end кадра со свопчейном у бэкенда.

### 4. VAO как публичная абстракция

`IVertexArray` — концепция OpenGL. В Vulkan/DX12 вместо неё: описание
vertex input (часть PSO) + байндинг буферов в командный буфер. Заодно:
`m_vertexBuffers` — `std::unordered_set<IVertexBuffer*>` (сырые указатели,
недетерминированный порядок; для vertex input порядок значим).

### 5. bind()-модель ресурсов и экранный фреймбуфер

`bind()` у шейдеров/текстур/буферов, `bindScreenFrameBuffer()`,
`renderTextureOnScreen()` — привязка к глобальному контексту. В комментариях
интерфейса прямо зафиксирована GL-конвенция: «OpenGL bottom-left origin»
(`IRenderer.h:89,97`) — координатные соглашения (viewport, origin, depth range
[-1;1] vs [0;1], флип Y в проекции) придётся вынести в свойства бэкенда.

### 6. Шейдеры компилируются из текста в рантайме

Цепочка сейчас: `TextFileAsset` → SGSL-транслятор → GLSL-строки →
`glShaderSource`/компиляция в `GL46Shader` (в т.ч. `m_autoRecompile`).
Для Vulkan/DX12 нужен путь SGSL → **SPIR-V** (офлайн или кешируемо на старте)
и рефлексия (дескрипторы, наборы, push-constant-блоки); для DX12 — далее
SPIR-V → DXIL. Runtime-перекомпиляция остаётся только в dev-режиме.

### 7. ImGui-бэкенд захардкожен

`ImGuiWrap/ImGuiLayer.cpp` включает `imgui_impl_opengl3.h` безусловно.
У imgui есть официальные `imgui_impl_vulkan`/`imgui_impl_dx12` — выбор
бэкенда должен идти по `GAPIType` (и по фиче vcpkg-пакета imgui).

### 8. Времена жизни ресурсов

Фабрики возвращают сырые `T*`, владение оформляется `Ref`/`Scope` поверх;
удаление ресурса немедленное. При кадрах in-flight (Vulkan/DX12) нужно
отложенное удаление (deferred destruction по номеру кадра) — место для него
в текущем `IGPUObjectsStorage` есть, но семантики отложенности нет.

---

## 📏 Оценка охвата

Потребители, которых заденет ревизия (все ходят в `IShader::use*` и
состояния):

- Проходы: `PBRRP`, `LWRP`, `ShadowMapping/CSM`, `Atmosphere`, `Decals`,
  `Terrain`, `PostProcess`, `Volumetric`, `Picking`, `Batching`, `Instancing`,
  `Lighting`, `TextRenderPass`, `DebugDraw`, `Gizmos`, `BaseRenderPasses`
  (≈ 16 подсистем в `Render/`).
- UI (`UI/`, ImGuiWrap), `SimpleFrameReceiver`/`LayeredFrameReceiver`.

GL-бэкендов сейчас три (`GL4`, `GL46`, `GLES` через те же классы) — мигрируют
на новый RHI как одна реализация (GL46 — эталон, задача 1.5).

---

## 🎯 Выводы для задачи 1.2

Проектировать в RHI (по результатам аудита):

1. **CommandList/CommandQueue** — проходы записывают команды, бэкенд
   исполняет; у GL-бэкенда исполнение может остаться немедленным внутри —
   интерфейс это допускает.
2. **PSO + хеш-кеш**: `RenderState`/`BlendingState`/`MeshRenderState` +
   vertex layout + шейдер = ключ пайплайна.
3. **Модель биндинга**: наборы дескрипторов (per-frame / per-pass /
   per-material / per-object), UBO и push constants вместо поимённых
   юниформов; рефлексия из SGSL.
4. **Свопчейн и кадр**: begin/end кадра, кадры in-flight, отложенное
   удаление ресурсов.
5. **Свойства бэкенда**: origin, depth range, флип Y — запрашиваются, а не
   предполагаются.
6. **Фасад совместимости** на время миграции: старые `use*`-вызовы работают
   поверх нового RHI (через дефолтный UBO), чтобы переносить проходы
   постепенно, а не одним big-bang.

---

*Документ фиксирует состояние на дату аудита; после ревизии RHI подлежит архивированию или обновлению.*
