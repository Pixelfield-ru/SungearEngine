#pragma once
#include "Parser/UISourceTreeView.h"

namespace SGCore::UI
{
    template<typename UISourceTreeViewValue, typename T> requires IUISourceTreeViewValue<UISourceTreeViewValue>
    struct Deserializer
    {
        static void deserializeInto(UISourceTreeViewValue treeView, T& obj) noexcept {  } // TODO: Fail
    };

    template<typename UISourceTreeViewValue> requires IUISourceTreeViewValue<UISourceTreeViewValue>
    struct Deserializer<UISourceTreeViewValue, float>
    {
        static void deserializeInto(UISourceTreeViewValue treeView, float& obj) noexcept;
    };

    template<typename UISourceTreeViewValue> requires IUISourceTreeViewValue<UISourceTreeViewValue>
    struct Deserializer<UISourceTreeViewValue, std::string>
    {
        static void deserializeInto(UISourceTreeViewValue treeView, std::string& obj) noexcept;
    };

}
