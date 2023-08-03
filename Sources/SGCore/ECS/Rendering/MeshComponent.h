#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ImportedScenesArch/IMesh.h"
#include "SGCore/ECS/IComponent.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

namespace Core::ECS
{
    class MeshComponent : public IComponent
    {
    private:
        void init() noexcept final { }

    public:
        bool m_enableFacesCulling = true;
        SGFaceType m_facesCullingFaceType = SGFaceType::SGG_BACK_FACE;
        SGPolygonsOrder m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;

        // just only mesh, yeah =)
        std::shared_ptr<ImportedScene::IMesh> m_mesh;
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
