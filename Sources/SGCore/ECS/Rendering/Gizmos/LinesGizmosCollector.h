//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_LINESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_LINESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class LinesGizmosCollector : public ISystem
    {
    public:
        void cacheEntity(const std::shared_ptr<Entity>& entity) final;
    };
}

#endif // SUNGEARENGINE_LINESCOLLECTORSYSTEM_H
