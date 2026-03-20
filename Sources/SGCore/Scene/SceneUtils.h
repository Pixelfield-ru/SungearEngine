//
// Created by stuka on 20.12.2024.
//

#pragma once

#include <glm/vec2.hpp>
#include <sgcore_export.h>

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IFrameBuffer;

    struct SGCORE_EXPORT SceneUtils
    {
        [[nodiscard]] static ECS::entity_t pickEntity(const glm::vec2& cursorPos,
                                                      const ECS::registry_t& fromRegistry,
                                                      const IFrameBuffer* fromFrameBuffer,
                                                      SGFrameBufferAttachmentType fromAttachment) noexcept;
    };
}
