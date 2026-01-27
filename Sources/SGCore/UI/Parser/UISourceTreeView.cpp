#include "UISourceTreeView.h"

SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator& SGCore::UI::UISourceTreeViewObject::ChildrenCollection::
Iterator::operator++() {
    m_index++;
    return *this;
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator::operator
*() {
    return m_parentCollection[m_index];
}

bool SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator::operator
==(const Iterator& other) const {
    return m_index == other.m_index;
}

SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator& SGCore::UI::UISourceTreeViewObject::
PropertiesCollection::Iterator::operator++() {
    m_index++;
    return *this;
}

SGCore::UI::UISourceTreeViewObjectProperty& SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator::operator
*() {
    return m_parentCollection[m_index];
}

bool SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator::operator==(const Iterator& other) const {
    return m_index == other.m_index;
}
