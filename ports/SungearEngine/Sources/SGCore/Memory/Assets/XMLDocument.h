//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_XMLDOCUMENT_H
#define SUNGEARENGINE_XMLDOCUMENT_H

#include "IAsset.h"
#include <pugixml.hpp>

namespace SGCore
{
    struct XMLDocument : public IAsset
    {
        pugi::xml_document m_document;
        
        void load(const std::string& path) override;
    };
}

#endif //SUNGEARENGINE_XMLDOCUMENT_H
