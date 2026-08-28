//
// Created by 8bitniksis on 17.08.2026.
//

#include "GL46Device.h"

#include <glad/glad.h>

#include "GL46CommandList.h"
#include "GL46DescriptorSet.h"
#include "GL46GPUBuffer.h"
#include "GL46PipelineState.h"
#include "GL46ShaderProgram.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Logger/Logger.h"

SGCore::GL46Device::GL46Device(GL4Renderer& renderer) noexcept : m_renderer(renderer)
{
    m_properties.m_apiType = renderer.getGAPIType();
    m_properties.m_originBottomLeft = true;
    m_properties.m_depthZeroToOne = false;
    m_properties.m_ndcYFlipRequired = false;
    m_properties.m_framesInFlight = 1;
    m_properties.m_pushConstantsMaxSize = 0;
    m_properties.m_supportsExplicitBarriers = false;
    m_properties.m_supportsMultithreadedRecording = false;
    m_properties.m_supportsBindless = false;
}

SGCore::Ref<SGCore::IGPUBuffer> SGCore::GL46Device::createBuffer(const GPUBufferDesc& desc) noexcept
{
    return MakeRef<GL46GPUBuffer>(desc);
}

SGCore::Ref<SGCore::IShaderProgram> SGCore::GL46Device::createShaderProgram(const ShaderProgramDesc& desc) noexcept
{
    return MakeRef<GL46ShaderProgram>(desc);
}

SGCore::Ref<SGCore::IDescriptorSet> SGCore::GL46Device::createDescriptorSet() noexcept
{
    return MakeRef<GL46DescriptorSet>();
}

SGCore::Ref<SGCore::IPipelineState> SGCore::GL46Device::getOrCreatePipeline(const PipelineStateDesc& desc) noexcept
{
    auto& bucket = m_pipelineCache[desc.hash()];
    for(const auto& existing : bucket)
    {
        if(existing->getDesc().equalsForCache(desc)) return existing;
    }
    auto pipeline = MakeRef<GL46PipelineState>(desc);
    bucket.push_back(pipeline);
    return pipeline;
}

SGCore::Ref<SGCore::ICommandList> SGCore::GL46Device::createCommandList() noexcept
{
    return MakeRef<GL46CommandList>(m_renderer);
}

void SGCore::GL46Device::submit(const Ref<ICommandList>& /*commandList*/) noexcept
{
    // already executed while recording
}

void SGCore::GL46Device::destroyDeferred(Ref<IGPUObject> /*object*/) noexcept
{
    // dropping the reference is enough on GL: the driver defers the actual release itself
}

void SGCore::GL46Device::waitIdle() noexcept
{
    glFinish();
}
