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
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore
{
    class Scene;

    /**
     * You must implement \p sg_implement_type_id macro in your custom system type.
     */
    class ISystem
    {
        friend class Scene;
    public:
        sg_implement_type_id_base(ISystem, 15)

        virtual ~ISystem() = default;

        bool m_active = true;
        std::unordered_map<std::string, double> m_executionTimes;

        virtual void fixedUpdate(const double& dt, const double& fixedDt) { }
        virtual void update(const double& dt, const double& fixedDt) { }
        // scene - this (m_scene) locked scene
        virtual void onAddToScene(const Scene* scene) { }
        // scene - this (m_scene) locked scene
        virtual void onRemoveFromScene(const Ref<Scene>& scene) { }
        
        void setScene(Scene* scene) noexcept;
        /**
         * Can be nullptr. You must ALWAYS check for nullptr.
         * @return Scene that is parent of current system.
         */
        [[nodiscard]] Scene* getScene() const noexcept;

        [[nodiscard]] Ref<Threading::Thread> getThread() const noexcept;

    protected:
        Ref<Threading::Thread> m_thread = Threading::ThreadsManager::getMainThread();

    private:
        Scene* m_scene {};
    };
}

#endif //NATIVECORE_ISYSTEM_H
