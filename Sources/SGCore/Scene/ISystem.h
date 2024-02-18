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
#include "entt/entity/entity.hpp"

namespace SGCore
{
    class Scene;

    class ISystem
    {
        friend class Scene;
    public:
        bool m_active = true;

        virtual void fixedUpdate() { }
        virtual void update() { }
        virtual void onAddToScene() { }
        
        virtual void setScene(const Ref<Scene>& scene) noexcept;
        
        double getUpdateFunctionExecutionTime() const noexcept;
        double getFixedUpdateFunctionExecutionTime() const noexcept;

    protected:
        Weak<Scene> m_scene;
        
        double m_update_executionTime = 0.0;
        double m_fixedUpdate_executionTime = 0.0;
    };
}

#endif //NATIVECORE_ISYSTEM_H
