//
// Created by Ilya on 18.08.2024.
//

#ifndef SUNGEARENGINE_STANDARDSERDESPECS_H
#define SUNGEARENGINE_STANDARDSERDESPECS_H

#include "Serde.h"
#include <glm/glm.hpp>
#include <SGCore/Logger/Logger.h>
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"

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

            if constexpr(!std::is_abstract_v<T>)
            {
                // allocating object
                *valueView.m_data = std::make_unique<T>();
            }

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

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::shared_ptr<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        static void serialize(SerializableValueView<std::shared_ptr<T>, TFormatType>& valueView)
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

        static void deserialize(DeserializableValueView<std::shared_ptr<T>, TFormatType>& valueView)
        {
            // if value is null then doing nothing
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            if constexpr(!std::is_abstract_v<T>)
            {
                // allocating object
                *valueView.m_data = std::make_shared<T>();
            }

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
            *valueView.m_data = std::move(std::shared_ptr<T0>(pointer));
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<T*, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        static void serialize(SerializableValueView<T*, TFormatType>& valueView)
        {
            // if value is null then doing nothing
            if(!*valueView.m_data)
            {
                valueView.getValueContainer().setAsNull();
                return;
            }

            SerializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = *valueView.m_data;

            // serializing values of T
            SerdeSpec<T, TFormatType>::serialize(tmpView);
        }

        static void deserialize(DeserializableValueView<T*, TFormatType>& valueView)
        {
            // if value is null then doing nothing
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            if constexpr(!std::is_abstract_v<T>)
            {
                // allocating object
                *valueView.m_data = new T();
            }

            // creating temporary value view that contains object with type T
            DeserializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = *valueView.m_data;

            // deserializing values of T
            SerdeSpec<T, TFormatType>::deserialize(tmpView);
        }

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView)
        {
            return (*valueView.m_data);
        }

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
            *valueView.m_data = pointer;
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

    // ====================================================================================

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

    // ====================================================================================

    template<typename CharT, FormatType TFormatType>
    struct SerdeSpec<std::basic_string<CharT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::basic_string";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<std::basic_string<CharT>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsString(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<std::basic_string<CharT>, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().template getAsString<CharT>();
        }
    };

    // ====================================================================================

    template<glm::length_t Length, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::vec<Length, T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::vec";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(std::size_t i = 0; i < Length; ++i)
            {
                valueView.getValueContainer().pushBack((*valueView.m_data)[i]);
            }
        }

        static void deserialize(DeserializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            for(std::size_t i = 0; i < Length; ++i)
            {
                (*valueView.m_data)[i] = vec[i];
            }
        }
    };

    // ====================================================================================

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::mat<C, R, T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::mat";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(std::size_t c = 0; c < C; ++c)
            {
                for(std::size_t r = 0; r < R; ++r)
                {
                    valueView.getValueContainer().pushBack((*valueView.m_data)[c][r]);
                }
            }
        }

        static void deserialize(DeserializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            size_t i = 0;
            for(std::size_t c = 0; c < C; ++c)
            {
                for(std::size_t r = 0; r < R; ++r)
                {
                    (*valueView.m_data)[c][r] = vec[i];
                    ++i;
                }
            }
        }
    };

    // ====================================================================================

    /**
     * KeyT REQUIRES AN IMPLICIT CONVERSION OPERATOR TO std::string OR OTHER TYPES FROM
     * WHICH std::string CAN BE CONSTRUCTED OR WHICH CAN BE IMPLICITLY CONVERTED TO std::string
     *
    **/
    template<typename KeyT, typename ValueT, FormatType TFormatType>
    struct SerdeSpec<std::unordered_map<KeyT, ValueT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::unordered_map";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<std::unordered_map<KeyT, ValueT>, TFormatType>& valueView)
        {
            for(const auto& [key, value] : *valueView.m_data)
            {
                valueView.getValueContainer().addMember(key, value);
            }
        }

        static void deserialize(DeserializableValueView<std::unordered_map<KeyT, ValueT>, TFormatType>& valueView)
        {
            for(auto it = valueView.getValueContainer().memberBegin(); it != valueView.getValueContainer().memberEnd(); ++it)
            {
                const auto val = valueView.getValueContainer().template getMember<ValueT>(it);

                if(val)
                {
                    (*valueView.m_data)[valueView.getValueContainer().getMemberName(it)] = *val;
                }
            }
        }
    };

    // ====================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<bool, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "bool";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<bool, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsBool(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<bool, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().getAsBool();
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    requires(std::is_enum_v<T>)
    struct SerdeSpec<T, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "enum";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<T, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(static_cast<std::underlying_type_t<T>>(*valueView.m_data));
        }

        static void deserialize(DeserializableValueView<T, TFormatType>& valueView)
        {
            *valueView.m_data = (T) valueView.getValueContainer().getAsInt64();
        }
    };

    // ====================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<std::filesystem::path, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::filesystem::path";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<std::filesystem::path, TFormatType>& valueView)
        {
            const std::string u8Path = SGCore::Utils::toUTF8(valueView.m_data->u16string());
            valueView.getValueContainer().setAsString(u8Path);
        }

        static void deserialize(DeserializableValueView<std::filesystem::path, TFormatType>& valueView)
        {
            const std::u16string tmpPath = valueView.getValueContainer().template getAsString<char16_t>();
            *valueView.m_data = tmpPath;
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SceneEntitySaveInfo, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SceneEntitySaveInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<SceneEntitySaveInfo, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            valueView.m_data->m_savableScene->template getOnEntitySave<TFormatType>()(
                    *valueView.m_data->m_savableScene,
                    valueView.m_data->m_savableEntity,
                    valueView
            );
        }

        static void deserialize(DeserializableValueView<registry_t, TFormatType>& valueView)
        {
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<registry_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::registry_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<registry_t, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            Scene* savableScene { };

            // getting scene
            auto sceneEntitiesView = valueView.m_data->template view<Scene*>();
            sceneEntitiesView.each([&savableScene](Scene* scene) {
                savableScene = scene;
            });

            if(!savableScene)
            {
                LOG_E(SGCORE_TAG, "Can not save null scene!");
                return;
            }

            SceneEntitySaveInfo sceneEntitySaveInfo;
            sceneEntitySaveInfo.m_savableScene = savableScene;

            auto entitiesView = valueView.m_data->template view<EntityBaseInfo>();
            for(const auto& entity : entitiesView)
            {
                // if current savable entity has parent the skip saving this entity because parent saves children entities himself
                EntityBaseInfo* entityBaseInfo = savableScene->getECSRegistry()->try_get<EntityBaseInfo>(entity);
                if(entityBaseInfo &&
                   entityBaseInfo->getParent() != entt::null &&
                   savableScene->getECSRegistry()->valid(entityBaseInfo->getParent()))
                {
                    continue;
                }

                LOG_I(SGCORE_TAG, "Saving entity '{}'...", std::to_underlying(entity));

                sceneEntitySaveInfo.m_savableEntity = entity;

                valueView.getValueContainer().pushBack(sceneEntitySaveInfo);
            }

            Scene::getOnSceneSavedEvent()(*savableScene);
        }

        static void deserialize(DeserializableValueView<registry_t, TFormatType>& valueView)
        {
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Scene";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<Scene, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_ecsRegistry", *valueView.m_data->m_ecsRegistry);
        }

        static void deserialize(DeserializableValueView<Scene, TFormatType>& valueView)
        {
        }
    };
}

#endif //SUNGEARENGINE_STANDARDSERDESPECS_H
