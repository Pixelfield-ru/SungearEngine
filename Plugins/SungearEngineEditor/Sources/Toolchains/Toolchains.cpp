//
// Created by Ilya on 17.07.2024.
//
#include "Toolchains.h"

SGCore::Ref<SGE::Toolchains> SGE::Toolchains::getInstance() noexcept
{
    return m_instance;
}
