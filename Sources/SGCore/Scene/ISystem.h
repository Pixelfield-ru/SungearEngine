//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include <functional>
#include <map>
#include <source_location>
#include <thread>

#include "SGUtils/Marker.h"
#include "SGUtils/Singleton.h"

#include "SGCore/Main/CoreGlobals.h"
#include "entt/entity/entity.hpp"

namespace SGCore
{
    class Scene;

    class ISystem
    {
        friend class Scene;
    public:
        bool m_active = true;
        std::unordered_map<std::string, double> m_executionTimes;

        virtual void fixedUpdate(const double& dt, const double& fixedDt) { }
        virtual void update(const double& dt, const double& fixedDt) { }
        virtual void onAddToScene() { }
        virtual void onRemoveFromScene() { }
        
        virtual void setScene(const Ref<Scene>& scene) noexcept;
        Weak<Scene> getScene() const noexcept;
        
        [[nodiscard]] const size_t& getThreadID() const noexcept;

    protected:
        size_t m_threadID = std::hash<std::thread::id>()(std::this_thread::get_id());
        
        Weak<Scene> m_scene;
    };
}

#endif //NATIVECORE_ISYSTEM_H
