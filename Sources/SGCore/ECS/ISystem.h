//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include <functional>
#include <map>
#include <source_location>

#include "Scene.h"
#include "SGCore/Patterns/Marker.h"
#include "Transformations/Transform.h"
#include "SGCore/Patterns/Singleton.h"

#include "ComponentsCollection.h"

#include "SGCore/Main/CoreGlobals.h"
#include "ComponentsCollector.h"

namespace SGCore
{
    class ISystem
    {
        friend class ECSWorld;
    public:
        ComponentsCollector m_componentsCollector;

        bool m_active = true;

        virtual void fixedUpdate(const Ref<Scene>& scene) { }
        virtual void update(const Ref<Scene>& scene) { }

        virtual void fixedUpdate(const Ref<Scene>& scene, const Ref<Entity>& entity) { }
        virtual void update(const Ref<Scene>& scene, const Ref<Entity>& entity) { }

        template<typename Func, typename... Args>
        void addFunctionToFixedUpdateQuery(const std::string& funcUUID, const Func& f, const Args&... args)
        {
            std::function<bool()> bindFunc = [f, args...]() { return f(args...); };
            m_fixedUpdateFunctionsQuery[funcUUID] = bindFunc;
        }

        template<typename Func, typename... Args>
        void addFunctionToUpdateQuery(const std::string& funcUUID, const Func& f, const Args&... args)
        {
            std::function<bool()> bindFunc = [f, args...]() { return f(args...); };
            m_updateFunctionsQuery[funcUUID] = (bindFunc);
        }

        double getUpdateFunctionExecutionTime() const noexcept;
        double getFixedUpdateFunctionExecutionTime() const noexcept;

    protected:
        double m_update_executionTime = 0.0;
        double m_fixedUpdate_executionTime = 0.0;

        std::unordered_map<std::string, std::function<bool()>> m_fixedUpdateFunctionsQuery;
        std::unordered_map<std::string, std::function<bool()>> m_updateFunctionsQuery;
    };
}

#endif //NATIVECORE_ISYSTEM_H
