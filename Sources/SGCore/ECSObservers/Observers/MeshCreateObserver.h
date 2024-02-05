//
// Created by Ilya on 05.02.2024.
//

#ifndef SUNGEARENGINE_MESHCREATEOBSERVER_H
#define SUNGEARENGINE_MESHCREATEOBSERVER_H

#include "entt/entt.hpp"

namespace SGCore
{
    struct MeshCreateObserver
    {
        static void onMeshCreate(entt::registry& registry, const entt::entity& entity) noexcept;
    };
}

#endif //SUNGEARENGINE_MESHCREATEOBSERVER_H
