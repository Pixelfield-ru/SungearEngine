//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include <functional>

#include "Scene.h"
#include "SGCore/Patterns/Marker.h"
#include "Transformations/TransformComponent.h"
#include "SGCore/Patterns/Singleton.h"

namespace Core::ECS
{
    struct SystemsFlags
    {
        // system`s functions will call not per entity
        static inline const std::uint16_t SGSF_NOT_PER_ENTITY = 1;
        static inline const std::uint16_t SGSF_PER_ENTITY = 1 << 1;
    };

    class ISystem : public Patterns::Marker<ISystem>
    {
        friend class ECSWorld;

    private:
        std::list<std::function<bool()>> m_fixedUpdateFunctionsQuery;
        std::list<std::function<bool()>> m_updateFunctionsQuery;

    public:
        bool m_active = true;

        virtual void fixedUpdate(const std::shared_ptr<Scene>& scene) { }
        virtual void update(const std::shared_ptr<Scene>& scene) { }

        virtual void fixedUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) { }
        virtual void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) { }

        virtual void cacheEntity(const std::shared_ptr<Entity>& entity) const { }

        template<typename Func, typename... Args>
        void addFunctionToFixedUpdateQuery(const Func& f, const Args&... args)
        {
            std::function<bool()> bindFunc = [f, args...]() { return f(args...); };
            m_fixedUpdateFunctionsQuery.push_back(bindFunc);
        }

        template<typename Func, typename... Args>
        void addFunctionToUpdateQuery(const Func& f, const Args&... args)
        {
            std::function<bool()> bindFunc = [&f, &args...]() { return f(args...); };
            m_updateFunctionsQuery.push_back(bindFunc);
        }
    };
}

#endif //NATIVECORE_ISYSTEM_H
