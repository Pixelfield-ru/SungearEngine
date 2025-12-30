//
// Created by stuka on 26.11.2023.
//

#include <glm/gtc/type_ptr.hpp>

#include "PBRRPSpotLightsPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Lighting/SpotLight.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

void SGCore::PBRRPSpotLightsPass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline)
{
    m_renderTimer.setTargetFrameRate(24);
    
    m_renderTimer.onUpdate += m_renderListener;
}

void SGCore::PBRRPSpotLightsPass::render(const Scene* scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    auto spotLightsView = scene->getECSRegistry()->view<SpotLight, RenderingBase>();

    auto geomPasses = renderPipeline->getRenderPasses<IGeometryPass>();

    m_renderListener = [&](double dt, double fixedDt) {
        const auto geomPasses = renderPipeline->getRenderPasses<IGeometryPass>();

        spotLightsView.each([&](const SpotLight& spotLight, RenderingBase::reg_t& rendering) {
            // spotLight.m_base.m_shadowMap;
            /*spotLight.m_base.m_shadowMap->bind();

            for(const auto geomPass : geomPasses)
            {
                geomPass->renderShadows(scene, renderPipeline);
            }

            spotLight.m_base.m_shadowMap->unbind();*/
        });
    };
}
