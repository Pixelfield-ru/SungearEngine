//
// Created by stuka on 17.01.2025.
//

#ifndef SUNGEARENGINE_CSSFILE_H
#define SUNGEARENGINE_CSSFILE_H

#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Memory/AssetRef.h"

#include "CSSSelector.h"

#include <antlr4-runtime.h>

#include "SGCore/UI/ANTLR4CSS3Generated/css3Lexer.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3Parser.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3ParserBaseListener.h"

namespace SGCore::UI
{
    struct CSSFile : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(CSSFile, 33)

        friend struct ANTLRCSSListener;

        [[nodiscard]] AssetRef<CSSSelector> findSelector(const std::string& name) const noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        std::vector<AssetRef<CSSSelector>> m_selectors;
    };
}

#endif //SUNGEARENGINE_CSSFILE_H
