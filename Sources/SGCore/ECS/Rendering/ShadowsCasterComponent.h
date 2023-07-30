//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H
#define SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H

//#include "SGCore/ECS/Rendering/ShadowsCasterSystem.h"

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"

#include "IRenderingComponent.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"

namespace Core::Graphics
{
    class IRenderer;
}

namespace Core::ECS
{
    class ShadowsCasterComponent : public IRenderingComponent
    {
        friend class ShadowsCasterSystem;

    private:
        static inline std::shared_ptr<Graphics::IShader> m_objectsShader;

        void init() noexcept final { }

    public:
        // frame buffer with depth attachment
        std::shared_ptr<Core::Graphics::IFrameBuffer> m_frameBuffer =
                std::shared_ptr<Core::Graphics::IFrameBuffer>(Main::CoreMain::getRenderer().createFrameBuffer())
                ->create()
                ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT,
                                "depthAttachment",
                                1024,
                                1024,
                                SGGColorFormat::SGG_DEPTH_COMPONENT,
                                SGGColorInternalFormat::SGG_DEPTH24_STENCIL8,
                                0,
                                0)
                                ->unbind();

        static std::shared_ptr<Graphics::IShader> getObjectsShader()
        {
            static bool firstInit = []() {
                m_objectsShader = std::shared_ptr<Graphics::IShader>(
                        Main::CoreMain::getRenderer().createOnlyGeometryShader()
                );

                return true;
            }();

            return m_objectsShader;
        }
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H
