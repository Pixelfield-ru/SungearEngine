//
// Created by stuka on 22.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Memory/Assets/AnimationsFile.h"
#include "SGCore/Memory/Assets/ByteFileAsset.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "SGCore/Render/Alpha/MaterialTransparencyType.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Graphics/ShaderTextureBinding.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"

namespace SGCore::Serde
{
    // =============================================== impl IAsset

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<IAsset, TFormatType>::serialize(SerializableValueView<const IAsset, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        valueView.getValueContainer().addMember("m_path", valueView.m_data->getPath());
        valueView.getValueContainer().addMember("m_alias", valueView.m_data->getAlias());
        valueView.getValueContainer().addMember("m_storedBy", valueView.m_data->storedByWhat());
        valueView.getValueContainer().addMember("m_useDataSerde", true);
    }

    template<FormatType TFormatType>
    void SerdeSpec<IAsset, TFormatType>::deserialize(DeserializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
        if(assetAlias)
        {
            valueView.m_data->m_alias = std::move(*assetAlias);
        }

        auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
        if(assetPath)
        {
            valueView.m_data->m_path = std::move(*assetPath);
        }

        const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
        if(assetPath)
        {
            valueView.m_data->m_storedBy = *assetStorageType;
        }

        const auto useDataSerde = valueView.getValueContainer().template getMember<bool>("m_useDataSerde");
        if(useDataSerde)
        {
            valueView.m_data->m_isSavedInBinaryFile = *useDataSerde;
        }
        else
        {
            valueView.m_data->m_isSavedInBinaryFile = false;
        }

        valueView.m_data->m_parentAssetManager = assetsPackage.getParentAssetManager()->shared_from_this();
    }

    template<FormatType TFormatType>
    void SerdeSpec<IAsset, TFormatType>::deserialize(DeserializableValueView<IAsset, TFormatType>& valueView) noexcept
    {
        auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
        if(assetAlias)
        {
            valueView.m_data->m_alias = std::move(*assetAlias);
        }

        auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
        if(assetPath)
        {
            valueView.m_data->m_path = std::move(*assetPath);
        }

        const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
        if(assetPath)
        {
            valueView.m_data->m_storedBy = *assetStorageType;
        }

        const auto useDataSerde = valueView.getValueContainer().template getMember<bool>("m_useDataSerde");
        if(useDataSerde)
        {
            valueView.m_data->m_isSavedInBinaryFile = *useDataSerde;
        }
        else
        {
            valueView.m_data->m_isSavedInBinaryFile = false;
        }
    }

    // =============================================== impl ITexture2D

    template<FormatType TFormatType>
    void SerdeSpec<ITexture2D, TFormatType>::serialize(SerializableValueView<const ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        /// Next, we serialize the heavy data (in this case, \p m_data )
        /// into a binary package file and get the output markup,
        /// which indicates the position of the \p m_data data in the binary file,
        /// as well as the size of \p m_data in bytes in the binary file.
        AssetsPackage::DataMarkup textureDataMarkup =
                assetsPackage.addData(valueView.m_data->m_textureData.get(),
                                      valueView.m_data->m_width * valueView.m_data->m_height *
                                      valueView.m_data->m_channelsCount
                );

        /// Next, we write the markup of heavy data into a JSON file for further deserialization of heavy data from a binary file.
        valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
        valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);

        /// We simply add lightweight data directly to the JSON file.
        valueView.getValueContainer().addMember("m_width", valueView.m_data->m_width);
        valueView.getValueContainer().addMember("m_height", valueView.m_data->m_height);
        valueView.getValueContainer().addMember("m_channelsCount", valueView.m_data->m_channelsCount);
        valueView.getValueContainer().addMember("m_internalFormat", valueView.m_data->m_internalFormat);
        valueView.getValueContainer().addMember("m_format", valueView.m_data->m_format);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ITexture2D, TFormatType>::deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        /// We just deserialize lightweight data because it is always in the JSON file (we always serialize lightweight data).
        auto width = valueView.getValueContainer().template getMember<std::int32_t>("m_width");
        auto height = valueView.getValueContainer().template getMember<std::int32_t>("m_height");
        auto channelsCount = valueView.getValueContainer().template getMember<int>("m_channelsCount");
        auto internalFormat = valueView.getValueContainer().template getMember<SGGColorInternalFormat>("m_internalFormat");
        auto format = valueView.getValueContainer().template getMember<SGGColorFormat>("m_format");

        /// Next we deserialize the required offsets and sizes of heavy data.
        auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
        auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

        /// We then assign the deserialized data to our current deserializing asset.

        if(dataOffsetOpt)
        {
            valueView.m_data->m_textureDataOffsetInPackage = *dataOffsetOpt;
        }

        if(dataSizeInBytesOpt)
        {
            valueView.m_data->m_textureDataSizeInPackage = *dataSizeInBytesOpt;
        }

        if(width)
        {
            valueView.m_data->m_width = *width;
        }

        if(height)
        {
            valueView.m_data->m_height = *height;
        }

