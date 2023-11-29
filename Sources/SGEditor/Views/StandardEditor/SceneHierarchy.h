//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_SCENEHIERARCHY_H
#define SUNGEARENGINE_SCENEHIERARCHY_H

#include "SGCore/ECS/Entity.h"
#include "SGCore/ImGuiWrap/Views/IView.h"

namespace SGEditor
{
    struct SceneHierarchy : public SGCore::ImGuiWrap::IView
    {
        bool begin() noexcept final;
        void renderBody() final;
        void end() noexcept final;

    private:
        void renderEntity(const SGCore::Ref<SGCore::Entity>& entity) noexcept;
    };
}

#endif //SUNGEARENGINE_SCENEHIERARCHY_H
