//
// Created by 8bitniksis on 17.08.2026.
//

#include "GL46PipelineState.h"

#include "GL46ProgramBase.h"
#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/Logger/Logger.h"

namespace
{
    bool isIntegerType(SGGDataType type) noexcept
    {
        switch(type)
        {
            case SGGDataType::SGG_INT: case SGGDataType::SGG_INT2: case SGGDataType::SGG_INT3: case SGGDataType::SGG_INT4:
            case SGGDataType::SGG_UNSIGNED_INT: case SGGDataType::SGG_SHORT: case SGGDataType::SGG_UNSIGNED_SHORT:
            case SGGDataType::SGG_BYTE: case SGGDataType::SGG_UNSIGNED_BYTE:
                return true;
            default:
                return false;
        }
    }
}

SGCore::GL46PipelineState::GL46PipelineState(const PipelineStateDesc& desc) noexcept
{
    m_desc = desc;
    m_debugName = desc.m_debugName;
    m_drawMode = GLGraphicsTypesCaster::sggDrawModeToGL(desc.m_meshRenderState.m_drawMode);

    glCreateVertexArrays(1, &m_vertexArray);

    for(const auto& slot : desc.m_vertexInput.m_slots)
    {
        glVertexArrayBindingDivisor(m_vertexArray, slot.m_slot, slot.m_perInstance ? 1 : 0);
    }

    for(const auto& attribute : desc.m_vertexInput.m_attributes)
    {
        glEnableVertexArrayAttrib(m_vertexArray, attribute.m_location);

        const GLenum glType = GLGraphicsTypesCaster::sggDataTypeToGL(attribute.m_dataType);
        if(isIntegerType(attribute.m_dataType) && !attribute.m_normalized)
        {
            glVertexArrayAttribIFormat(m_vertexArray, attribute.m_location, static_cast<GLint>(attribute.m_componentsCount), glType, attribute.m_offset);
        }
        else
        {
            glVertexArrayAttribFormat(m_vertexArray, attribute.m_location, static_cast<GLint>(attribute.m_componentsCount), glType,
                                      attribute.m_normalized ? GL_TRUE : GL_FALSE, attribute.m_offset);
        }
        glVertexArrayAttribBinding(m_vertexArray, attribute.m_location, attribute.m_bufferSlot);
    }

    if(!m_debugName.empty())
    {
        glObjectLabel(GL_VERTEX_ARRAY, m_vertexArray, static_cast<GLsizei>(m_debugName.size()), m_debugName.c_str());
    }
}

SGCore::GL46PipelineState::~GL46PipelineState()
{
    if(m_vertexArray) glDeleteVertexArrays(1, &m_vertexArray);
}

GLuint SGCore::GL46PipelineState::getProgram() const noexcept
{
    const auto* program = static_cast<const GL46ProgramBase*>(m_desc.m_program.get());
    return program ? program->getHandle() : 0;
}

std::uint32_t SGCore::GL46PipelineState::getSlotStride(std::uint32_t slot) const noexcept
{
    for(const auto& description : m_desc.m_vertexInput.m_slots)
    {
        if(description.m_slot == slot) return description.m_stride;
    }
    return 0;
}