        if(channelsCount)
        {
            valueView.m_data->m_channelsCount = *channelsCount;
        }

        if(internalFormat)
        {
            valueView.m_data->m_internalFormat = *internalFormat;
        }

        if(format)
        {
            valueView.m_data->m_format = *format;
        }
    }

    template<FormatType TFormatType>
    ITexture2D* SerdeSpec<ITexture2D, TFormatType>::allocateObject(DeserializableValueView<ITexture2D, TFormatType>& valueView) noexcept
    {
        return CoreMain::getRenderer()->createTexture2D();
    }

    // =============================================== impl ByteFileAsset

    template<FormatType TFormatType>
    void SerdeSpec<ByteFileAsset, TFormatType>::serialize(SerializableValueView<const ByteFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        const AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_dataBuffer, valueView.m_data->m_dataBufferSize);

        valueView.getValueContainer().addMember("m_dataMarkupInPackage", dataMarkup);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ByteFileAsset, TFormatType>::deserialize(DeserializableValueView<ByteFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        const auto dataMarkup = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>("m_dataMarkupInPackage");

        if(dataMarkup)
        {
            valueView.m_data->m_dataMarkupInPackage = *dataMarkup;
        }
    }

    // =============================================== impl AudioTrackAsset

    template<FormatType TFormatType>
    void SerdeSpec<AudioTrackAsset, TFormatType>::serialize(SerializableValueView<const AudioTrackAsset, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_audioTrackType", valueView.m_data->m_audioTrackType);
        valueView.getValueContainer().addMember("m_audioFormat", valueView.m_data->m_audioFormat);
        valueView.getValueContainer().addMember("m_numChannels", valueView.m_data->m_numChannels);
        valueView.getValueContainer().addMember("m_sampleRate", valueView.m_data->m_sampleRate);
        valueView.getValueContainer().addMember("m_byteRate", valueView.m_data->m_byteRate);
        valueView.getValueContainer().addMember("m_blockAlign", valueView.m_data->m_blockAlign);
        valueView.getValueContainer().addMember("m_bitsPerSample", valueView.m_data->m_bitsPerSample);
        valueView.getValueContainer().addMember("m_frequency", valueView.m_data->m_frequency);
    }

    template<FormatType TFormatType>
    void SerdeSpec<AudioTrackAsset, TFormatType>::deserialize(DeserializableValueView<AudioTrackAsset, TFormatType>& valueView) noexcept
    {
        const auto audioTrackType = valueView.getValueContainer().template getMember<AudioTrackType>("m_audioTrackType");
        if(audioTrackType)
        {
            valueView.m_data->m_audioTrackType = *audioTrackType;
        }

        const auto audioFormat = valueView.getValueContainer().template getMember<std::int16_t>("m_audioFormat");
        if(audioFormat)
        {
            valueView.m_data->m_audioFormat = *audioFormat;
        }

        const auto numChannels = valueView.getValueContainer().template getMember<std::int32_t>("m_numChannels");
        if(numChannels)
        {
            valueView.m_data->m_numChannels = *numChannels;
        }

        const auto sampleRate = valueView.getValueContainer().template getMember<std::int32_t>("m_sampleRate");
        if(sampleRate)
        {
            valueView.m_data->m_sampleRate = *sampleRate;
        }

        const auto byteRate = valueView.getValueContainer().template getMember<std::uint32_t>("m_byteRate");
        if(byteRate)
        {
            valueView.m_data->m_byteRate = *byteRate;
        }

        const auto blockAlign = valueView.getValueContainer().template getMember<std::uint16_t>("m_blockAlign");
        if(blockAlign)
        {
            valueView.m_data->m_blockAlign = *blockAlign;
        }

        const auto bitsPerSample = valueView.getValueContainer().template getMember<std::uint16_t>("m_bitsPerSample");
        if(bitsPerSample)
        {
            valueView.m_data->m_bitsPerSample = *bitsPerSample;
        }

        const auto frequency = valueView.getValueContainer().template getMember<std::uint32_t>("m_frequency");
        if(frequency)
        {
            valueView.m_data->m_frequency = *frequency;
        }
    }

    // =============================================== impl ICubemapTexture

    template<FormatType TFormatType>
    void SerdeSpec<ICubemapTexture, TFormatType>::serialize(SerializableValueView<const ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_parts", valueView.m_data->m_parts, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ICubemapTexture, TFormatType>::deserialize(DeserializableValueView<ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto parts = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_parts)>("m_parts", assetsPackage);
        if(parts)
        {
            valueView.m_data->m_parts = std::move(*parts);
        }
    }

    template<FormatType TFormatType>
    ICubemapTexture* SerdeSpec<ICubemapTexture, TFormatType>::allocateObject(DeserializableValueView<ICubemapTexture, TFormatType>& valueView) noexcept
    {
        return CoreMain::getRenderer()->createCubemapTexture();
    }

    // =============================================== impl TextFileAsset

    template<FormatType TFormatType>
    void SerdeSpec<TextFileAsset, TFormatType>::serialize(SerializableValueView<const TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        AssetsPackage::DataMarkup textureDataMarkup = assetsPackage.addData(valueView.m_data->m_data);

        valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
        valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
    }

    template<FormatType TFormatType>
    void SerdeSpec<TextFileAsset, TFormatType>::deserialize(DeserializableValueView<TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
        auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

        if(dataOffsetOpt)
        {
            valueView.m_data->m_dataOffsetInPackage = *dataOffsetOpt;
        }

        if(dataSizeInBytesOpt)
        {
            valueView.m_data->m_dataSizeInPackage = *dataSizeInBytesOpt;
        }
    }

    // =============================================== impl IShader

    template<FormatType TFormatType>
    void SerdeSpec<IShader, TFormatType>::serialize(SerializableValueView<const IShader, TFormatType>& valueView) noexcept
    {
        // valueView.getValueContainer().addMember("m_fileAssetPath", valueView.m_data->getFile()->getPath());
        valueView.getValueContainer().addMember("m_shaderAnalyzedFile", valueView.m_data->m_shaderAnalyzedFile.lock());
        valueView.getValueContainer().addMember("m_fileAsset", valueView.m_data->m_fileAsset.lock());
        valueView.getValueContainer().addMember("m_autoRecompile", valueView.m_data->m_autoRecompile);
        valueView.getValueContainer().addMember("m_textureBindings", valueView.m_data->m_textureBindings);
        valueView.getValueContainer().addMember("m_defines", valueView.m_data->m_defines);
    }

    template<FormatType TFormatType>
    void SerdeSpec<IShader, TFormatType>::deserialize(DeserializableValueView<IShader, TFormatType>& valueView) noexcept
    {
        /*const auto m_fileAssetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_fileAssetPath");

        if(m_fileAssetPath)
        {
            auto shaderFile = valueView.m_data->getParentAssetManager()->template loadAsset<TextFileAsset>(*m_fileAssetPath);
            auto shaderAnalyzedFile = valueView.m_data->getParentAssetManager()->template loadAsset<ShaderAnalyzedFile>(*m_fileAssetPath);
            valueView.m_data->m_fileAsset = shaderFile;
            valueView.m_data->m_shaderAnalyzedFile = shaderAnalyzedFile;
        }*/

        const auto m_shaderAnalyzedFile = valueView.getValueContainer().template getMember<AssetRef<ShaderAnalyzedFile>>("m_shaderAnalyzedFile");
        if(m_shaderAnalyzedFile)
        {
            valueView.m_data->m_shaderAnalyzedFile = *m_shaderAnalyzedFile;
        }

        const auto m_fileAsset = valueView.getValueContainer().template getMember<AssetRef<TextFileAsset>>("m_fileAsset");
        if(m_fileAsset)
        {
            valueView.m_data->m_fileAsset = *m_fileAsset;
        }

        const auto m_autoRecompile = valueView.getValueContainer().template getMember<bool>("m_autoRecompile");
        if(m_autoRecompile)
        {
            valueView.m_data->m_autoRecompile = *m_autoRecompile;
        }

        auto m_textureBindings = valueView.getValueContainer().template getMember<std::vector<ShaderTextureBinding>>("m_textureBindings");
        if(m_textureBindings)
        {
            valueView.m_data->m_textureBindings = std::move(*m_textureBindings);
        }

        auto m_defines = valueView.getValueContainer().template getMember<decltype(IShader::m_defines)>("m_defines");
        if(m_defines)
        {
            valueView.m_data->m_defines = std::move(*m_defines);
        }
    }

    template<FormatType TFormatType>
    IShader* SerdeSpec<IShader, TFormatType>::allocateObject(DeserializableValueView<IShader, TFormatType>& valueView) noexcept
    {
        return CoreMain::getRenderer()->createShader();
    }

    // =============================================== impl ShaderAnalyzedFile

    template<FormatType TFormatType>
    void SerdeSpec<ShaderAnalyzedFile, TFormatType>::serialize(SerializableValueView<const ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_subPassName", valueView.m_data->m_subPassName);
        valueView.getValueContainer().addMember("m_attributes", valueView.m_data->m_attributes);
        valueView.getValueContainer().addMember("m_subShaders", valueView.m_data->m_subShaders, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ShaderAnalyzedFile, TFormatType>::deserialize(DeserializableValueView<ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto m_subPassName = valueView.getValueContainer().template getMember<std::string>("m_subPassName");
        if(m_subPassName)
        {
            valueView.m_data->m_subPassName = std::move(*m_subPassName);
        }

        auto m_attributes = valueView.getValueContainer().template getMember<decltype(ShaderAnalyzedFile::m_attributes)>("m_attributes");
        if(m_attributes)
        {
            valueView.m_data->m_attributes = std::move(*m_attributes);
        }

        auto m_subShaders = valueView.getValueContainer().template getMember<decltype(ShaderAnalyzedFile::m_subShaders)>("m_subShaders", assetsPackage);
        if(m_subShaders)
        {
            valueView.m_data->m_subShaders = std::move(*m_subShaders);
        }
    }

    // =============================================== impl SGSLESubShader

    template<FormatType TFormatType>
    void SerdeSpec<SGSLESubShader, TFormatType>::serialize(SerializableValueView<const SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_type", valueView.m_data->m_type);

        AssetsPackage::DataMarkup textureDataMarkup = assetsPackage.addData(valueView.m_data->m_code);

        valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
        valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
    }

    template<FormatType TFormatType>
    void SerdeSpec<SGSLESubShader, TFormatType>::deserialize(DeserializableValueView<SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        const auto typeOpt = valueView.getValueContainer().template getMember<SGSLESubShaderType>("m_type");
        auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
        auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

        if(typeOpt)
        {
            valueView.m_data->m_type = *typeOpt;
        }

        if(dataOffsetOpt)
        {
            valueView.m_data->m_codeOffsetInPackage = *dataOffsetOpt;
        }

        if(dataSizeInBytesOpt)
        {
            valueView.m_data->m_codeSizeInPackage = *dataSizeInBytesOpt;
        }
    }

    // =============================================== impl IMaterial

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<IMaterial, TFormatType>::serialize(SerializableValueView<const IMaterial, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
        valueView.getValueContainer().addMember("m_transparencyType", valueView.m_data->m_transparencyType);
        valueView.getValueContainer().addMember("m_shaders", valueView.m_data->m_shaders, std::forward<SharedDataT>(sharedData)...);
        valueView.getValueContainer().addMember("m_meshRenderState", valueView.m_data->m_meshRenderState);

        valueView.getValueContainer().addMember("m_textures", valueView.m_data->m_textures, std::forward<SharedDataT>(sharedData)...);

        valueView.getValueContainer().addMember("m_diffuseColor", valueView.m_data->m_diffuseColor);
        valueView.getValueContainer().addMember("m_specularColor", valueView.m_data->m_specularColor);
        valueView.getValueContainer().addMember("m_ambientColor", valueView.m_data->m_ambientColor);
        valueView.getValueContainer().addMember("m_emissionColor", valueView.m_data->m_emissionColor);
        valueView.getValueContainer().addMember("m_transparentColor", valueView.m_data->m_transparentColor);
        valueView.getValueContainer().addMember("m_shininess", valueView.m_data->m_shininess);
        valueView.getValueContainer().addMember("m_metallicFactor", valueView.m_data->m_metallicFactor);
        valueView.getValueContainer().addMember("m_roughnessFactor", valueView.m_data->m_roughnessFactor);
    }

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<IMaterial, TFormatType>::deserialize(DeserializableValueView<IMaterial, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
        if(name)
        {
            valueView.m_data->m_name = std::move(*name);
        }

        auto transparencyType = valueView.getValueContainer().template getMember<MaterialTransparencyType>("m_transparencyType");
        if(transparencyType)
        {
            valueView.m_data->m_transparencyType = std::move(*transparencyType);
        }

        auto shaders = valueView.getValueContainer().template getMember<std::unordered_map<std::string, AssetRef<IShader>>>("m_shaders", std::forward<SharedDataT>(sharedData)...);
        if(shaders)
        {
            valueView.m_data->m_shaders = std::move(*shaders);
        }

        const auto meshRenderState = valueView.getValueContainer().template getMember<MeshRenderState>("m_meshRenderState");
        if(meshRenderState)
        {
            valueView.m_data->m_meshRenderState = *meshRenderState;
        }

        auto textures = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_textures)>("m_textures", std::forward<SharedDataT>(sharedData)...);
        if(textures)
        {
            valueView.m_data->m_textures = std::move(*textures);
        }

        const auto diffuseColor = valueView.getValueContainer().template getMember<glm::vec4>("m_diffuseColor");
        if(diffuseColor)
        {
            valueView.m_data->m_diffuseColor = *diffuseColor;
        }

        const auto specularColor = valueView.getValueContainer().template getMember<glm::vec4>("m_specularColor");
        if(specularColor)
        {
            valueView.m_data->m_specularColor = *specularColor;
        }

        const auto ambientColor = valueView.getValueContainer().template getMember<glm::vec4>("m_ambientColor");
        if(ambientColor)
        {
            valueView.m_data->m_ambientColor = *ambientColor;
        }

        const auto emissionColor = valueView.getValueContainer().template getMember<glm::vec4>("m_emissionColor");
        if(emissionColor)
        {
            valueView.m_data->m_emissionColor = *emissionColor;
        }

        const auto transparentColor = valueView.getValueContainer().template getMember<glm::vec4>("m_transparentColor");
        if(transparentColor)
        {
            valueView.m_data->m_transparentColor = *transparentColor;
        }

        const auto shininess = valueView.getValueContainer().template getMember<float>("m_shininess");
        if(shininess)
        {
            valueView.m_data->m_shininess = *shininess;
        }

        const auto metallicFactor = valueView.getValueContainer().template getMember<float>("m_metallicFactor");
        if(metallicFactor)
        {
            valueView.m_data->m_metallicFactor = *metallicFactor;
        }

        const auto roughnessFactor = valueView.getValueContainer().template getMember<float>("m_roughnessFactor");
        if(roughnessFactor)
        {
            valueView.m_data->m_roughnessFactor = *roughnessFactor;
        }
    }

    // =============================================== impl IMeshData

    template<FormatType TFormatType>
    void SerdeSpec<IMeshData, TFormatType>::serialize(SerializableValueView<const IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_aabb", valueView.m_data->m_aabb);
        valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);

        {
            AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_indices);

            valueView.getValueContainer().addMember("m_indicesOffset", dataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_indicesSizeInBytes", dataMarkup.m_sizeInBytes);
        }

        {
            AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_vertices);

            valueView.getValueContainer().addMember("m_verticesOffset", dataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_verticesSizeInBytes", dataMarkup.m_sizeInBytes);
        }

        // TODO: VERTEX COLORS SERIALIZING IN PACKAGE

        // TODO: MAYBE SERIALIZING DATA OF PHYSICAL MESH (I DONT FUCKING KNOW HOW TO GET DATA FROM btTriangleMesh)
        valueView.getValueContainer().addMember("m_generatePhysicalMesh", valueView.m_data->m_physicalMesh != nullptr);

        valueView.getValueContainer().addMember("m_material", valueView.m_data->m_material, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<IMeshData, TFormatType>::deserialize(DeserializableValueView<IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto aabb = valueView.getValueContainer().template getMember<AABB<>>("m_aabb");
        if(aabb)
        {
            valueView.m_data->m_aabb = *aabb;
        }

        auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
        if(name)
        {
            valueView.m_data->m_name = std::move(*name);
        }

        {
            auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                    "m_indicesOffset"
            );
            auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                    "m_indicesSizeInBytes"
            );

            if(dataOffsetOpt)
            {
                valueView.m_data->m_indicesOffsetInPackage = *dataOffsetOpt;
            }

            if(dataSizeInBytesOpt)
            {
                valueView.m_data->m_indicesSizeInPackage = *dataSizeInBytesOpt;
            }
        }

        {
            auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                    "m_verticesOffset"
            );
            auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                    "m_verticesSizeInBytes"
            );

            if(dataOffsetOpt)
            {
                valueView.m_data->m_verticesOffsetInPackage = *dataOffsetOpt;
            }

            if(dataSizeInBytesOpt)
            {
                valueView.m_data->m_verticesSizeInPackage = *dataSizeInBytesOpt;
            }
        }

        // TODO: VERTEX COLORS DESERIALIZING FROM PACKAGE

        auto generatePhysicalMesh = valueView.getValueContainer().template getMember<bool>("m_generatePhysicalMesh");
        if(generatePhysicalMesh)
        {
            valueView.m_data->generatePhysicalMesh();
        }

        auto material = valueView.getValueContainer().template getMember<AssetRef<IMaterial>>("m_material");
        if(material)
        {
            valueView.m_data->m_material = std::move(*material);
        }
    }

    template<FormatType TFormatType>
    IMeshData* SerdeSpec<IMeshData, TFormatType>::allocateObject(DeserializableValueView<IMeshData, TFormatType>& valueView) noexcept
    {
        return CoreMain::getRenderer()->createMeshData();
    }

    // =============================================== impl ModelAsset

    template<FormatType TFormatType>
    void SerdeSpec<ModelAsset, TFormatType>::serialize(SerializableValueView<const ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_importerFlags", valueView.m_data->m_importerFlags);
        valueView.getValueContainer().addMember("m_modelName", valueView.m_data->m_modelName);
        valueView.getValueContainer().addMember("m_rootNode", valueView.m_data->m_rootNode, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ModelAsset, TFormatType>::deserialize(DeserializableValueView<ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto importerFlags = valueView.getValueContainer().template getMember<int>("m_importerFlags");
        if(importerFlags)
        {
            valueView.m_data->m_importerFlags = *importerFlags;
        }

        auto modelName = valueView.getValueContainer().template getMember<std::string>("m_modelName");
        if(modelName)
        {
            valueView.m_data->m_modelName = std::move(*modelName);
        }

        auto rootNode = valueView.getValueContainer().template getMember<Ref<Node>>("m_rootNode", assetsPackage);
        if(rootNode)
        {
            valueView.m_data->m_rootNode = std::move(*rootNode);
        }
    }

    // =============================================== impl Skeleton

    template<FormatType TFormatType>
    void SerdeSpec<Skeleton, TFormatType>::serialize(SerializableValueView<const Skeleton, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_rootBone", valueView.m_data->m_rootBone, assetsPackage);
        valueView.getValueContainer().addMember("m_allBones", valueView.m_data->m_allBones, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Skeleton, TFormatType>::deserialize(DeserializableValueView<Skeleton, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto rootBone = valueView.getValueContainer().template getMember<AssetRef<Bone>>("m_rootBone", assetsPackage);
        if(rootBone)
        {
            valueView.m_data->m_rootBone = std::move(*rootBone);
        }

        auto allBones = valueView.getValueContainer().template getMember<decltype(Skeleton::m_allBones)>("m_allBones", assetsPackage);
        if(allBones)
        {
            valueView.m_data->m_allBones = std::move(*allBones);
        }
    }

    // =============================================== impl Bone

    template<FormatType TFormatType>
    void SerdeSpec<Bone, TFormatType>::serialize(SerializableValueView<const Bone, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        // todo: is it correct?
        valueView.getValueContainer().addMember("m_id", valueView.m_data->m_id);
        valueView.getValueContainer().addMember("m_boneName", valueView.m_data->m_boneName);
        valueView.getValueContainer().addMember("m_offsetMatrix", valueView.m_data->m_offsetMatrix);
        valueView.getValueContainer().addMember("m_affectedMeshesBoneData", valueView.m_data->m_affectedMeshesBoneData, assetsPackage);
        valueView.getValueContainer().addMember("m_children", valueView.m_data->m_children, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Bone, TFormatType>::deserialize(DeserializableValueView<Bone, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        // todo: is it correct?
        auto id = valueView.getValueContainer().template getMember<decltype(Bone::m_id)>("m_id");
        if(id)
        {
            valueView.m_data->m_id = std::move(*id);
        }

        auto boneName = valueView.getValueContainer().template getMember<std::string>("m_boneName");
        if(boneName)
        {
            valueView.m_data->m_boneName = std::move(*boneName);
        }

        auto offsetMatrix = valueView.getValueContainer().template getMember<glm::mat4>("m_offsetMatrix");
        if(offsetMatrix)
        {
            valueView.m_data->m_offsetMatrix = std::move(*offsetMatrix);
        }

        auto affectedMeshesBoneData = valueView.getValueContainer().template getMember<decltype(Bone::m_affectedMeshesBoneData)>("m_affectedMeshesBoneData", assetsPackage);
        if(affectedMeshesBoneData)
        {
            valueView.m_data->m_affectedMeshesBoneData = std::move(*affectedMeshesBoneData);
        }

        auto children = valueView.getValueContainer().template getMember<decltype(Bone::m_children)>("m_children", assetsPackage);
        if(children)
        {
            valueView.m_data->m_children = std::move(*children);
        }
    }

    // =============================================== impl MeshBoneData

    template<FormatType TFormatType>
    void SerdeSpec<MeshBoneData, TFormatType>::serialize(SerializableValueView<const MeshBoneData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_affectedMesh", valueView.m_data->m_affectedMesh, assetsPackage);

        {
            AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_weights);

            valueView.getValueContainer().addMember("m_weightsDataMarkupInPackage", dataMarkup);
        }
    }

    template<FormatType TFormatType>
    void SerdeSpec<MeshBoneData, TFormatType>::deserialize(DeserializableValueView<MeshBoneData, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto affectedMesh = valueView.getValueContainer().template getMember<AssetRef<IMeshData>>("m_affectedMesh", assetsPackage);
        if(affectedMesh)
        {
            valueView.m_data->m_affectedMesh = std::move(*affectedMesh);
        }

        {
            auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                    "m_weightsDataMarkupInPackage"
            );

            if(dataMarkupOpt)
            {
                valueView.m_data->m_weightsDataMarkupInPackage = *dataMarkupOpt;
            }
        }
    }

    // =============================================== impl AnimationsFile

    template<FormatType TFormatType>
    void SerdeSpec<AnimationsFile, TFormatType>::serialize(SerializableValueView<const AnimationsFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_skeletalAnimations", valueView.m_data->m_skeletalAnimations, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<AnimationsFile, TFormatType>::deserialize(DeserializableValueView<AnimationsFile, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        std::cout << "AnimationsFile deserialize: skeletal animations count before deserialization: " << valueView.m_data->m_skeletalAnimations.size() << std::endl;

        auto skeletalAnimations = valueView.getValueContainer().template getMember<decltype(AnimationsFile::m_skeletalAnimations)>("m_skeletalAnimations", assetsPackage);
        if(skeletalAnimations)
        {
            valueView.m_data->m_skeletalAnimations = std::move(*skeletalAnimations);
        }

        std::cout << "AnimationsFile deserialize: skeletal animations count after deserialization: " << valueView.m_data->m_skeletalAnimations.size() << std::endl;
    }

    // =============================================== impl SkeletalAnimationAsset

    template<FormatType TFormatType>
    void SerdeSpec<SkeletalAnimationAsset, TFormatType>::serialize(SerializableValueView<const SkeletalAnimationAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_animationName", valueView.m_data->m_animationName);
        valueView.getValueContainer().addMember("m_duration", valueView.m_data->m_duration);
        valueView.getValueContainer().addMember("m_ticksPerSecond", valueView.m_data->m_ticksPerSecond);
        valueView.getValueContainer().addMember("m_bonesAnimations", valueView.m_data->m_bonesAnimations, assetsPackage);
    }

    template<FormatType TFormatType>
    void SerdeSpec<SkeletalAnimationAsset, TFormatType>::deserialize(DeserializableValueView<SkeletalAnimationAsset, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto animationName = valueView.getValueContainer().template getMember<std::string>("m_animationName");
        if(animationName)
        {
            valueView.m_data->m_animationName = std::move(*animationName);
        }

        auto duration = valueView.getValueContainer().template getMember<float>("m_duration");
        if(duration)
        {
            valueView.m_data->m_duration = std::move(*duration);
        }

        auto ticksPerSecond = valueView.getValueContainer().template getMember<float>("m_ticksPerSecond");
        if(ticksPerSecond)
        {
            valueView.m_data->m_ticksPerSecond = std::move(*ticksPerSecond);
        }

        auto bonesAnimations = valueView.getValueContainer().template getMember<decltype(SkeletalAnimationAsset::m_bonesAnimations)>("m_bonesAnimations", assetsPackage);
        if(bonesAnimations)
        {
            valueView.m_data->m_bonesAnimations = std::move(*bonesAnimations);
        }
    }

    // =============================================== impl SkeletalBoneAnimation

    template<FormatType TFormatType>
    void SerdeSpec<SkeletalBoneAnimation, TFormatType>::serialize(SerializableValueView<const SkeletalBoneAnimation, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.getValueContainer().addMember("m_boneName", valueView.m_data->m_boneName);

        {
            AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_positionKeys);

            valueView.getValueContainer().addMember("m_positionKeysMarkupInPackage", dataMarkup);
        }

        {
            AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_rotationKeys);

            valueView.getValueContainer().addMember("m_rotationKeysMarkupInPackage", dataMarkup);
        }

        {
            AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_scaleKeys);

            valueView.getValueContainer().addMember("m_scaleKeysMarkupInPackage", dataMarkup);
        }
    }

    template<FormatType TFormatType>
    void SerdeSpec<SkeletalBoneAnimation, TFormatType>::deserialize(DeserializableValueView<SkeletalBoneAnimation, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto boneName = valueView.getValueContainer().template getMember<std::string>("m_boneName");
        if(boneName)
        {
            valueView.m_data->m_boneName = std::move(*boneName);
        }

        {
            auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                    "m_positionKeysMarkupInPackage"
            );

            if(dataMarkupOpt)
            {
                valueView.m_data->m_positionKeysMarkupInPackage = *dataMarkupOpt;
            }
        }

        {
            auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                    "m_rotationKeysMarkupInPackage"
            );

            if(dataMarkupOpt)
            {
                valueView.m_data->m_rotationKeysMarkupInPackage = *dataMarkupOpt;
            }
        }

        {
            auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                    "m_scaleKeysMarkupInPackage"
            );

            if(dataMarkupOpt)
            {
                valueView.m_data->m_scaleKeysMarkupInPackage = *dataMarkupOpt;
            }
        }
    }

    // =============================================== impl AssetRef

    // WE ARE SERIALIZING ONLY META INFO OF ASSET BECAUSE IT IS ASSET REFERENCE. WE DO NOT NEED TO DO SERIALIZATION OF DATA
    template<typename AssetT, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<AssetRef<AssetT>, TFormatType>::serialize(SerializableValueView<const AssetRef<AssetT>, TFormatType>& valueView, SharedDataT&&...) noexcept
    {
        if(!valueView.m_data->m_asset)
        {
            valueView.getValueContainer().setAsNull();
            return;
        }

        valueView.getValueContainer().addMember("m_path", (*valueView.m_data)->getPath());
        valueView.getValueContainer().addMember("m_assetTypeID", (*valueView.m_data)->getTypeID());
        valueView.getValueContainer().addMember("m_alias", (*valueView.m_data)->getAlias());
        valueView.getValueContainer().addMember("m_storedBy", (*valueView.m_data)->storedByWhat());
        valueView.getValueContainer().addMember("m_parentAssetManagerName", (*valueView.m_data)->getParentAssetManager()->getName());
    }

    // WE ARE DESERIALIZING ONLY META INFO OF ASSET BECAUSE IT IS ASSET REFERENCE. WE DO NOT NEED TO DO DESERIALIZATION OF DATA
    template<typename AssetT, FormatType TFormatType>
    void SerdeSpec<AssetRef<AssetT>, TFormatType>::deserialize(DeserializableValueView<AssetRef<AssetT>, TFormatType>& valueView, AssetsPackage&) noexcept
    {
        if(valueView.getValueContainer().isNull())
        {
            return;
        }

        auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
        const auto assetTypeID = valueView.getValueContainer().template getMember<size_t>("m_assetTypeID");
        auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
        const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
        const auto parentAssetManagerName = valueView.getValueContainer().template getMember<std::string>("m_parentAssetManagerName");

        if(!parentAssetManagerName) return;

        auto parentAssetManager = AssetManager::getAssetManager(*parentAssetManagerName);

        // WE DO NOT set parent asset manager because we are getting already existing asset from asset manager
        // and this asset is already has parent asset manager

        // checking if asset is already exists
        // TRYING TO RESOLVE REFERENCE AUTOMATICALLY
        if (parentAssetManager->isAssetExists(*assetAlias, *assetPath, *assetStorageType, *assetTypeID))
        {
            // setting m_asset to asset from parent asset manager
            LOG_I(SGCORE_TAG, "Asset is already exist. Reference was resolved automatically.");
            valueView.m_data->m_asset =
                    std::static_pointer_cast<AssetT>(
                            parentAssetManager->loadExistingAsset(*assetAlias, *assetPath, *assetStorageType, *assetTypeID).m_asset);
        }

        // assigning values only after getting asset from asset manager
        valueView.m_data->m_deserializedAssetAlias = std::move(*assetAlias);
        valueView.m_data->m_deserializedAssetPath = std::move(*assetPath);
        valueView.m_data->m_deserializedAssetStoredBy = *assetStorageType;
        valueView.m_data->m_deserializedAssetTypeID = *assetTypeID;
        valueView.m_data->m_deserializedParentAssetManager = parentAssetManager;
    }

    template<typename AssetT, FormatType TFormatType>
    void SerdeSpec<AssetRef<AssetT>, TFormatType>::deserialize(DeserializableValueView<AssetRef<AssetT>, TFormatType>& valueView) noexcept
    {
        if(valueView.getValueContainer().isNull())
        {
            return;
        }

        auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
        const auto assetTypeID = valueView.getValueContainer().template getMember<size_t>("m_assetTypeID");
        auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
        const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
        const auto parentAssetManagerName = valueView.getValueContainer().template getMember<std::string>("m_parentAssetManagerName");

        LOG_I(SGCORE_TAG, "Deserializing AssetRef... AssetRef data: path: '{}', alias: '{}', asset type ID: '{}', stored by: '{}'",
              SGCore::Utils::toUTF8(assetPath->resolved().u16string()),
              *assetAlias,
              *assetTypeID,
              std::to_underlying(*assetStorageType));

        if(!parentAssetManagerName) return;

        auto parentAssetManager = AssetManager::getAssetManager(*parentAssetManagerName);

        // WE DO NOT set parent asset manager because we are getting already existing asset from asset manager
        // and this asset is already has parent asset manager

        // checking if asset is already exists
        // TRYING TO RESOLVE REFERENCE AUTOMATICALLY
        bool assetResolvedAutomatically = false;
        if (parentAssetManager->isAssetExists(*assetAlias, *assetPath, *assetStorageType, *assetTypeID))
        {
            // setting m_asset to asset from parent asset manager
            LOG_I(SGCORE_TAG, "Asset is already exist. Reference was resolved automatically.");
            valueView.m_data->m_asset =
                    std::static_pointer_cast<AssetT>(
                            parentAssetManager->loadExistingAsset(*assetAlias, *assetPath, *assetStorageType, *assetTypeID).m_asset);

            assetResolvedAutomatically = true;
        }

        // assigning values only after getting asset from asset manager
        valueView.m_data->m_deserializedAssetAlias = *assetAlias;
        valueView.m_data->m_deserializedAssetPath = *assetPath;
        valueView.m_data->m_deserializedAssetStoredBy = *assetStorageType;
        valueView.m_data->m_deserializedAssetTypeID = *assetTypeID;
        valueView.m_data->m_deserializedParentAssetManager = parentAssetManager;

        if(assetResolvedAutomatically)
        {
            return;
        }

        // if asset ref was serialized by path and path is valid and if we are deserializing this ref
        // not from assets package then we are trying to load asset from filesystem
        if(*assetStorageType == AssetStorageType::BY_PATH && std::filesystem::exists(assetPath->resolved()))
        {
            LOG_I(SGCORE_TAG, "Loading AssetRef by path... AssetRef data: path: '{}', alias: '{}', type ID: '{}'",
                  SGCore::Utils::toUTF8(assetPath->resolved().u16string()),
                  *assetAlias,
                  *assetTypeID);

            valueView.m_data->m_asset = parentAssetManager->template loadAsset<AssetT>(*assetPath).m_asset;
        }
    }

    // =============================================== impl AssetRef

    template<FormatType TFormatType>
    void SerdeSpec<AssetsPackage::DataMarkup, TFormatType>::serialize(SerializableValueView<const AssetsPackage::DataMarkup, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_offset", valueView.m_data->m_offset);
        valueView.getValueContainer().addMember("m_sizeInBytes", valueView.m_data->m_sizeInBytes);
    }

    template<FormatType TFormatType>
    void SerdeSpec<AssetsPackage::DataMarkup, TFormatType>::deserialize(DeserializableValueView<AssetsPackage::DataMarkup, TFormatType>& valueView) noexcept
    {
        const auto m_offset = valueView.getValueContainer().template getMember<std::streamsize>("m_offset");
        if(m_offset)
        {
            valueView.m_data->m_offset = *m_offset;
        }

        const auto m_sizeInBytes = valueView.getValueContainer().template getMember<std::streamsize>("m_sizeInBytes");
        if(m_sizeInBytes)
        {
            valueView.m_data->m_sizeInBytes = *m_sizeInBytes;
        }
    }
}
