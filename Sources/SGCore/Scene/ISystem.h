//
// Created by stuka on 02.05.2023.
//

#pragma once

#include "SGCore/Threading/ThreadsManager.h"
#include "SGCore/Threading/Thread.h"

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore
{
    class Scene;

    /**
     * You must implement \p sg_implement_type_id macro in your custom system type.
     */
    class SGCORE_EXPORT ISystem
    {
        friend class Scene;
    public:
        sg_implement_type_id_base(SGCore::ISystem)

        virtual ~ISystem() = default;

        bool m_active = true;
        std::unordered_map<std::string, double> m_executionTimes;

        virtual void fixedUpdate(double dt, double fixedDt) { }
        virtual void update(double dt, double fixedDt) { }
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
