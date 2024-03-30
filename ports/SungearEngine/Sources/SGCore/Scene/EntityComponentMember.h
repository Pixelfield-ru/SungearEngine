//
// Created by ilya on 04.03.24.
//

#ifndef SUNGEARENGINE_ENTITYCOMPONENTMEMBER_H
#define SUNGEARENGINE_ENTITYCOMPONENTMEMBER_H

#include "entt/entity/entity.hpp"

namespace SGCore
{
    template<typename T>
    struct EntityComponentMember
    {
        entity_t m_owner;
        T m_memberValue;
    };
}

#endif //SUNGEARENGINE_ENTITYCOMPONENTMEMBER_H
