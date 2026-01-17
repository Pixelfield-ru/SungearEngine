//
// Created by stuka on 16.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

struct Cart : SGCore::ECS::Component<Cart, const Cart>
{
private:
    bool m_dummy {};
};