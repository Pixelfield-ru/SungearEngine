#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/ImportedScenesArch/IMesh.h"

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
