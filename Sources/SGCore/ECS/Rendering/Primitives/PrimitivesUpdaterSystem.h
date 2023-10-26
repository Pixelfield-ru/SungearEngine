#ifndef SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
#define SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class PrimitivesUpdaterSystem : public ISystem
    {
        SG_DECLARE_SINGLETON(PrimitivesUpdaterSystem)

    public:
        void update(const std::shared_ptr<Scene>& scene) final;

        void cacheEntity(const std::shared_ptr<Entity>& entity) const final;
    };
}

#endif //SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
