//
// Created by stuka on 24.09.2025.
//

#pragma once

#include "UIElementNodeProcessor.h"
#include "SGCore/Main/CoreGlobals.h"
#include "UITemplateUsageProcessor.h"

namespace SGCore::UI
{
    struct UIElementsProcessorsRegistry
    {
        /**
         * Returns XML processor for node with type name \p nodeType .
         * @param nodeType Node type name.
         * @return Null or instance.
         */
        static Ref<UIElementNodeProcessor> getProcessor(const std::string& nodeType) noexcept;

        static void setProcessorForType(std::string nodeType, const Ref<UIElementNodeProcessor>& processor) noexcept;

    private:
        static inline std::unordered_map<std::string, Ref<UIElementNodeProcessor>> s_nodesProcessors;

        static void initializeCoreProcessors() noexcept;

        static inline bool s_staticInit = []() {
            UIElementsProcessorsRegistry::initializeCoreProcessors();
            return true;
        }();
    };
}
