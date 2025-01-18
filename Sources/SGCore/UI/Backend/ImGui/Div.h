//
// Created by stuka on 19.01.2025.
//

#ifndef SUNGEARENGINE_UIDIV_H
#define SUNGEARENGINE_UIDIV_H

#include "SGCore/UI/UIElement.h"

namespace SGCore::UI
{
    struct Div : UIElement
    {
        void render() noexcept final;
    };
}

#endif //SUNGEARENGINE_UIDIV_H
