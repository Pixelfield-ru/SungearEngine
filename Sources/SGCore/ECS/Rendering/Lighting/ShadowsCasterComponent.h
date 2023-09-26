//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H
#define SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H

//#include "SGCore/ECS/Rendering/ShadowsCasterSystem.h"

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"

#include "SGCore/ECS/Rendering/IRenderingComponent.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/Graphics/GraphicsFilesResourcesManager.h"
#include "SGCore/Memory/Assets/ShaderAsset.h"

namespace Core::Graphics
{
    class IRenderer;
}

namespace Core::ECS
{
    class ShadowsCasterComponent : public IRenderingComponent
    {
        friend class ShadowsCasterSystem;

    public:
        /*std::shared_ptr<Memory::Assets::ShaderAsset> m_shaderAsset =
                Memory::AssetManager::loadAsset<Memory::Assets::ShaderAsset>(
                        Graphics::getShaderPath(Graphics::StandardShaderType::SG_SHADOWS_GENERATOR_SHADER)
                        );*/

        // frame buffer with depth attachment
        std::shared_ptr<Core::Graphics::IFrameBuffer> m_frameBuffer =
                std::shared_ptr<Core::Graphics::IFrameBuffer>(Main::CoreMain::getRenderer().createFrameBuffer())
                ->create()
                ->setSize(1024 * 2, 1024 * 2)
                ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT,
                                SG_FRAMEBUFFER_DEPTH_ATTACHMENT_NAME,
                                SGGColorFormat::SGG_DEPTH_COMPONENT,
                                SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F,
                                0,
                                0)
                                ->unbind();

        /*static std::shared_ptr<Graphics::IShader> getObjectsShader()
        {
            static bool firstInit = []() {
                m_objectsShader = std::shared_ptr<Graphics::IShader>(
                        Main::CoreMain::getRenderer().createShader(
                                Graphics::getShaderPath(Graphics::StandardShaderType::SG_SHADOWS_GENERATOR_SHADER)
                                )
                );

                return true;
            }();

            return m_objectsShader;
        }*/

    private:
        //std::shared_ptr<Graphics::IShader> m_shader;

        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H
