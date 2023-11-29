//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_WINDOW_H
#define SUNGEARENGINE_WINDOW_H

#include "SGCore/ImGuiWrap/Views/IView.h"

namespace SGCore::ImGuiWrap
{
    struct Window : public IView
    {
        bool begin() noexcept final;
        void renderBody() final;
        void end() noexcept final;
    };
}

#endif //SUNGEARENGINE_WINDOW_H
