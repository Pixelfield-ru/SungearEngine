//
// Created by stuka on 26.01.2025.
//

#ifndef SUNGEARENGINE_UINODESPROCESSORSUTILS_H
#define SUNGEARENGINE_UINODESPROCESSORSUTILS_H

#include "SGCore/UI/UIDocument.h"

namespace SGCore::UI::UINodesProcessorsUtils
{
    void printUnknownAttributeError(const UIDocument* inDocument,
                                    const pugi::xml_attribute& attribute,
                                    const pugi::xml_node& xmlNode) noexcept;

    InterpolatedPath resolveIncludePath(const UIDocument* inDocument,
                                        const std::string& originalPath) noexcept;
}

#endif //SUNGEARENGINE_UINODESPROCESSORSUTILS_H
