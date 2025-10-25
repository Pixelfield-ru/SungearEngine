//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_IGEOMETRYPASS_H
#define SUNGEARENGINE_IGEOMETRYPASS_H

#include "SGCore/Render/IRenderPass.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct IGeometryPass : public IRenderPass
    {
    private:
        bool m_dummy = false;
    };
}

#endif //SUNGEARENGINE_IGEOMETRYPASS_H
