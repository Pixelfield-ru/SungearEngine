#pragma once
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/PolymorphicIterator/PolymorphicIterator.h"
#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"

namespace SGCore::UI
{
    struct UISourceTreeView
    {
        virtual ~UISourceTreeView() = default;

        virtual SGCore::Utils::PolymorphicIterator<UISourceTreeView&> children();
    };
}
