//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include <SGCore/pch.h>

#include "SGCore/Threading/ThreadsManager.h"
#include "SGCore/Threading/Thread.h"
#include "SGCore/Utils/Marker.h"
#include "SGCore/Utils/Singleton.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class Scene;

    class ISystem
    {
        friend class Scene;
    public:
        virtual ~ISystem() = default;

        bool m_active = true;
        std::unordered_map<std::string, double> m_executionTimes;

        virtual void fixedUpdate(const double& dt, const double& fixedDt) { }
        virtual void update(const double& dt, const double& fixedDt) { }
        // scene - this (m_scene) locked scene
        virtual void onAddToScene(const Ref<Scene>& scene) { }
        // scene - this (m_scene) locked scene
        virtual void onRemoveFromScene(const Ref<Scene>& scene) { }
        
        void setScene(const Ref<Scene>& scene) noexcept;
        Weak<Scene> getScene() const noexcept;
        
        Ref<Threading::Thread> getThread() const noexcept;

    protected:
        Ref<Threading::Thread> m_thread = Threading::ThreadsManager::getMainThread();
        
        Weak<Scene> m_scene;
    };
}

#endif //NATIVECORE_ISYSTEM_H
