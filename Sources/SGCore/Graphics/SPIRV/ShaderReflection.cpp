//
// Created by 8bitniksis on 17.08.2026.
//

#include "ShaderReflection.h"

const SGCore::ShaderReflection::DescriptorBinding* SGCore::ShaderReflection::findBinding(std::string_view name) const noexcept
{
    for(const auto& binding : m_bindings)
    {
        if(binding.m_name == name) return &binding;
    }
    return nullptr;
}

const SGCore::ShaderReflection::DescriptorBinding* SGCore::ShaderReflection::findBinding(std::uint32_t set, std::uint32_t binding) const noexcept
{
    for(const auto& entry : m_bindings)
    {
        if(entry.m_set == set && entry.m_binding == binding) return &entry;
    }
    return nullptr;
}

const SGCore::ShaderReflection::BlockMember* SGCore::ShaderReflection::findMember(std::string_view blockName, std::string_view memberName) const noexcept
{
    const auto* block = findBinding(blockName);
    if(!block) return nullptr;

    for(const auto& member : block->m_members)
    {
        if(member.m_name == memberName) return &member;
    }
    return nullptr;
}
