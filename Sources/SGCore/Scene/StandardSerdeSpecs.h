//
// Created by Ilya on 18.08.2024.
//

#ifndef SUNGEARENGINE_STANDARDSERDESPECS_H
#define SUNGEARENGINE_STANDARDSERDESPECS_H

#include "Serializer.h"

namespace SGCore::Serde
{
    // =========================================================================================
    // STANDARD SerdeSpec IMPLEMENTATIONS
    // =========================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::unique_ptr<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        static void serialize(SerializableValueView<std::unique_ptr<T>, TFormatType>& valueView)
        {
            if(!*valueView.m_data)
            {
                valueView.getValueContainer().setAsNull();
                return;
            }

            SerializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = valueView.m_data->get();

            // serializing values of T
            SerdeSpec<T, TFormatType>::serialize(tmpView);
        }

        static void deserialize(DeserializableValueView<std::unique_ptr<T>, TFormatType>& valueView)
        {
            // if value is null then doing nothing
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            // allocating object
            *valueView.m_data = std::make_unique<T>();

            // creating temporary value view that contains object with type T
            DeserializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = valueView.m_data->get();

            // deserializing values of T
            SerdeSpec<T, TFormatType>::deserialize(tmpView);
        }

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView)
        {
            return (*valueView.m_data).get();
        }

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
            *valueView.m_data = std::move(std::unique_ptr<T0>(pointer));
        }
    };

    // ====================================================================================

    template<std::floating_point FloatingT, FormatType TFormatType>
    struct SerdeSpec<FloatingT, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "float";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<FloatingT, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsFloat(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<FloatingT, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().getAsFloat();
        }
    };

    // ====================================================================================

    template<std::integral IntegralT, FormatType TFormatType>
    struct SerdeSpec<IntegralT, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "int";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<IntegralT, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<IntegralT, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().getAsInt64();
        }
    };

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::vector<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::vector";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<std::vector<T>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(const auto& v : *valueView.m_data)
            {
                valueView.getValueContainer().pushBack(v);
            }
        }

        static void deserialize(DeserializableValueView<std::vector<T>, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().template getAsArray<T>();
        }
    };
}

#endif //SUNGEARENGINE_STANDARDSERDESPECS_H
