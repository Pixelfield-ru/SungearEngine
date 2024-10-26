//
// Created by stuka on 23.10.2024.
//

#ifndef SUNGEARENGINE_ASSETSPACKAGE_H
#define SUNGEARENGINE_ASSETSPACKAGE_H

#include <SGCore/Utils/Utils.h>
#include <SGCore/Utils/FileUtils.h>

namespace SGCore
{
    struct IAsset;
    struct AssetManager;

    /**
     * DO NOT STORE ANYWHERE.
     */
    struct AssetsPackage
    {
        struct DataMarkup
        {
            /// Data offset in binary file.
            std::streamsize m_offset = 0;

            /// Data size (in bytes) in binary file.
            std::streamsize m_sizeInBytes = 0;
        };

        friend struct AssetManager;
        friend struct AssetSection;

        template<collection CollectionT>
        [[nodiscard]] DataMarkup addData(const CollectionT& data) noexcept
        {
            DataMarkup writtenDataMarkup { std::ssize(m_buffer), std::ssize(data) * sizeof(CollectionT::value_type) };

            auto* bytes = reinterpret_cast<const char*>(data.data());
            m_buffer.insert(m_buffer.end(), bytes, (bytes + data.size() * sizeof(CollectionT::value_type)));

            return writtenDataMarkup;
        }

        template<typename T>
        [[nodiscard]] DataMarkup addData(const T& data) noexcept
        {
            DataMarkup writtenDataMarkup { std::ssize(m_buffer), sizeof(T) };

            auto* bytes = reinterpret_cast<const char*>(&data);
            m_buffer.insert(m_buffer.end(), bytes, (bytes + sizeof(T)));

            return writtenDataMarkup;
        }

        template<typename T>
        [[nodiscard]] DataMarkup addData(const T* buffer, const std::streamsize& bufferSize) noexcept
        {
            DataMarkup writtenDataMarkup { std::ssize(m_buffer), bufferSize * sizeof(T) };

            auto* bytes = reinterpret_cast<const char*>(buffer);
            m_buffer.insert(m_buffer.end(), bytes, (bytes + bufferSize * sizeof(T)));

            return writtenDataMarkup;
        }

        /**
         * READS CONTAINER FROM BINARY FILE.
         * @tparam T
         * @param offset
         * @param size
         * @return
         */
        template<collection CollectionT>
        [[nodiscard]] CollectionT readData(const std::streamsize& offset, const std::streamsize& size) noexcept
        {
            using value_t = typename CollectionT::value_type;

            std::vector<char> bytes = FileUtils::readBytesBlock(m_path, offset, size);
            CollectionT data = CollectionT((value_t*) bytes.data(), ((value_t*) bytes.data()) + size / sizeof(value_t));

            return data;
        }

        /**
         * READS OBJECT FROM BINARY FILE.
         * @tparam T
         * @param offset
         * @param size
         * @return
         */
        template<typename T>
        [[nodiscard]] T readData(const std::streamsize& offset, const std::streamsize& size) noexcept
        {
            return *reinterpret_cast<T*>(FileUtils::readBytesBlock(m_path, offset, size).data());
        }

        [[nodiscard]] bool isDataSerde() const noexcept
        {
            return m_useSerdeData;
        }

        [[nodiscard]] const std::filesystem::path& getPath() const noexcept
        {
            return m_path;
        }

        [[nodiscard]] auto getParentAssetManager() const noexcept
        {
            return m_parentAssetManager;
        }

    private:
        AssetManager* m_parentAssetManager { };
        std::filesystem::path m_path;
        bool m_useSerdeData = false;
        std::vector<char> m_buffer;
    };
}

#endif //SUNGEARENGINE_ASSETSPACKAGE_H
