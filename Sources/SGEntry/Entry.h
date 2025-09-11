#ifndef SUNGEARENGINE_ENTRY_H
#define SUNGEARENGINE_ENTRY_H

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Serde/Serde.h>
#include <SGCore/Utils/TypeTraits.h>
#include "SGCore/Serde/StandardSerdeSpecs/SerdeSpecs.h"
// #include "SGCore/GeneratedSerializers.h"

struct Base
{
    int a = 3;

    virtual void v()
    {

    }
};

struct Derived : Base
{
    float b = 3.14f;
    std::string str0 = "hello utf8";
    std::basic_string<char16_t> str1 = u"hello utf16";
};

struct Derived0 : Derived
{
    float c = 5.1f;
    std::vector<float> floats { 1, 2, 3, 51.1f };
    glm::vec3 myVec3 { 3.0, 1.0, -1.0 };
    std::unordered_map<std::string, float> unMap { { "v0", 1.0f }, { "v1", -5.0f } };

    std::unique_ptr<Base> m_derived = std::make_unique<Derived>();
    Base m_base;
};

struct Derived1 : Derived
{
    float e = 3.0f;
    float f = 3.0f;
    float g = 3.0f;
};

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<Derived1, TFormatType> : SGCore::Serde::BaseTypes<Derived>
{
    static inline const std::string type_name = "Derived1";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const Derived1, TFormatType>& valueView) noexcept
    {
        std::printf("derived1 serializing\n");
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<Derived1, TFormatType>& valueView) noexcept
    {
        std::printf("derived1 deserializing\n");
    }
};

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<Derived0, TFormatType> : SGCore::Serde::BaseTypes<Derived>
{
    static inline const std::string type_name = "Derived0";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const Derived0, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("c", valueView.m_data->c);
        valueView.container().addMember("floats", valueView.m_data->floats);
        valueView.container().addMember("myVec3", valueView.m_data->myVec3);
        valueView.container().addMember("unMap", valueView.m_data->unMap);
        valueView.container().addMember("m_derived", valueView.m_data->m_derived);
        valueView.container().addMember("m_base", valueView.m_data->m_base);

        std::printf("derived0 serializing\n");
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<Derived0, TFormatType>& valueView) noexcept
    {
        const auto c = valueView.container().template getMember<float>("c");
        if(c)
        {
            valueView.m_data->c = *c;
        }

        const auto floats = valueView.container().template getMember<std::vector<float>>("floats");
        if(floats)
        {
            valueView.m_data->floats = *floats;
        }

        const auto myVec3 = valueView.container().template getMember<glm::vec3>("myVec3");
        if(myVec3)
        {
            valueView.m_data->myVec3 = *myVec3;
        }

        const auto unMap = valueView.container().template getMember<decltype(Derived0::unMap)>("unMap");
        if(unMap)
        {
            valueView.m_data->unMap = *unMap;
        }

        auto derived = valueView.container().template getMember<decltype(Derived0::m_derived)>("m_derived");
        if(derived)
        {
            valueView.m_data->m_derived = std::move(*derived);
        }

        auto base = valueView.container().template getMember<Base>("m_base");
        if(base)
        {
            valueView.m_data->m_base = std::move(*base);
        }

        std::printf("derived0 deserializing\n");
    }
};

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<Derived, TFormatType> : SGCore::Serde::BaseTypes<Base>, SGCore::Serde::DerivedTypes<Derived0, Derived1>
{
    static inline const std::string type_name = "Derived";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const Derived, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("b", valueView.m_data->b);
        valueView.container().addMember("str0", valueView.m_data->str0);
        valueView.container().addMember("str1", valueView.m_data->str1);
        std::printf("derived serializing\n");
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<Derived, TFormatType>& valueView) noexcept
    {
        const auto b = valueView.container().template getMember<float>("b");
        if(b)
        {
            valueView.m_data->b = *b;
        }

        const auto str0 = valueView.container().template getMember<std::string>("str0");
        if(str0)
        {
            valueView.m_data->str0 = *str0;
        }

        const auto str1 = valueView.container().template getMember<std::basic_string<char16_t>>("str1");
        if(str1)
        {
            valueView.m_data->str1 = *str1;
        }

        std::printf("derived deserializing\n");
    }
};

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<Base, TFormatType> : SGCore::Serde::DerivedTypes<Derived>
{
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const Base, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("a", valueView.m_data->a);
        std::printf("base serializing\n");
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<Base, TFormatType>& valueView) noexcept
    {
        const auto a = valueView.container().template getMember<std::int32_t>("a");
        if(a)
        {
            valueView.m_data->a = *a;
        }
        std::printf("base deserializing\n");
    }
};

void coreInit();

void onUpdate(const double& dt, const double& fixedDt);
void onFixedUpdate(const double& dt, const double& fixedDt);

int main();

#endif //SUNGEARENGINE_ENTRY_H
