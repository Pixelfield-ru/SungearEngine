//
// Created by stuka on 18.02.2026.
//

#pragma once

#include <entt/entity/entity.hpp>

#include "Config.h"
#include "CoreGlobals.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Utils/Signal.h"
#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"

namespace SGCore
{
    struct SGCORE_EXPORT BasicApp
    {
        InterpolatedPath m_configPath = "SungearEngineConfig.json";
        Config m_config;
        /// Attachment from main camera that will be displayed on screen.
        SGFrameBufferAttachmentType m_attachmentToDisplay = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
        bool m_flipDisplay = false;

        /// Created automatically if basic scene was created.
        ECS::entity_t m_cameraEntity = entt::null;
        /// Created automatically if basic scene was created.
        ECS::entity_t m_atmosphereEntity = entt::null;

        virtual ~BasicApp() noexcept;

        void start(bool createBasicScene = false) noexcept;

        virtual void onInit() noexcept = 0;
        virtual void onUpdate(double dt, double fixedDt) = 0;
        virtual void onFixedUpdate(double dt, double fixedDt) = 0;
        virtual void onDestroy() noexcept {}

    private:
        Slot<void()> m_initSlot = [this] {
            initImpl();
        };

        Slot<void(double dt, double fixedDt)> m_updateSlot = [this](double dt, double fixedDt) {
            updateImpl(dt, fixedDt);
        };

        Slot<void(double dt, double fixedDt)> m_fixedUpdateSlot = [this](double dt, double fixedDt) {
            fixedUpdateImpl(dt, fixedDt);
        };

        bool m_isBasicSceneMustBeCreated = false;

        void initImpl() noexcept;
        void updateImpl(double dt, double fixedDt) noexcept;
        void fixedUpdateImpl(double dt, double fixedDt) noexcept;
    };
}
