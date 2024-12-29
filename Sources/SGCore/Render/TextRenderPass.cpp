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
    auto textsView = scene->getECSRegistry()->view<Text, Transform>();
    auto uiCamerasView = scene->getECSRegistry()->view<UICamera, RenderingBase, Transform>();
    auto fontsView = AssetManager::getInstance()->getAssetsWithType<Font>();
    
    textsView.each([](Text::reg_t& text, Transform::reg_t& textTransform) {
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
    
    uiCamerasView.each([&fontsView](UICamera::reg_t& uiCamera, RenderingBase::reg_t& renderingBase, Transform::reg_t& transform) {
        CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);

        for(const auto& font : fontsView)
        {
            for(const auto& spec: font->getSpecializations())
            {
                spec.second->getRenderer()->drawAll();
            }
        }
    });
    
    for(const auto& font : fontsView)
    {
        for(const auto& spec : font->getSpecializations())
        {
            spec.second->getRenderer()->resetRenderer();
        }
    }
}
