#ifndef SUNGEARENGINE_NODE_H
#define SUNGEARENGINE_NODE_H

#include "IMeshData.h"
#include "SGCore/Scene/Scene.h"

namespace SGCore
{
    class Node : public IAssetsRefsResolver<Node>
    {
        friend class ModelAsset;
        sg_assets_refs_resolver_as_friend

        using mesh_func = std::function<void(ECS::entity_t parentEntity, ECS::entity_t meshEntity)>;
        using entity_func = std::function<void(ECS::entity_t parentEntity)>;

        ECS::entity_t addOnScene(const Ref<Scene>& scene,
                                 const entity_func& eachEntityFunc,
                                 const mesh_func& meshFunc,
                                 const bool& rootAdd) const noexcept;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept;
        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

    public:
        std::string m_name;

        std::vector<Ref<Node>> m_children;
        std::vector<AssetRef<IMeshData>> m_meshesData;

        glm::vec3 m_position { 1.0 };
        glm::quat m_rotationQuaternion = glm::identity<glm::quat>();
        glm::vec3 m_scale { 1.0 };

        void addOnScene(const Ref<Scene>& scene,
                        const entity_func& eachEntityFunc,
                        const mesh_func& meshFunc) const noexcept;
        void addOnScene(const Ref<Scene>& scene, const entity_func& eachEntityFunc) const noexcept;
        void addOnScene(const Ref<Scene>& scene, const mesh_func& meshFunc) const noexcept;
        std::vector<ECS::entity_t> addOnScene(const Ref<Scene>& scene) const noexcept;

        /**
         * Trying to find mesh with name \p name , begging at this node.
         * @param name
         * @return
         */
        [[nodiscard]] AssetRef<IMeshData> findMesh(const std::string& name) const noexcept;
    };
}

#endif //SUNGEARENGINE_NODE_H
