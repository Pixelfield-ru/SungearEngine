//
// Created by ilya on 23.02.24.
//

#pragma once

#include "IAsset.h"
#include "FontSpecialization.h"
#include <msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

namespace SGCore::UI
{
    struct SGCORE_EXPORT Font : public IAsset
    {
        sg_implement_type_id(SGCore::UI::Font)

        ~Font() noexcept override;

        Ref<FontSpecialization> addOrGetSpecialization(const FontSpecializationSettings& fontSpecializationSettings);
        Ref<FontSpecialization> getSpecialization(const FontSpecializationSettings& fontSpecializationSettings);
        [[nodiscard]] bool isSpecializationExists(const FontSpecializationSettings& fontSpecializationSettings) const noexcept;

        const auto& getSpecializations() const noexcept
        {
            return m_specializations;
        }

        msdfgen::FontHandle* getFontHandler() const noexcept;
        
    protected:
        // todo: impl
        void doLoad(const InterpolatedPath& path) override;
        // todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        
    private:
        msdfgen::FontHandle* m_fontHandler = nullptr;

        std::unordered_map<FontSpecializationSettings, Ref<FontSpecialization>> m_specializations;
    };
}
