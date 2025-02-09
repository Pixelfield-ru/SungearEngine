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
    protected:
        void doCalculateLayout(const CSSSelectorCache* parentSelectorCache, CSSSelectorCache& thisSelectorCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMeshBaseSelector() noexcept final;

        void doGenerateBasicMesh() noexcept final;
    };
}

#endif //SUNGEARENGINE_ROOT_H
