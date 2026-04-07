#pragma once
#include <type_traits>

namespace SGCore::UI::Deserialization
{
    template<typename T>
    struct MetaDef : std::false_type {};
}
