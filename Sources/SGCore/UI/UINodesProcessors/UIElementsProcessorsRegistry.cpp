//
// Created by stuka on 24.09.2025.
//

#include "UIElementsProcessorsRegistry.h"

#include "UIButtonNodeProcessor.h"
#include "UIDivNodeProcessor.h"
#include "UIIncludeNodeProcessor.h"
#include "UIRootNodeProcessor.h"
#include "UITemplateNodeProcessor.h"
#include "UITextNodeProcessor.h"

SGCore::Ref<SGCore::UI::UIElementNodeProcessor> SGCore::UI::UIElementsProcessorsRegistry::getProcessor(const std::string& nodeType) noexcept
{
    auto it = s_nodesProcessors.find(nodeType);
    if (it != s_nodesProcessors.end())
    {
        return it->second;
    }

    return nullptr;
}

void SGCore::UI::UIElementsProcessorsRegistry::setProcessorForType(std::string nodeType, const Ref<UIElementNodeProcessor>& processor) noexcept
{
    s_nodesProcessors.emplace(std::move(nodeType), processor);
}

void SGCore::UI::UIElementsProcessorsRegistry::initializeCoreProcessors() noexcept
{
    s_nodesProcessors["xml"] = MakeRef<UIRootNodeProcessor>();
    s_nodesProcessors["template"] = MakeRef<UITemplateNodeProcessor>();
    s_nodesProcessors["include"] = MakeRef<UIIncludeNodeProcessor>();
    s_nodesProcessors["div"] = MakeRef<UIDivNodeProcessor>();
    s_nodesProcessors["text"] = MakeRef<UITextNodeProcessor>();
    s_nodesProcessors["button"] = MakeRef<UIButtonNodeProcessor>();
}
