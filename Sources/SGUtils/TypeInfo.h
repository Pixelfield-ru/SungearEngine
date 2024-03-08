//
// Created by stuka on 05.01.2024.
//

#ifndef ECS_TYPEMETA_H
#define ECS_TYPEMETA_H

#include <cstddef>
#include <set>
#include <map>

#define SGECS_DECLARE_BASE(DerivedType, BaseType) \
private: \
    static inline const bool sgTypeInfoInit_base_##BaseType = []() { \
                const auto& baseTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<BaseType>(); \
                const auto& derivedTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<DerivedType>(); \
                SGUtils::TypeInfoUtils::inheritAllBases(baseTypeInfo.getID(), derivedTypeInfo.getID()); \
                return true; \
            }(); \

#if defined _MSC_VER
#   define GENERATOR_PRETTY_FUNCTION __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
#   define GENERATOR_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

namespace SGUtils
{
    unsigned constexpr constexprHash(char const* input) {
        return *input ?
               static_cast<unsigned int>(*input) + 33 * constexprHash(input + 1) :
               5381;
    }

    struct TypesCounter
    {
        static size_t next() noexcept
        {
            return m_typesCount++;
        }

    private:
        static inline size_t m_typesCount = 0;
    };

    template<typename Type>
    struct TypeID
    {
#ifdef GENERATOR_PRETTY_FUNCTION
        static constexpr std::size_t id()
        {
            constexpr auto value = constexprHash(GENERATOR_PRETTY_FUNCTION);
            return value;
        }
#else
        static std::size_t id() {
            static const std::size_t value = TypesCounter::next();
            return value;
        }
#endif
    };
    
    template <typename> struct MemberFunctionTraits;
    
    template <typename Return, typename Object, typename... Args>
    struct MemberFunctionTraits<Return(Object::*)(Args...)>
    {
        typedef Return return_type;
        typedef Object instance_type;
        typedef Object & instance_reference;
        using function_type = Return(Object::*)(Args...);
        
        // Can mess with Args... if you need to, for example:
        static constexpr size_t argument_count = sizeof...(Args);
    };

    template <typename Return, typename Object, typename... Args>
    struct MemberFunctionTraits<Return(Object::*)(Args...) const>
    {
        typedef Return return_type;
        typedef Object instance_type;
        typedef Object const & instance_reference;
        using function_type = Return(Object::*)(Args...);
        
        // Can mess with Args... if you need to, for example:
        static constexpr size_t argument_count = sizeof...(Args);
    };
}

#endif //ECS_TYPEMETA_H
