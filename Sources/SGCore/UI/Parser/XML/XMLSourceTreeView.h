#pragma once
#include <pugixml.hpp>

#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/Utils/PolymorphicIterator/StaticIterWrapper.h"

namespace SGCore::UI
{
    struct XMLSourceTreeView : UISourceTreeView
    {
        ~XMLSourceTreeView() override;

        XMLSourceTreeView(const pugi::xml_node& node) : m_node(node) {};

        pugi::xml_node m_node;

        Ref<SGCore::Utils::PolymorphicIterator<UISourceTreeView&>> children() override {

        }
    };
}


