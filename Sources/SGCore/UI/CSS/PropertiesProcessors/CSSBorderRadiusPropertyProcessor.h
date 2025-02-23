//
// Created by stuka on 22.02.2025.
//

#ifndef CSSROUNDINGPROPERTYPROCESSOR_H
#define CSSROUNDINGPROPERTYPROCESSOR_H

#include "SGCore/UI/CSS/CSSPropertyType.h"

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/UI/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_BORDER_RADIUS>
    {
        static constexpr int property_default_value = 0;
        static inline const std::string property_default_value_str = "unset";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {

        }
    };
}

#endif //CSSROUNDINGPROPERTYPROCESSOR_H
