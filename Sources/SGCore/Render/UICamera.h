//
// Created by ilya on 25.02.24.
//

#pragma once

namespace SGCore
{
    class SGCORE_EXPORT UICamera : public ECS::Component<UICamera, const UICamera>
    {
        volatile int m_dummy = 0;
    };
}
