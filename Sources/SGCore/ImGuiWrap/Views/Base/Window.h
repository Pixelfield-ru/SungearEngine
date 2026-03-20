//
// Created by stuka on 28.11.2023.
//

#pragma once

#include "SGCore/ImGuiWrap/Views/IView.h"

namespace SGCore::ImGuiWrap
{
    struct SGCORE_EXPORT Window : public IView
    {
        bool begin() noexcept final;
        void renderBody() final;
        void end() noexcept final;
    };
}
