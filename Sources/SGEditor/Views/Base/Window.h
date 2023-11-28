//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_WINDOW_H
#define SUNGEARENGINE_WINDOW_H

#include <vector>
#include "IView.h"

namespace SGEditor
{
    struct Window : public IView
    {
        void render() noexcept final;
    };
}

#endif //SUNGEARENGINE_WINDOW_H
