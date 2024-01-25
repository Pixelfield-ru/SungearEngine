//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include <functional>
#include <map>
#include <source_location>

#include "SGUtils/Marker.h"
#include "SGUtils/Singleton.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class Scene;

    class ISystem
    {
        friend class Scene;
    public:
        bool m_active = true;

        virtual void fixedUpdate(const Ref<Scene>& scene) { }
        virtual void update(const Ref<Scene>& scene) { }

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

        // this function realization must clear cached components and set up system for scene
        virtual void attachToScene(const Ref<Scene>& scene) = 0;

    protected:
        double m_update_executionTime = 0.0;
        double m_fixedUpdate_executionTime = 0.0;

        std::unordered_map<std::string, std::function<bool()>> m_fixedUpdateFunctionsQuery;
        std::unordered_map<std::string, std::function<bool()>> m_updateFunctionsQuery;
    };
}

#endif //NATIVECORE_ISYSTEM_H
