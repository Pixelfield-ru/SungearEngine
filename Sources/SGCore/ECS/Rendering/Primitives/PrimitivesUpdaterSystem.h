#ifndef SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
#define SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class PrimitivesUpdaterSystem : public ISystem
    {
    public:
        void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
