//
// Created by ilya on 23.02.24.
//

#include "XMLDocument.h"
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

void SGCore::XMLDocument::doLoad(const std::string& path)
{
    pugi::xml_parse_result parseResult = m_document.load_file(path.c_str());
    
    if(!parseResult)
    {
        LOG_E(SGCORE_TAG, "Error while parsing XML document by path '{}': {}", path, parseResult.description());
        return;
    }
}

