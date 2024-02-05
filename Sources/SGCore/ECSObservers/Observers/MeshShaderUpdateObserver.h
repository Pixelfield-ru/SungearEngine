//
// Created by Ilya on 05.02.2024.
//

#ifndef SUNGEARENGINE_MESHSHADERUPDATEOBSERVER_H
#define SUNGEARENGINE_MESHSHADERUPDATEOBSERVER_H

#include "entt/entt.hpp"

namespace SGCore
{
    struct MeshShaderUpdateObserver
    {
        static void onComponentConstruct(entt::registry& registry, const entt::entity& entity) noexcept;
    };
}

#endif //SUNGEARENGINE_MESHSHADERUPDATEOBSERVER_H
