//
// Created by stuka on 22.10.2023.
//

#ifndef SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Patterns/Singleton.h"

namespace SGCore
{
    class MeshesCollector : public ISystem
    {
        SG_DECLARE_SINGLETON(MeshesCollector)

    public:
        void cacheEntity(const std::shared_ptr<Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
