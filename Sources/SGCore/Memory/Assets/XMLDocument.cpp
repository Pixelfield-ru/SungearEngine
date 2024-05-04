//
// Created by ilya on 23.02.24.
//

#include "XMLDocument.h"
#include <spdlog/spdlog.h>

void SGCore::XMLDocument::doLoad(const std::string& path)
{
    pugi::xml_parse_result parseResult = m_document.load_file(path.c_str());
    
    if(!parseResult)
    {
        spdlog::error("Error while parsing XML document by path '{0}': {1}", path, parseResult.description());
        return;
    }
}

