//
// Created by stuka on 23.01.2025.
//

#ifndef SUNGEARENGINE_UIDIV_H
#define SUNGEARENGINE_UIDIV_H

#include "SGCore/UI/UIElement.h"

namespace SGCore::UI
{
    struct Div : UIElement
    {
    protected:
        void doCalculateLayout() noexcept final;

        void doGenerateMeshBaseSelector() noexcept final;

        void doGenerateBasicMesh() noexcept final;
    };
}

#endif //SUNGEARENGINE_UIDIV_H
