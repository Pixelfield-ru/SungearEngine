#pragma once
#include <expected>

#include "SGCore/Utils/TypeTraits.h"

#include "ImplDeserializable.h"
// #include "ImplDeserializableStruct.h"
#include "IsValidContainer.h"
#include "Scope.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/Utils/IterTuple.h"
#include "MetaDefenition.h"
#include "SGCore/Main/CoreMain.h"

namespace SGCore::UI::Deserialization
{
    using DeserializeIntoResultType = std::optional<std::string>;

    template<typename T>
    struct TypePrinter;

    template<typename T>
    struct Deserializer
    {
        /**
         * Deserializes @ref value into @ref field
         * @param value Value, that should be parsed into a field
         * @param field Field, where values should be stored (maybe initialized by default)
         * @pagram scope Holds variables and more
         * @return error message, if parsing was unsuccessful, @ref std::nullopt otherwise
         */
        static DeserializeIntoResultType deserializeInto([[maybe_unused]] UISourceTreeViewValue& value, [[maybe_unused]] T& field, [[maybe_unused]] DeserScope scope) {
            std::cout << "ERR " << typeid(T).name() << std::endl;
            return std::format("Type {} aren't supported. Implement SGCore::UI::Deserialization::Deserializer specialization or SGCore::UI::Deserialization::ImplDeserializable trait for type", typeid(T).name());
        }
    };

    template<typename Deserializable>
    struct Deserializer<Deserializable&>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Deserializable& field, DeserScope scope) {
            return Deserializer<Deserializable>::deserializeInto(value, field, scope);
        }
    };


    template<typename T>
    DeserializeIntoResultType resolve(UISourceTreeViewReference& ref, T& into, DeserScope& scope) {
        if (const auto val = scope.tryGetStatic(ref.getPath())) {
            return Deserializer<T>::deserializeInto(val->get(), into, scope);
        }

        return std::format("Unknown reference \"{}\"", ref.getPath());
    }

    template<>
    struct Deserializer<std::string>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, std::string& field, DeserScope scope) {
            if (auto ref = value.tryGetReference()) {
                return resolve<std::string>(*ref, field, scope);
            }
            const std::optional<std::string_view> val = value.tryGetString();
            if (!val.has_value()) {
                return std::format("Expected std::string");
            }
            field = std::string(*val);
            return std::nullopt;
        }
    };

    template<>
    struct Deserializer<int>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, int& field, DeserScope scope) {
            const std::optional<int> val = value.tryGetInt();
            if (!val.has_value()) {return std::format("Expected int");}
            field = *val;
            return std::nullopt;
        }
    };

    template<typename T>
    struct MemberType;

    template<typename T, typename U>
    struct MemberType<T U::*> {
        using type = T;
    };

    template<typename Deserializable>
    concept ImplementsManualDeserialization = requires() {
        {
            MetaDef<Deserializable>::deserializeInto(
                std::declval<UISourceTreeViewValue&>(),
                std::declval<Deserializable&>(),
                std::declval<DeserScope>())
        } -> std::convertible_to<DeserializeIntoResultType>;
    };

    template<typename Deserializable> requires
        ImplDeserializable<Deserializable> && ImplementsManualDeserialization<Deserializable>
    struct Deserializer<Deserializable>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Deserializable& field, DeserScope scope) {
            return MetaDef<Deserializable>::deserializeInto(value, field, scope);
        }
    };

    template<typename Container> requires
        IsValidContainer<Container>::value
        && (!ImplDeserializableContainer<Container>)
        && ImplDeserializable<typename IsValidContainer<Container>::ValueType_t>
    struct Deserializer<Container>
    {
        using ValueType = IsValidContainer<Container>::ValueType_t;

        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Container& field, DeserScope scope) {
            return Deserializer<ValueType>::deserializeInto(value, IsValidContainer<Container>::prepare(field).get(), scope);
        }

        using _is_shared_container_specialization = std::true_type;
    };

    template<typename Container> requires
        IsValidContainer<Container>::value
        && ImplDeserializableContainer<Container>
    struct Deserializer<Container>
    {
        using ValueType = IsValidContainer<Container>::ValueType_t;

        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Container& field, DeserScope scope) {
            return MetaDef<ValueType>::template deserializeIntoContainer<Container>(value, field, scope);
        }
    };

    template<typename T>
