//
// Created by ilya on 23.02.24.
//

#include "XMLDocument.h"
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

void SGCore::XMLDocument::doLoad(const InterpolatedPath& path)
{
    const std::string u8Path = Utils::toUTF8(path.resolved().u16string());

    pugi::xml_parse_result parseResult = m_document.load_file(u8Path.c_str());
    
    if(!parseResult)
    {
        LOG_E(SGCORE_TAG, "Error while parsing XML document by path '{}': {}", u8Path, parseResult.description());
        return;
    }
}

void SGCore::XMLDocument::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

