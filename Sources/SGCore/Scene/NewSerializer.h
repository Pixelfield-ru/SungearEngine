//
// Created by Ilya on 14.08.2024.
//

#ifndef SUNGEARENGINE_NEWSERIALIZER_H
#define SUNGEARENGINE_NEWSERIALIZER_H

#include "SGCore/Utils/TypeTraits.h"

namespace SGCore::NewSerde
{
    enum class FormatType
    {
        JSON,
        BSON,
        YAML
    };

    // DO NOT STORE ANYWHERE
    // DOES NOT OWN ANYTHING
    template<typename T>
    struct IDeserializableValueView
    {

    };

    template<typename T>
    struct JSONSerializableValueView;

    struct ISerializableValueViewFormatPointers
    {
        virtual void copyFormatPointers(ISerializableValueViewFormatPointers* to) const noexcept = 0;
    };

    struct JSONSerializableValueViewFormatPointers final : ISerializableValueViewFormatPointers
    {
        void copyFormatPointers(ISerializableValueViewFormatPointers* to) const noexcept final
        {
            to->m_document = m_document;
        }

    protected:
        rapidjson::Document* m_document { };
    };

    // DO NOT STORE ANYWHERE
    // DOES NOT OWN ANYTHING
    template<typename T>
    struct ISerializableValueView
    {
        friend struct Serializer;

        const T* m_data { };

        [[nodiscard]] FormatType getFormatType() const noexcept
        {
            return m_formatType;
        }

        template<typename T0>
        void copyFormatPointers(ISerializableValueView<T0>& to) noexcept
        {
            assert(to.getFormatType() == m_formatType &&
                   "SGCore::Serde::ISerializableValueView::copyFormatPointers: to format and from format types are not equal.");

            switch(to.getFormatType())
            {
                case FormatType::JSON:
                    copyFormatPointersImpl(static_cast<JSONSerializableValueView<T0>&>(to));
                    break;
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }
        }

    protected:
        virtual void copyFormatPointersImpl(ISerializableValueView<T>& to) noexcept = 0;

        FormatType m_formatType = FormatType::JSON;
        Ref<ISerializableValueView<T>> m_parent;
    };

    template<typename T>
    struct JSONSerializableValueView final : public ISerializableValueView<T>
    {
        friend struct Serializer;

        JSONSerializableValueView()
        {
            this->m_formatType = FormatType::JSON;
        }

    protected:
        void copyFormatPointersImpl(ISerializableValueView<T>& to) noexcept final
        {
            //to.m_document = m_document;

            std::printf("called json copyFormatPointersImpl\n");
        }
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    // USE THAT STRUCTS TO DEFINE BASE CLASSES OF SERIALIZABLE TYPE AND DERIVED CLASSES
    template<typename... Cls>
    struct BaseTypes
    {
        static constexpr std::uint64_t base_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_base_type = extract<Idx, Cls...>::type;
    };

    template<typename... Cls>
    struct DerivedTypes
    {
        static constexpr std::uint64_t derived_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_derived_type = extract<Idx, Cls...>::type;
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    template<typename T>
    struct SerdeSpec : BaseTypes<>, DerivedTypes<>
    {
        static_assert(always_false<T>::value,
                      "This type is not serializable. Please, implement specialization of SerdeSpec for this type.");

        // the type name of T
        static inline const std::string type_name = "unknown";
        // is T is pointer type
        static inline constexpr bool is_pointer_type = false;

        static void serialize(ISerializableValueView<T>& valueView)
        {

        }

        static void deserialize(IDeserializableValueView<T>& valueView)
        {

        }
    };

    struct Serializer
    {
        template<typename T>
        static void toFormat(T& value, FormatType formatType = m_defaultFormatType) noexcept
        {
            switch (m_defaultFormatType)
            {
                case FormatType::JSON:
                {
                    rapidjson::Document document;
                    document.SetObject();

                    toJSON(document, value);
                    break;
                }
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }
        }

        template<typename T>
        static void serializeMember(ISerializableValueView<T>& parent,
                                    const std::string& varName,
                                    const T& value) noexcept
        {

        }

        template<typename T>
        static Ref<ISerializableValueView<T>> createSerializableValueView(FormatType formatType) noexcept
        {
            switch (formatType)
            {
                case FormatType::JSON:
                    return MakeRef<JSONSerializableValueView<T>>();
                case FormatType::BSON:
                    return nullptr;
                case FormatType::YAML:
                    return nullptr;
            }

            return nullptr;
        }

    private:
        static inline FormatType m_defaultFormatType = SGCore::NewSerde::FormatType::JSON;

        template<typename T>
        static void toJSON(rapidjson::Document& toDocument,
                           const T& value)
        {
            toDocument.AddMember("version", 1, toDocument.GetAllocator());

            rapidjson::Value typeNameSectionValue(rapidjson::kStringType);
            typeNameSectionValue.SetString(SerdeSpec<T>::type_name.c_str(), SerdeSpec<T>::type_name.length());
            toDocument.AddMember("typeName", typeNameSectionValue, toDocument.GetAllocator());

            rapidjson::Value valueSectionValue(rapidjson::kObjectType);

            // ==== serialization code

            auto valueView = MakeRef<JSONSerializableValueView<T>>();
            valueView->m_data = &value;
            valueView->m_document = &toDocument;

            SerdeSpec<T>::serialize(*valueView);

            // =======================

            toDocument.AddMember("value", valueSectionValue, toDocument.GetAllocator());
        }
    };

    template<typename T>
    struct SerdeSpec<std::unique_ptr<T>> : BaseTypes<>, DerivedTypes<>
    {
        // the type name of T
        static inline const std::string type_name = SerdeSpec<T>::type_name;
        // is T is pointer type
        static inline constexpr bool is_pointer_type = true;

        static void serialize(ISerializableValueView<std::unique_ptr<T>>& valueView)
        {
            if(!*valueView.m_data)
            {
                // TODO: serialize nullptr
                return;
            }

            auto tmpView = Serializer::createSerializableValueView<T>(valueView.getFormatType());
            valueView.template copyFormatPointers<T>(*tmpView);
            tmpView->m_data = valueView.m_data->get();

            // todo: serializing values of T
            SerdeSpec<T>::serialize(*tmpView);
        }

        static void deserialize(IDeserializableValueView<std::unique_ptr<T>>& valueView)
        {

        }
    };
}

#endif //SUNGEARENGINE_NEWSERIALIZER_H
