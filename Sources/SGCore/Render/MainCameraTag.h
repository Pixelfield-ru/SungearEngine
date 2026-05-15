//
// Created by stuka on 15.05.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"
#include "SGCore/ECS/SingletonComponent.h"
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore
{
    struct MainCameraTag : ECS::Component<MainCameraTag, const MainCameraTag>, ECS::SingletonComponent<MainCameraTag>
    {
        sg_implement_singleton_component(SGCore::MainCameraTag)
    };
}
