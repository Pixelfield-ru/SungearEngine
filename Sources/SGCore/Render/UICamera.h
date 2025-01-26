//
// Created by ilya on 25.02.24.
//

#ifndef SUNGEARENGINE_UICAMERA_H
#define SUNGEARENGINE_UICAMERA_H

namespace SGCore
{
    class UICamera : public ECS::Component<UICamera, const UICamera>
    {
        volatile int m_dummy = 0;
    };
}

#endif //SUNGEARENGINE_UICAMERA_H
