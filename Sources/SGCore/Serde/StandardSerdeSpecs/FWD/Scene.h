//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Scene/Scene.h"

namespace SGCore
{
    struct AssetsPackage;
    class Node;
    struct Layer;
    struct EntityRef;
}

namespace SGCore::Serde
{
    // ======================================================== EntityRef FWD

    template<FormatType TFormatType>
    struct SerdeSpec<EntityRef, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::EntityRef";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const EntityRef, TFormatType>& valueView,
                              const ECS::entity_t& deserializedEntity,
                              ECS::registry_t& toRegistry) noexcept;

        static void deserialize(DeserializableValueView<EntityRef, TFormatType>& valueView,
                                const ECS::entity_t& deserializedEntity,
                                ECS::registry_t& toRegistry) noexcept;
    };

    // ======================================================== Layer FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Layer, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Layer";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Layer, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Layer, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ECS::entity_t FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ECS::entity_t, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ECS::entity_t";
        static inline constexpr bool is_pointer_type = false;

        // behaviour as just enum value
        static void serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView) noexcept;

        // behaviour as just enum value
        static void deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView) noexcept;

        // behaviour as serializable entity for scene
        static void serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView, const Scene& serializableScene) noexcept;

        // behaviour as deserializable entity for scene
        static void deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView, Scene& deserializableScene) noexcept;
    };

    // ======================================================== ECS::registry_t FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ECS::registry_t, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ECS::registry_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ECS::registry_t, TFormatType>& valueView, const Scene& serializableScene) noexcept;

        static void deserialize(DeserializableValueView<ECS::registry_t, TFormatType>& valueView, Scene& serializableScene) noexcept;
    };

    // ======================================================== ECS::systems_container_t FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Scene::systems_container_t, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Scene::systems_container_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Scene::systems_container_t, TFormatType>& valueView, const Scene& serializableScene) noexcept;

        static void deserialize(DeserializableValueView<Scene::systems_container_t, TFormatType>& valueView) noexcept;
    };

    // ======================================================== SceneMetaInfo FWD

    template<FormatType TFormatType>
    struct SerdeSpec<SceneMetaInfo, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SceneMetaInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SceneMetaInfo, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<SceneMetaInfo, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Scene FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Scene, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        // sg_validate_serdespec_supported_formats(TFormatType, FormatType::JSON, FormatType::BSON)

        static inline const std::string type_name = "SGCore::Scene";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Scene, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Scene, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Node FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Node, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Node";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Node, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<Node, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };
}
