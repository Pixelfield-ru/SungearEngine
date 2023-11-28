//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_COLLAPSINGHEADER_H
#define SUNGEARENGINE_COLLAPSINGHEADER_H

#include "IView.h"

namespace SGEditor
{
    struct CollapsingHeader : public IView
    {
        void render() noexcept final;
    };
}

#endif //SUNGEARENGINE_COLLAPSINGHEADER_H
