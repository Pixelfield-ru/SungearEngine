//
// Created by stuka on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRRPGEOMETRYPASS_H
#define SUNGEARENGINE_PBRRPGEOMETRYPASS_H

#include <entt/entity/registry.hpp>

#include "SGCore/Render/IRenderPass.h"
#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

namespace SGCore
{
    struct Mesh;
    struct OctreeNode;
    struct Transform;
    
    struct PBRRPGeometryPass : public IRenderPass, IGeometryPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;
        
        void render(const Ref<Scene>& meshEntity, const Ref<IRenderPipeline>& renderPipeline) final;
        
    private:
        void renderMesh(entt::registry& registry,
                        const entt::entity& meshEntity,
                        const Ref<Transform>& meshTransform,
                        Mesh& mesh,
                        const Ref<ISubPassShader>& standardGeometryShader) noexcept;
        void renderOctreeNode(entt::registry& registry,
                              const entt::entity& forCamera,
                              const Ref<OctreeNode>& node,
                              const Ref<ISubPassShader>& standardGeometryShader) noexcept;
    };
}

#endif //SUNGEARENGINE_PBRRPGEOMETRYPASS_H
