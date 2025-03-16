//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_FONT_H
#define SUNGEARENGINE_FONT_H

#include <SGCore/pch.h>

#include "IAsset.h"
#include "FontSpecialization.h"

namespace SGCore::UI
{
    struct Font : public IAsset
    {
        sg_implement_type_id(Font, 5)

        Ref<FontSpecialization> addOrGetSpecialization(const FontSpecializationSettings& fontSpecializationSettings);
        Ref<FontSpecialization> getSpecialization(const FontSpecializationSettings& fontSpecializationSettings);
        [[nodiscard]] bool isSpecializationExists(const FontSpecializationSettings& fontSpecializationSettings) const noexcept;

        const auto& getSpecializations() const noexcept
        {
            return m_specializations;
        }
        
    protected:
        // todo: impl
        void doLoad(const InterpolatedPath& path) override;
        // todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        
    private:
        std::unordered_map<FontSpecializationSettings, Ref<FontSpecialization>> m_specializations;
    };
}

#endif //SUNGEARENGINE_FONT_H