constexpr std::string_view fullTypeName() {
#if defined(__clang__) || defined(__GNUC__)
        return __PRETTY_FUNCTION__; // например: "constexpr std::string_view fullTypeName() [with T = const int&]"
#elif defined(_MSC_VER)
        return __FUNCSIG__;         // например: "std::string_view __cdecl fullTypeName<const int &>(void)"
#endif
    }

    template<typename Deserializable> requires
        ImplDeserializable<Deserializable>
        && (!ImplementsManualDeserialization<Deserializable>)
        && (!IsValidContainer<Deserializable>::value)
    struct Deserializer<Deserializable>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Deserializable& field, DeserScope scope) {
            // Try to parse self as object
            // TODO: support "component singleValue;"
            auto valueAsMaybeObject = value.tryGetObject();
            if (!valueAsMaybeObject) { return "wrong object type"; }

            auto& object = *valueAsMaybeObject;

            using Meta = MetaDef<Deserializable>;

            if constexpr(requires { Meta::base_types; })
            {
                if (auto result = for_types_until([&](auto wrapper) {
                    // std::cout << "!!!" << typeid(decltype(wrapper)::type).name() << std::endl;
                    return Deserializer<typename decltype(wrapper)::type>::deserializeInto(value, field, scope);
                }, Meta::base_types)) {
                    return result;
                }
            }

            if constexpr(
                requires { Meta::properties_fields; }
            ) {
                if constexpr(
                    std::tuple_size<decltype(Meta::properties_fields)>() != 0 // tuple_iter_until can't hundle empty tuples
                ) {
                    auto& iter = object.properties();

                    for (auto& prop : iter)
                    {
                        auto propName = prop.getName();

                        auto deserializeFunc = [&](auto& memberDesc) {
                            if (propName == memberDesc.second)
                            {
                                using prop_t = decltype(field.*(memberDesc.first));

                                if (auto error = Deserializer<prop_t>::deserializeInto(prop.getValue(), field.*memberDesc.first, scope))
                                {
                                    LOG_E("SGCore", "UNCATCHED ERROR: {}", std::format("Error when parsing field {}.{}, error: {}", typeid(Deserializable).name(), memberDesc.second, *error));
                                    // return std::format("Error when parsing field {}.{}, error: {}", typeid(Deserializable).name(), memberDesc.second, *error);
                                }
                            }
                            // return std::nullopt;
                        };

                        std::apply([&]<typename... MemberDesc>(MemberDesc&&... memberDesc) {
                            (deserializeFunc(std::forward<MemberDesc>(memberDesc)), ...);
                        }, Meta::properties_fields);

                        // That shit doesnt work please fix (iter_tuple_unit is broken)
                        /*auto result = iter_tuple_until([&](auto member) {
                            using prop_t = decltype((field.*member).first);
                            if (member.second == propName) {

                                if (auto error = Deserializer<prop_t>::deserializeInto(prop.getValue(), field.*member.first, scope))
                                {
                                    return std::optional(
                                        std::format(
                                            "Error when parsing field {}.{}, error: {}",
                                            typeid(Deserializable).name(),
                                            member.second,
                                            *error
                                        ));
                                }

                                return std::optional<std::string>();
                            }
                            return std::optional(std::format("Property {} not found", member.second));
                        }, Meta::properties_fields);

                        if (result) {
                            return result;
                            continue;
                        }
                        */
                        return std::format("Unknown property {}", propName);
                    }
                }
            }

            if constexpr(requires {
                Meta::children_field;
            } )
            {
                using children_field_type = std::remove_reference_t<decltype(field.*Meta::children_field)>;
                auto& children = object.children();

                if constexpr(requires() {
                    typename children_field_type::value_type;
                    std::declval<children_field_type>().push_back(std::declval<typename children_field_type::value_type>());
                })
                {
                    using ValueType = children_field_type::value_type;

                    for (auto& child : children)
                    {
                        (field.*Meta::children_field).push_back({});

                        if (auto result = Deserializer<ValueType>::deserializeInto(child, *(field.*Meta::children_field).rbegin(), scope)) {
                            return result;
                        }
                    }
                }
                else
                { // TODO: add support for range constructor
                    if (children.end().m_index != 1) {
                        return std::format("Type {} can accept only one child", typeid(children_field_type).name());
                    }
                    if (auto result = Deserializer<children_field_type>::deserializeInto(children[0], field.*Meta::children_field, scope)) {
                        return result;
                    }
                }
            }

            return std::nullopt;

            // return Deserialization::deserializeInto(value, field, scope);
        }
    };


}
