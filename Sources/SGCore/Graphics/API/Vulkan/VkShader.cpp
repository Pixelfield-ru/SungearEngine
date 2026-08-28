//
// Created by stuka on 07.07.2023.
//

#include "VkShader.h"

#include "RHI/VulkanDescriptorSet.h"
#include "VkRenderer.h"

void SGCore::VkShader::setAsCurrentShader() const noexcept
{
    if(const auto& renderer = VkRenderer::getInstance())
    {
        renderer->setCurrentLegacyShader(const_cast<VkShader*>(this));
    }
}

void SGCore::VkShader::fillBackendDescriptors(IDescriptorSet& set) noexcept
{
    // A draw whose declared descriptor was never written is undefined behaviour and the driver may
    // drop it outright, so every texel buffer the program declares gets at least the dummy: the
    // passes bind a real one only for animated meshes (u_bonesMatricesUniformBuffer).
    const auto view = VkRenderer::getInstance()->getDummyTexelBufferView();
    if(view == VK_NULL_HANDLE) return;

    for(const auto& binding : getReflection().m_bindings)
    {
        if(binding.m_type != ShaderDescriptorType::UNIFORM_TEXEL_BUFFER &&
           binding.m_type != ShaderDescriptorType::STORAGE_TEXEL_BUFFER) continue;

        static_cast<VulkanDescriptorSet&>(set).setTexelBuffer(binding.m_binding, view);
    }
}
