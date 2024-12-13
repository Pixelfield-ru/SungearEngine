//
// Created by stuka on 26.11.2024.
//

#ifndef SUNGEARENGINE_POSTPROCESSFXSUBPASS_H
#define SUNGEARENGINE_POSTPROCESSFXSUBPASS_H

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IShader;

    struct PostProcessFXSubPass
    {
        SGFrameBufferAttachmentType m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;

        std::function<void(const Ref<IShader>& ppLayerShader)> m_prepassFunction;
        std::function<void(const Ref<IShader>& ppLayerShader)> m_postpassFunction;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXSUBPASS_H
