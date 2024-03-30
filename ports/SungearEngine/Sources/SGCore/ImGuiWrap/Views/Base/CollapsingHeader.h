//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_COLLAPSINGHEADER_H
#define SUNGEARENGINE_COLLAPSINGHEADER_H

#include "SGCore/ImGuiWrap/Views/IView.h"

namespace SGCore::ImGuiWrap
{
    struct CollapsingHeader : public IView
    {
        bool begin() noexcept final;
        void renderBody() final;
        void end() noexcept final;
    };
}

#endif //SUNGEARENGINE_COLLAPSINGHEADER_H
