//
// Created by stuka on 17.12.2024.
//

#include "PickingPass.h"

#include "SGCore/Utils/Paths.h"

void SGCore::PickingPass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline) noexcept
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            Paths::getDefaultPaths()["StandardMeshShader"]->resolved());
}

void SGCore::PickingPass::render(const SGCore::Ref<SGCore::Scene>& scene,
                                 const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline) noexcept
{

}
