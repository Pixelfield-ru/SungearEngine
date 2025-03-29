#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <SGCore/pch.h>

#include <sgcore_export.h>
#include <optional>

#include "Layer.h"
#include "ISystem.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/UniqueNamesManager.h"
#include "SGCore/Utils/Signal.h"
#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/ECS/Registry.h"

sg_predeclare_serde()

namespace SGCore
{
    struct UIDocument;

    struct SceneEntitySaveInfo
    {
        const Scene* m_serializableScene { };
        ECS::entity_t m_serializableEntity { };
    };

    // todo: make on path changed event
    struct SceneMetaInfo
    {
        std::string m_sceneName;
        std::filesystem::path m_sceneLocalPath;
    };
    
    class SGCORE_EXPORT Scene : public std::enable_shared_from_this<Scene>
    {
    public:
        using systems_container_t = std::vector<Ref<ISystem>>;

    private:
        template<Serde::FormatType TFormatType>
        struct SGCORE_EXPORT SceneSerdeEvents
        {
            static inline Signal<void(Serde::SerializableValueView<SceneEntitySaveInfo, TFormatType>& entityView,
                                      const Scene& serializableScene,
                                      const ECS::entity_t& serializableEntity)> onEntitySerialize;

            static inline Signal<void(Serde::DeserializableValueView<SceneEntitySaveInfo, TFormatType>& entityView,
                                      const typename Serde::FormatInfo<TFormatType>::array_iterator_t& componentsIterator,
                                      ECS::registry_t& toRegistry)> onEntityDeserialize;

            static inline Signal<void(Serde::SerializableValueView<Scene::systems_container_t, TFormatType>& systemsContainerView,
                                      const Scene& serializableScene,
                                      const Ref<ISystem>& serializableSystem)> onSystemSerialize;

            static inline Signal<void(Serde::DeserializableValueView<Scene::systems_container_t, TFormatType>& systemsContainerView,
                                      const typename Serde::FormatInfo<TFormatType>::array_iterator_t& systemsIterator)> onSystemDeserialize;
        };

    public:
        sg_serde_as_friend()

        Scene();

        SceneMetaInfo m_metaInfo;

        void createDefaultSystems();

        static Ref<Scene> getCurrentScene() noexcept;
        
        void update(const double& dt, const double& fixedDt);
        void fixedUpdate(const double& dt, const double& fixedDt);

        template<typename SystemT>
        // requires(std::is_base_of_v<ISystem, SystemT>)
        Ref<SystemT> getSystem()
        {
            for(auto& system : m_systems)
            {
                if(SG_INSTANCEOF(system.get(), SystemT))
                {
                    return std::static_pointer_cast<SystemT>(system);
                }
            }

            return nullptr;
        }

        template<typename SystemT>
        // requires(std::is_base_of_v<SystemT, ISystem>)
        std::vector<Ref<SystemT>> getSystems()
        {
            std::vector<Ref<SystemT>> foundSystems;

            for(auto& system : m_systems)
            {
                if(SG_INSTANCEOF(system.get(), SystemT))
                {
                    foundSystems.push_back(std::static_pointer_cast<SystemT>(system));
                }
            }

            return foundSystems;
        }

        [[nodiscard]] bool isSystemExists(const Ref<ISystem>& system) const noexcept;

        void addSystem(const Ref<ISystem>& system) noexcept;
        const std::vector<Ref<ISystem>>& getAllSystems() noexcept;

        auto getUniqueNamesManager() const noexcept
        {
            return m_uniqueNamesManager;
        }

        // size_t createBaseEntity() noexcept;

        auto getECSRegistry() const noexcept
        {
            return m_ecsRegistry;
        }
        
        /**
         * Creates new layer or returns existing layer.
         * @param name - Name of new layer.
         * @return Layer.
         */
        Ref<Layer> createLayer(const std::string& name) noexcept;
        
        Ref<Layer> getLayer(const std::string& name) noexcept;

        void reloadUI() noexcept;
        
        double getUpdateFunctionExecutionTime() const noexcept;
        double getFixedUpdateFunctionExecutionTime() const noexcept;
        
        void saveToFile(const std::filesystem::path& path) noexcept;

        /**
         * Sets new pointer \p newScene to scene with the same name as in \p newScene or adds new scene \p newScene .
         * @param newScene
         */
        static void swapOrAddScene(const SceneMetaInfo& newScene) noexcept;
        static void addScene(const SceneMetaInfo& scene) noexcept;
        static std::optional<SceneMetaInfo> getSceneMeta(const std::string& sceneName) noexcept;
        static Ref<Scene> setCurrentScene(const std::string& sceneName) noexcept;
        static void setCurrentScene(const Ref<Scene>& scene) noexcept;
        static Ref<Scene> loadSceneAndSetAsCurrent(const std::filesystem::path& scenePath) noexcept;

        void resolveAllEntitiesRefs() const noexcept;

        SG_NOINLINE static auto& getOnSceneSavedEvent() noexcept
        {
            return onSceneSaved;
        }

        template<Serde::FormatType TFormatType>
        SG_NOINLINE static auto& getOnEntitySerializeEvent() noexcept
        {
            return SceneSerdeEvents<TFormatType>::onEntitySerialize;
        }

        template<Serde::FormatType TFormatType>
        SG_NOINLINE static auto& getOnEntityDeserializeEvent() noexcept
        {
            return SceneSerdeEvents<TFormatType>::onEntityDeserialize;
        }

        template<Serde::FormatType TFormatType>
        SG_NOINLINE static auto& getOnSystemSerializeEvent() noexcept
        {
            return SceneSerdeEvents<TFormatType>::onSystemSerialize;
        }

        template<Serde::FormatType TFormatType>
        SG_NOINLINE static auto& getOnSystemDeserializeEvent() noexcept
        {
            return SceneSerdeEvents<TFormatType>::onSystemDeserialize;
        }

    private:
        static inline Signal<void(const Scene& savedScene)> onSceneSaved;

        double m_update_executionTime = 0.0;
        double m_fixedUpdate_executionTime = 0.0;
        
        Ref<ECS::registry_t> m_ecsRegistry = MakeRef<ECS::registry_t>();

        Ref<UniqueNamesManager> m_uniqueNamesManager = MakeRef<UniqueNamesManager>();

        systems_container_t m_systems;
        std::vector<Ref<Layer>> m_layers;

        size_t m_maxLayersCount = 0;
        
        static inline Ref<Scene> m_currentScene;
        static inline std::vector<SceneMetaInfo> m_scenesMeta;
    };
}

#endif //SUNGEARENGINE_SCENE_H