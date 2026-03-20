//
// Created by stuka on 26.11.2024.
//

#pragma once

#include <functional>
#include <sgcore_export.h>

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IShader;

    struct SGCORE_EXPORT PostProcessFXSubPass
    {
        SGFrameBufferAttachmentType m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;

        std::vector<std::pair<std::string, SGFrameBufferAttachmentType>> m_usedAttachments;

        std::function<void(const Ref<IShader>& ppLayerShader)> m_prepassFunction;
        std::function<void(const Ref<IShader>& ppLayerShader)> m_postpassFunction;
    };
}
