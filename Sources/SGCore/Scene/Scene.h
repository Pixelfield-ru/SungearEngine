#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <SGCore/pch.h>

#include <sgcore_export.h>

#include "Layer.h"
#include "ISystem.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/UniqueNamesManager.h"
#include "SGCore/Utils/Event.h"
#include "SGCore/Utils/EventListener.h"
#include "SGCore/Utils/TypeTraits.h"

namespace SGCore
{
    namespace Serde
    {
        enum class FormatType;
        template<typename T, FormatType TFormatType>
        struct SerializableValueView;
    }

    struct XMLDocument;

    struct SceneEntitySaveInfo
    {
        Scene* m_savableScene { };
        entity_t m_savableEntity { };
    };
    
    class SGCORE_EXPORT Scene : public std::enable_shared_from_this<Scene>
    {
    private:
        template<Serde::FormatType TFormatType>
        struct SGCORE_EXPORT EntitySaveEvent
        {
            static inline Event<void(Scene& savableScene,
                                     const entity_t& savableEntity,
                                     Serde::SerializableValueView<SceneEntitySaveInfo, TFormatType>& entityView)> onEntitySave;
        };

    public:
        sg_serdespec_as_friend()

        Scene();
        
        std::string m_name;

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
        
        void setUIXMLDocument(const Ref<XMLDocument>& xmlDocument) noexcept;
        Weak<XMLDocument> getUIXMLDocument() const noexcept;
        void reloadUI() noexcept;
        
        double getUpdateFunctionExecutionTime() const noexcept;
        double getFixedUpdateFunctionExecutionTime() const noexcept;
        
        void saveToFile(const std::filesystem::path& path) noexcept;
        
        static void addScene(const Ref<Scene>& scene) noexcept;
        static Ref<Scene> getScene(const std::string& sceneName) noexcept;
        static void setCurrentScene(const std::string& sceneName) noexcept;
        
        static const auto& getScenes() noexcept
        {
            return m_scenes;
        }
        
        SG_NOINLINE static auto& getOnSceneSavedEvent() noexcept
        {
            return onSceneSaved;
        }

        template<Serde::FormatType TFormatType>
        SG_NOINLINE static auto& getOnEntitySave() noexcept
        {
            return EntitySaveEvent<TFormatType>::onEntitySave;
        }

    private:
        static inline Event<void(Scene& savedScene)> onSceneSaved;

        double m_update_executionTime = 0.0;
        double m_fixedUpdate_executionTime = 0.0;
        
        Ref<registry_t> m_ecsRegistry = MakeRef<registry_t>();

        Ref<UniqueNamesManager> m_uniqueNamesManager = MakeRef<UniqueNamesManager>();

        std::vector<Ref<ISystem>> m_systems;
        std::vector<Ref<Layer>> m_layers;

        size_t m_maxLayersCount = 0;
        
        Weak<XMLDocument> m_UIXMLDocument;
        
        static inline Ref<Scene> m_currentScene;
        static inline std::vector<Ref<Scene>> m_scenes;
    };
}

#endif //SUNGEARENGINE_SCENE_H