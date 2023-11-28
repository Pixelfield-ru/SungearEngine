//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_SCENEHIERARCHY_H
#define SUNGEARENGINE_SCENEHIERARCHY_H

#include "SGEditor/Views/Base/IView.h"
#include "SGCore/ECS/Entity.h"

namespace SGEditor
{
    struct SceneHierarchy : public IView
    {
        void render() final;

    private:
        void renderEntity(const SGCore::Ref<SGCore::Entity>& entity) noexcept;
    };
}

#endif //SUNGEARENGINE_SCENEHIERARCHY_H
