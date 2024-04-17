//
// Created by Ilya on 05.12.2023.
//

#ifndef SUNGEARENGINE_POSTPROCESSFXSUBPASS_H
#define SUNGEARENGINE_POSTPROCESSFXSUBPASS_H

#include <functional>

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class ISubPassShader;

    struct PostProcessFXSubPass
    {
        std::function<void(const Ref<ISubPassShader>& ppLayerShader)> m_prepareFunction;
        /**
         * YOU MUST SPECIFY RENDER TARGET.
         */
        SGFrameBufferAttachmentType m_attachmentRenderTo = SGG_COLOR_ATTACHMENT1;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXSUBPASS_H
