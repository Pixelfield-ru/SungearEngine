//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_UIDOCUMENT_H
#define SUNGEARENGINE_UIDOCUMENT_H

#include <pugixml.hpp>

#include "BindingsStorage.h"
#include "SGCore/Memory/Assets/IAsset.h"

#include "SGCore/UI/Elements/UIRoot.h"

namespace SGCore::UI
{
    struct CSSFile;
    struct TemplateElement;
    
    struct UIDocument : public IAsset
    {
        friend struct UIElementNodeProcessor;

        sg_implement_type_id(UIDocument, 10)

        pugi::xml_document m_document;

        Ref<UIRoot> m_rootElement;
        std::vector<Ref<TemplateElement>> m_templates;

        std::vector<AssetRef<CSSFile>> m_includedCSSFiles;
        std::vector<AssetRef<UIDocument>> m_includedUIDocuments;

        BindingsStorage m_bindingsStorage;

        [[nodiscard]] AssetRef<Style> findStyle(const std::string& selector) const noexcept;
        [[nodiscard]] Ref<UIElement> findElement(const std::string& elementName) const noexcept;
        [[nodiscard]] Ref<TemplateElement> findTemplate(const std::string& templateName) const noexcept;

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

        [[nodiscard]] static Ref<UIElement> processUIElement(UIDocument& inDocument,
                                                             const Ref<UIElement>& parent,
                                                             const pugi::xml_node& xmlNode) noexcept;

        /**
         * @param xmlNode XML node.
         * @return Line and column of xmlNode in XML file.
         */
        std::pair<ptrdiff_t, ptrdiff_t> getLocationInFile(const pugi::xml_node& xmlNode) noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void applyDefaultStylesToNonStyledElements() noexcept;

    private:
        void applyDefaultStylesToNonStyledElementsImpl(const Ref<UIElement>& element) noexcept;

        std::optional<std::string> readXmlFileAndBuildOffsets(const std::filesystem::path& filePath) noexcept;

        std::vector<std::ptrdiff_t> m_debugOffsets;
    };
}

#endif //SUNGEARENGINE_UIDOCUMENT_H
