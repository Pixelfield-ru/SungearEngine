#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ImportedScenesArch/IMesh.h"
#include "SGCore/ECS/IComponent.h"

namespace Core::ECS
{
    class MeshComponent : public IComponent
    {
    private:
        void init() noexcept final { }

    public:
        // just only mesh, yeah =)
        std::shared_ptr<ImportedScene::IMesh> m_mesh;
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
