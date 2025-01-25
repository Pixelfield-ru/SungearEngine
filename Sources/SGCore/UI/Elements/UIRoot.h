//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_ROOT_H
#define SUNGEARENGINE_ROOT_H

#include "SGCore/UI/UIElement.h"

namespace SGCore::UI
{
    struct UIRoot : UIElement
    {
        void calculateLayout() noexcept final;
    };
}

#endif //SUNGEARENGINE_ROOT_H
