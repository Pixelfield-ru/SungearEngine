//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_UIDOCUMENT_H
#define SUNGEARENGINE_UIDOCUMENT_H

#include "SGCore/Memory/Assets/IAsset.h"

#include "SGCore/UI/Elements/UIRoot.h"

namespace SGCore::UI
{
    struct CSSFile;
    
    struct UIDocument : public IAsset
    {
        friend struct UIElementNodeProcessor;

        sg_implement_type_id(UIDocument, 10)

        pugi::xml_document m_document;

        Ref<UIRoot> m_rootElement;

        std::vector<AssetRef<CSSFile>> m_includedCSSFiles;

        [[nodiscard]] AssetRef<CSSSelector> findSelector(const std::string& selectorName) const noexcept;
        [[nodiscard]] Ref<UIElement> findElement(const std::string& elementName) const noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        [[nodiscard]] Ref<UIElement> processUIElement(const pugi::xml_node& xmlNode) noexcept;
    };
}

#endif //SUNGEARENGINE_UIDOCUMENT_H
