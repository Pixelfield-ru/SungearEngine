//
// Created by Ilya on 17.07.2024.
//
#include "Toolchains.h"

std::vector<SGCore::Ref<SGE::Toolchain>> SGE::Toolchains::getToolchains() noexcept
{
    return m_toolchains;
}

