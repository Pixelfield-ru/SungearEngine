//
// Created by stuka on 22.10.2023.
//

#ifndef SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class MeshedEntitiesCollectorSystem : public ISystem
    {
    public:
        void cacheEntity(const std::shared_ptr<Entity>& entity) const final;
    };
}

#endif //SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
