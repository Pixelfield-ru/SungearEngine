//
// Created by stuka on 15.10.2023.
//

#ifndef SUNGEARENGINE_DATATRANSFERTOSHADERSSYSTEM_H
#define SUNGEARENGINE_DATATRANSFERTOSHADERSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class DataTransferToShadersSystem : public ISystem
    {
    public:
        void FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_DATATRANSFERTOSHADERSSYSTEM_H
