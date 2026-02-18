//
// Created by stuka on 18.02.2026.
//

#pragma once

#include <entt/entity/entity.hpp>

#include "Config.h"
#include "CoreGlobals.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Utils/Slot.h"
#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"

namespace SGCore
{
    struct BasicApp
    {
        InterpolatedPath m_configPath = "SungearEngineConfig.json";
        Config m_config;
        /// Attachment from main camera that will be displayed on screen.
        SGFrameBufferAttachmentType m_attachmentToDisplay = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
        bool m_flipDisplay = false;

        virtual ~BasicApp() noexcept;

        void start(bool createBasicScene = false) noexcept;

        virtual void onInit() noexcept = 0;
        virtual void onUpdate(double dt, double fixedDt) = 0;
        virtual void onFixedUpdate(double dt, double fixedDt) = 0;
        virtual void onDestroy() noexcept {}

        /// Can be used only if basic scene was created
        ECS::entity_t getCameraEntity() const noexcept;

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

        ECS::entity_t m_cameraEntity = entt::null;

        void initImpl() noexcept;
        void updateImpl(double dt, double fixedDt) noexcept;
        void fixedUpdateImpl(double dt, double fixedDt) noexcept;
    };
}
