//
// Created by stuka on 29.11.2025.
//

#pragma once

#include <string>

#include "SGCore/UI/AttributeValue.h"

namespace SGCore::UI
{
    struct TemplateAttribute
    {
        AttributeValue<std::u32string> m_value;
        std::u32string m_defaultValue;
        bool m_isRequired = true;
    };
}
