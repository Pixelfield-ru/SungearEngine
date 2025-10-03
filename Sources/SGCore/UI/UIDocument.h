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

        [[nodiscard]] AssetRef<CSSStyle> findStyle(const std::string& selector) const noexcept;
        [[nodiscard]] Ref<UIElement> findElement(const std::string& elementName) const noexcept;

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, UIElement*, UIElement*>)
        void iterate(const FuncT& func, bool& breakToken) noexcept
        {
            m_rootElement->iterate(func, breakToken);
        }

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, const UIElement*, const UIElement*>)
        void iterate(const FuncT& func, bool& breakToken) const noexcept
        {
            m_rootElement->iterate(func, breakToken);
        }

    protected:
        void doLoad(const InterpolatedPath& path) override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        [[nodiscard]] Ref<UIElement> processUIElement(const pugi::xml_node& xmlNode) noexcept;

        void applyDefaultStylesToNonStyledElements() noexcept;

    private:
        void applyDefaultStylesToNonStyledElementsImpl(const Ref<UIElement>& element) noexcept;
    };
}

#endif //SUNGEARENGINE_UIDOCUMENT_H
