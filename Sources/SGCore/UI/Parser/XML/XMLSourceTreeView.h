#pragma once
#include <pugixml.hpp>

#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/Utils/PolymorphicIterator/StaticIterWrapper.h"

namespace SGCore::UI
{
    struct XMLSourceTreeView : UISourceTreeView
    {
        ~XMLSourceTreeView() override;

        pugi::xml_node rootNode;

        SGCore::Utils::PolymorphicIterator<UISourceTreeView&> children() override {
            auto w = SGCore::Utils::PolymorphicIteratorStaticIteratorWrapper<
                pugi::xml_node,
                pugi::xml_object_range<pugi::xml_node_iterator>,
                pugi::xml_node_iterator
            >(rootNode.children());
        }
    };
}


