//
// Created by ilya on 25.02.24.
//

#include "TextRenderPass.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Memory/Assets/Font.h"
#include "SGCore/Memory/Assets/FontSpecialization.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/UI/Text.h"
#include "SGCore/UI/FontSpecializationRenderer.h"
#include "UICamera.h"
#include "RenderingBase.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"

void SGCore::TextRenderPass::render(const SGCore::Ref<SGCore::Scene>& scene,
                                    const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    auto textsView = scene->getECSRegistry().view<Text, Ref<Transform>>();
    auto uiCamerasView = scene->getECSRegistry().view<UICamera, RenderingBase, Ref<Transform>>();
    auto fontsView = AssetManager::getRegistry().view<Ref<Font>>();
    
    textsView.each([](Text& text, Ref<Transform>& textTransform) {
        Ref<Font> font = text.m_usedFont.lock();
        
        if(font)
        {
            Ref<FontSpecialization> fontSpec = font->getSpecialization(text.m_fontSettings);
            
            if(fontSpec)
            {
                fontSpec->getRenderer()->drawText(text, textTransform);
            }
        }
    });
    
    uiCamerasView.each([&fontsView](UICamera& uiCamera, RenderingBase& renderingBase, Ref<Transform>& transform) {
        CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);
        
        fontsView.each([](Ref<Font>& font) {
            for(const auto& spec : font->getSpecializations())
            {
                spec.second->getRenderer()->drawAll();
            }
        });
    });
    
    fontsView.each([](Ref<Font>& font) {
        for(const auto& spec : font->getSpecializations())
        {
            spec.second->getRenderer()->resetRenderer();
        }
    });
}
