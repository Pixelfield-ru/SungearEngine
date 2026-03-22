//
// Created by stuka on 26.01.2025.
//

#pragma once

#include "SGCore/UI/UIDocument.h"

namespace SGCore::UI::UINodesProcessorsUtils
{
    SGCORE_EXPORT void printUnknownAttributeError(const UIDocument* inDocument,
                                                  const pugi::xml_attribute& attribute,
                                                  const pugi::xml_node& xmlNode) noexcept;

    SGCORE_EXPORT InterpolatedPath resolveIncludePath(const UIDocument* inDocument,
                                                      const std::string& originalPath) noexcept;
}
