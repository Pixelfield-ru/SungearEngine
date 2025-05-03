//
// Created by stuka on 03.05.2025.
//

#ifndef SUNGEARENGINE_IDECALSPASS_H
#define SUNGEARENGINE_IDECALSPASS_H

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct IDecalsPass : public IRenderPass
    {
    private:
        bool m_dummy = false;
    };
}

#endif // SUNGEARENGINE_IDECALSPASS_H
