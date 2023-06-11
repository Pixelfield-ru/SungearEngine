#pragma once

#ifndef SUNGEARENGINE_IUNIFORMBUFFER_H
#define SUNGEARENGINE_IUNIFORMBUFFER_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <forward_list>

#include "IShaderUniform.h"

namespace Core::Graphics::API
{
    class IUniformBuffer : public std::enable_shared_from_this<IUniformBuffer>
    {
    protected:
        // buffer of scalar values
        char* m_buffer = nullptr;

        // buffer layout needed for pointers to values in buffer
        std::list<char*> m_bufferLayout;
        std::list<char*>::iterator m_currentLayoutPosition = m_bufferLayout.begin();

        std::uint64_t m_bufferSize = 0;
        // the number of uniforms that (scalar data) have been added to the buffer
        std::uint16_t m_filledUniformsNum = 0;

        // uniforms are just description of data
        std::list<IShaderUniform> m_uniforms;
    public:
        IUniformBuffer() = default;

        explicit IUniformBuffer(const std::list<IShaderUniform>& uniforms);

        virtual ~IUniformBuffer();

        void putUniforms(const std::list<IShaderUniform>& uniforms) noexcept;

        /**
         * This method puts ONE uniform`s scalars to buffer.
         * You need to put data strictly in accordance with UNIFORMS TYPES YOU PASSED TO
         * putUniforms (or constructor) (except SGG_STRUCT_START and SGG_STRUCT_END)
         *
         * Notice, that your data is copying to buffer
         *
         * Example:
         *
         * uniformBuffer->putUniforms({
         *          Core::Graphics::API::IShaderUniform("color", SGGDataType::SGG_FLOAT4),
         *          Core::Graphics::API::IShaderUniform("tex", SGGDataType::SGG_INT)
         *          });
         *
         * // for uniform "color"
         * uniformBuffer->putData<float>({ 0.5, 0.1, 0.5, 1.0});
         * // for uniform "tex"
         * uniformBuffer->putData<int>({ 0 });
         *
         * @tparam Scalar Scalars type
         * @param scalars Uniform scalars
         * @return This (shared_ptr)
         */
        template<typename Scalar>
        requires(std::is_scalar_v<Scalar>)
        std::shared_ptr<IUniformBuffer> putData(const std::initializer_list<Scalar>& scalars)
        {
            // local position between two uniforms to put current uniform`s scalar
            char* localPositionPtr = *m_currentLayoutPosition;
            for(auto& scalar : scalars)
            {
                // copying scalar to current position (localPtr)
                memcpy(localPositionPtr, &scalar, sizeof(scalar));
                localPositionPtr += sizeof(scalar);
            }
            // increment iterator
            m_currentLayoutPosition++;
            // increment number of filled uniforms
            m_filledUniformsNum++;

            return shared_from_this();
        }

        virtual std::shared_ptr<IUniformBuffer> create() = 0;

        virtual std::shared_ptr<IUniformBuffer> bind() = 0;

        virtual std::shared_ptr<IUniformBuffer> updateLocations(const IShader& fromShader) = 0;

        virtual void destroy() = 0;
    };
}

#endif //SUNGEARENGINE_IUNIFORMBUFFER_H
