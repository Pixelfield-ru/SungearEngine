//
// Created by stuka on 24.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSPROPERTYPROCESSOR_H

#include "SGCore/UI/CSS/CSSPropertyType.h"

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/UI/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    template<CSSPropertyType PropertyType>
    struct CSSPropertyProcessor
    {
        static_assert(always_false_obj<PropertyType>::value, "CSSPropertyProcessor for this property is not implemented!");

        /// Must be implemented in your own specializations of CSSPropertyProcessor. Means default value of property.
        static constexpr int property_default_value = 0;
        /// Must be implemented in your own specializations of CSSPropertyProcessor. Means string with default value of property.
        static inline const std::string property_default_value_str = "unknown";

        /// Must be implemented in your own specializations of CSSPropertyProcessor.\n
        /// You must process property value from property declaration in this function.
        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {

        }
    };
}

#endif //SUNGEARENGINE_CSSPROPERTYPROCESSOR_H
