//
// Created by ilya on 01.03.24.
//

#ifndef SUNGEARENGINE_BATCHINGRANGE_H
#define SUNGEARENGINE_BATCHINGRANGE_H

#include <SGCore/pch.h>

namespace SGCore
{
    struct BatchEntityRanges
    {
        friend struct Batch;
    
    private:
        using uivec2 = glm::vec<2, unsigned int, glm::defaultp>;
        
        uivec2 m_verticesRange { };
        uivec2 m_UVsRange { };
        uivec2 m_indicesRange { };
    };
}

#endif // SUNGEARENGINE_BATCHINGRANGE_H
