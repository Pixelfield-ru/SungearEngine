//
// Created by stuka on 26.01.2025.
//

#include "UINodesProcessorsUtils.h"

void SGCore::UI::UINodesProcessorsUtils::printUnknownAttributeError(const UIDocument* inDocument,
                                                                    const pugi::xml_attribute& attribute,
                                                                    const pugi::xml_node& xmlNode) noexcept
{
    LOG_E(SGCORE_TAG,
          "Can not process UIDocument correctly: unknown attribute '{}' (value: '{}') in node '{}'.\n"
          "In XML file: '{}'",
          attribute.name(),
          attribute.value(),
          xmlNode.name(),
          Utils::toUTF8(inDocument->getPath().resolved().u16string()));
}

SGCore::InterpolatedPath SGCore::UI::UINodesProcessorsUtils::resolveIncludePath(const UIDocument* inDocument,
                                                                                const std::string& originalPath) noexcept
{
    InterpolatedPath localPath = inDocument->getPath().raw().parent_path() / originalPath;

    if(std::filesystem::exists(localPath.resolved()))
    {
        return localPath;
    }

    return originalPath;
}