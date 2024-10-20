//
// Created by stuka on 20.10.2024.
//

#ifndef SUNGEARENGINE_SERDE_COMMON_H
#define SUNGEARENGINE_SERDE_COMMON_H

#include "SGCore/Utils/TypeTraits.h"

namespace SGCore::Serde
{
    template<typename... Types>
    using custom_derived_types = types_container<Types...>;

    template<typename T>
    concept custom_derived_types_t = types_container_t<T>;

    enum class FormatType
    {
        JSON,
        BSON,
        YAML
    };
}

#endif // SUNGEARENGINE_SERDE_COMMON_H
