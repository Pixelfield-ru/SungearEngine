//
// Created by stuka on 23.10.2024.
//

#ifndef SUNGEARENGINE_ASSETSPACKAGE_H
#define SUNGEARENGINE_ASSETSPACKAGE_H

#include <SGCore/Utils/Utils.h>

namespace SGCore
{
    struct IAsset;

    /**
     * DO NOT STORE ANYWHERE.
     */
    struct AssetsPackage
    {
        struct AssetSection
        {
            friend struct AssetsPackage;
            friend struct AssetManager;

            explicit AssetSection(AssetsPackage* parentPackage)
            {
                m_parentPackage = parentPackage;
                m_lastPackageSize = parentPackage->m_buffer.size();
            }

            template<typename AssetT>
            requires(std::is_base_of_v<IAsset, AssetT>)
            void addStandardInfo(const AssetT* currentAssetInstance) noexcept
            {
                static_assert(sizeof(AssetT::asset_type_id) == 8,
                              "Size of type of 'asset_type_id' field must be equal to 8."
                );

                // size_t IS IMPORTANT! 8 BYTES FOR STORING SIZE OF WHOLE ASSET SECTION
                addSection<size_t>("size:", 0);
                addSection("path:", Utils::toUTF8(currentAssetInstance->getPath().u16string()));
                addSection("alias:", "unknown");
                addSection("typeID:", AssetT::asset_type_id);
            }

            template<typename T>
            void addSection(const std::string& name, const T& data) noexcept
            {
                m_currentSectionSize += name.size() + sizeof(T);

                auto* bytes = reinterpret_cast<const char*>(&data);
                m_parentPackage->m_buffer.insert(m_parentPackage->m_buffer.end(), name.data(),
                                                 (name.data() + name.size()));
                m_parentPackage->m_buffer.insert(m_parentPackage->m_buffer.end(), bytes, (bytes + sizeof(T)));

                updateSizeSection();
            }

            template<typename T>
            void addSection(const std::string& name, const T* buffer, const size_t& bufferSize) noexcept
            {
                m_currentSectionSize += name.size() + sizeof(T);

                auto* bytes = reinterpret_cast<const char*>(buffer);
                m_parentPackage->m_buffer.insert(m_parentPackage->m_buffer.end(), name.data(),
                                                 (name.data() + name.size()));
                m_parentPackage->m_buffer.insert(m_parentPackage->m_buffer.end(), bytes, (bytes + bufferSize * sizeof(T)));

                updateSizeSection();
            }

            template<typename CollectionT>
            requires(requires { CollectionT::size(); CollectionT::data(); CollectionT::value_type; })
            void addSection(const std::string& name, const CollectionT& data) noexcept
            {
                m_currentSectionSize += name.size() + data.size();

                auto* bytes = reinterpret_cast<const char*>(data.data());
                m_parentPackage->m_buffer.insert(m_parentPackage->m_buffer.end(), name.data(),
                                                 (name.data() + name.size()));
                m_parentPackage->m_buffer.insert(m_parentPackage->m_buffer.end(), bytes, (bytes + data.size() * sizeof(CollectionT::value_type)));

                updateSizeSection();
            }

        private:
            void updateSizeSection()
            {
                auto* bytes = reinterpret_cast<const char*>(&m_currentSectionSize);

                // size of 'size:' == 5

                for(size_t i = 0; i < sizeof(m_currentSectionSize); ++i)
                {
                    m_parentPackage->m_buffer[m_lastPackageSize + 5 + i] = bytes[i];
                }
            }

            AssetsPackage* m_parentPackage = nullptr;

            size_t m_currentSectionSize = 0;
            // size of package before adding any data of current asset
            size_t m_lastPackageSize = 0;
        };

        friend struct AssetManager;
        friend struct AssetSection;

        std::filesystem::path m_path;

    private:
        std::vector<char> m_buffer;
    };
}

#endif //SUNGEARENGINE_ASSETSPACKAGE_H
