//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_SCENEHIERARCHY_H
#define SUNGEARENGINE_SCENEHIERARCHY_H

#include "SGCore/ImGuiWrap/Views/IView.h"
#include <entt/entity/entity.hpp>

namespace SGEditor
{
    struct SceneHierarchy : public SGCore::ImGuiWrap::IView
    {
        bool begin() noexcept final;
        void renderBody() final;
        void end() noexcept final;

    private:
        void renderEntity(const entt::entity& entity) noexcept;
    };
}

#endif //SUNGEARENGINE_SCENEHIERARCHY_H
