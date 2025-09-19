//
// Created by ilya on 25.02.24.
//

#include "TextRenderPass.h"

#include "LayeredFrameReceiver.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Memory/Assets/Font.h"
#include "SGCore/Memory/Assets/FontSpecialization.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/UI/FontSpecializationRenderer.h"
#include "UICamera.h"
#include "RenderingBase.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/UI/FontsManager.h"

void SGCore::TextRenderPass::render(const Scene* scene,
                                    const Ref<IRenderPipeline>& renderPipeline)
{
    // auto textsView = scene->getECSRegistry()->view<Text, Transform>();
    auto uiCamerasView = scene->getECSRegistry()->view<LayeredFrameReceiver, RenderingBase, Transform>();
    auto fontsView = UI::FontsManager::getInstance().getAssetManager()->getAssetsWithType<UI::Font>();
    
    /*textsView.each([](Text::reg_t& text, Transform::reg_t& textTransform) {
        Ref<Font> font = text.m_usedFont.lock();
        
        if(font)
        {
            Ref<FontSpecialization> fontSpec = font->getSpecialization(text.m_fontSettings);
            
            if(fontSpec)
            {
                fontSpec->getRenderer()->drawText(text, textTransform);
            }
        }
    });*/

    uiCamerasView.each([&fontsView](LayeredFrameReceiver::reg_t& cameraReceiver,
                                         RenderingBase::reg_t& renderingBase,
                                         Transform::reg_t& transform) {
        cameraReceiver.m_layersFrameBuffer->bind();

        cameraReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(cameraReceiver.m_attachmentToRenderIn);

        CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);

        for(const auto& font : fontsView)
        {
            for(const auto& spec: font->getSpecializations())
            {
                spec.second->getRenderer()->drawAll();
            }
        }

        cameraReceiver.m_layersFrameBuffer->unbind();
    });

    for(const auto& font : fontsView)
    {
        for(const auto& spec : font->getSpecializations())
        {
            spec.second->getRenderer()->resetRenderer();
        }
    }
}
