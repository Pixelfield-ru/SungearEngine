//
// Created by stuka on 22.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

namespace SGCore
{
    struct SkeletalBoneAnimation;
    struct MeshBoneData;
    struct AudioTrackAsset;
    struct ByteFileAsset;
    struct SkeletalAnimationAsset;
    struct Bone;
    struct AnimationsFile;
    struct Skeleton;
    class ModelAsset;
}

namespace SGCore::Serde
{
    // ======================================================== IAsset FWD

    template<FormatType TFormatType>
    struct SerdeSpec<IAsset, TFormatType> :
            BaseTypes<>,
            DerivedTypes<
                ITexture2D,
                TextFileAsset,
                ShaderAnalyzedFile,
                ModelAsset,
                IMaterial,
                IMeshData,
                IShader,
                Skeleton,
                AnimationsFile,
                Bone,
                SkeletalAnimationAsset,
                ByteFileAsset
            >
    {
        static inline const std::string type_name = "SGCore::IAsset";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const IAsset, TFormatType>& valueView, SharedDataT&&...) noexcept;

        /// This function is used only when deserializing an asset manager package.
        static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        /// This function is used when deserializing directly from file that contains only this asset.
        static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ITexture2D FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ITexture2D, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<ICubemapTexture>
    {
        /// We indicate the type we are working with.
        static inline const std::string type_name = "SGCore::ITexture2D";
        /// ITexture2D is not pointer type. That is, ITexture2D is not a type that uses RAII to store the pointer and to wrap it.
        static inline constexpr bool is_pointer_type = false;

        /// Required function: This function is ONLY used when serializing assets into a package.\n
        /// You can implement serialize function that does not accept AssetsPackage for serializing asset directly to file.\n
        /// The second argument is the current package being serialized.
        static void serialize(SerializableValueView<const ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        /// You should never implement this function.
        // static void serialize(SerializableValueView<const ITexture2D, TFormatType>& valueView);

        /// Required function: This function is ONLY used when deserializing assets from a package.\n\n
        /// The second argument is the current package being deserialized.\n\n
        /// This function never deserializes heavy data,
        /// but deserializes the markup of heavy data (offsets and sizes) to write to variables of the currently deserialized asset,
        /// in order to later lazily deserialize the heavy asset data.\n\n
        /// The heavy data of the current asset will be deserialized when this asset is requested from the asset manager.\n
        /// If the current asset was requested from the asset manager, and the heavy data was not deserialized earlier,
        /// then the virtual function \p loadFromBinaryFile(...) is called for the requested asset if
        /// the asset was previously serialized to a binary file or \p load(...) if it was not previously serialized to a binary file,
        /// which means loading by the path to the asset (.gltf, .obj).\n\n
        /// Each implementation of the \p loadFromBinaryFile(...) function must load heavy data by offsets and sizes from a binary file.
        /// So for each member with heavy data in your class inheriting IAsset you must store the offset in the binary file (in bytes) and the size of the data in bytes.\n\n
        /// You can implement \p deserialize function that does not accept \p AssetsPackage for serializing asset directly to file.
        /// \param valueView
        /// \param assetsPackage
        static void deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        /// You should never implement this function.
        // static void deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView);

        static ITexture2D* allocateObject(DeserializableValueView<ITexture2D, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ByteFileAsset FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ByteFileAsset, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<AudioTrackAsset>
    {
        static inline const std::string type_name = "SGCore::ByteFileAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ByteFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<ByteFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== AudioTrackAsset FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AudioTrackAsset, TFormatType> :
            BaseTypes<ByteFileAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AudioTrackAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AudioTrackAsset, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AudioTrackAsset, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ICubemapTexture FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ICubemapTexture, TFormatType> :
            BaseTypes<ITexture2D>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ICubemapTexture";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static ICubemapTexture* allocateObject(DeserializableValueView<ICubemapTexture, TFormatType>& valueView) noexcept;
    };

    // ======================================================== TextFileAsset FWD

    template<FormatType TFormatType>
    struct SerdeSpec<TextFileAsset, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TextFileAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== IShader FWD

    template<FormatType TFormatType>
    struct Serde::SerdeSpec<IShader, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IShader";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IShader, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<IShader, TFormatType>& valueView) noexcept;

        static IShader* allocateObject(DeserializableValueView<IShader, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ShaderAnalyzedFile FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ShaderAnalyzedFile, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ShaderAnalyzedFile";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== SGSLESubShader FWD

    template<FormatType TFormatType>
    struct SerdeSpec<SGSLESubShader, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SGSLESubShader";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== IMaterial FWD

    template<FormatType TFormatType>
    struct SerdeSpec<IMaterial, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IMaterial";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const IMaterial, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<IMaterial, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== IMeshData FWD

    template<FormatType TFormatType>
    struct SerdeSpec<IMeshData, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IMeshData";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static IMeshData* allocateObject(DeserializableValueView<IMeshData, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ModelAsset FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ModelAsset, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ModelAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== Skeleton FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Skeleton, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Skeleton";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Skeleton, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<Skeleton, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== Bone FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Bone, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Bone";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Bone, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<Bone, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== MeshBoneData FWD

    template<FormatType TFormatType>
    struct SerdeSpec<MeshBoneData, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MeshBoneData";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MeshBoneData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<MeshBoneData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== AnimationsFile FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AnimationsFile, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AnimationsFile";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AnimationsFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<AnimationsFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== SkeletalAnimationAsset FWD

    template<FormatType TFormatType>
    struct SerdeSpec<SkeletalAnimationAsset, TFormatType> :
            BaseTypes<IAsset>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SkeletalAnimationAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SkeletalAnimationAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<SkeletalAnimationAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== SkeletalBoneAnimation FWD

    template<FormatType TFormatType>
    struct SerdeSpec<SkeletalBoneAnimation, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SkeletalBoneAnimation";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SkeletalBoneAnimation, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;

        static void deserialize(DeserializableValueView<SkeletalBoneAnimation, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept;
    };

    // ======================================================== AssetRef FWD

    template<typename AssetT, FormatType TFormatType>
    struct SerdeSpec<AssetRef<AssetT>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AssetRef";
        static inline constexpr bool is_pointer_type = false;

        // WE ARE SERIALIZING ONLY META INFO OF ASSET BECAUSE IT IS ASSET REFERENCE. WE DO NOT NEED TO DO SERIALIZATION OF DATA
        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const AssetRef<AssetT>, TFormatType>& valueView, SharedDataT&&...) noexcept;

        // WE ARE DESERIALIZING ONLY META INFO OF ASSET BECAUSE IT IS ASSET REFERENCE. WE DO NOT NEED TO DO DESERIALIZATION OF DATA
        static void deserialize(DeserializableValueView<AssetRef<AssetT>, TFormatType>& valueView, AssetsPackage&) noexcept;

        static void deserialize(DeserializableValueView<AssetRef<AssetT>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== AssetsPackage::DataMarkup FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AssetsPackage::DataMarkup, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AssetsPackage::DataMarkup";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AssetsPackage::DataMarkup, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AssetsPackage::DataMarkup, TFormatType>& valueView) noexcept;
    };
}
