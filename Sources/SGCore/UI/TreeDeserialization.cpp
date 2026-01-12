#include "TreeDeserialization.h"

template <typename UISourceTreeViewValue> requires SGCore::UI::IUISourceTreeViewValue<UISourceTreeViewValue>
void SGCore::UI::Deserializer<UISourceTreeViewValue, std::string>::deserializeInto(UISourceTreeViewValue treeView,
                                                                                   std::string& obj) noexcept {
    if (auto str = treeView.tryGetString()) {
        obj = std::move(*str);
    }
}