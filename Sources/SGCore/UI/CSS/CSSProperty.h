//
// Created by stuka on 17.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTY_H
#define SUNGEARENGINE_CSSPROPERTY_H

#include <any>

#include "CSSPropertyValueType.h"
#include "CSSPropertyType.h"

namespace SGCore::UI
{
    struct CSSProperty
    {
    private:
        std::any m_value;
        CSSPropertyValueType m_valueType = CSSPropertyValueType::PVT_UNKNOWN;
        CSSPropertyType m_type = CSSPropertyType::PT_UNKNOWN;
    };
}

#endif //SUNGEARENGINE_CSSPROPERTY_H
