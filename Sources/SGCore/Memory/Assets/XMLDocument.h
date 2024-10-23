//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_XMLDOCUMENT_H
#define SUNGEARENGINE_XMLDOCUMENT_H

#include "IAsset.h"

namespace SGCore
{
    struct XMLDocument : public IAsset
    {
        static inline size_t asset_type_id = StaticTypeID<XMLDocument>::setID(10);

        pugi::xml_document m_document;
        
    protected:
        void doLoad(const std::filesystem::path& path) override;
    };
}

#endif //SUNGEARENGINE_XMLDOCUMENT_H
