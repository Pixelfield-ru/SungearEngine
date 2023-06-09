#pragma once

#ifndef SUNGEARENGINE_IUNIFORMBUFFER_H
#define SUNGEARENGINE_IUNIFORMBUFFER_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "IShaderUniform.h"

namespace Core::Graphics::API
{
    struct UniformsDataIterator
    {
        UniformsDataIterator* previous = nullptr;
        UniformsDataIterator* current = nullptr;
        UniformsDataIterator* next = nullptr;

        UniformsDataIterator& operator++() noexcept
        {
            current = next;
            return *current;
        }

        Core::Graphics::API::UniformsDataIterator operator++(int) noexcept;
    };

    class IUniformBuffer : public std::enable_shared_from_this<IUniformBuffer>
    {
    protected:
        char* m_buffer = nullptr;
        UniformsDataIterator m_begin;
        UniformsDataIterator m_end;
        UniformsDataIterator m_current;

        std::uint64_t m_bufferSize = 0;

        std::list<IShaderUniform> m_uniforms;
    public:
        IUniformBuffer() = default;

        explicit IUniformBuffer(std::list<IShaderUniform> uniforms);

        virtual ~IUniformBuffer();

        void putUniforms(std::list<IShaderUniform> uniforms) noexcept;

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
        std::shared_ptr<IUniformBuffer> putData(std::initializer_list<Scalar> scalars)
        {
            UniformsDataIterator currentIter = m_current;
            for(auto scalar : scalars)
            {
                memcpy(currentIter.current, &scalar, sizeof(scalar));
                currentIter.current = reinterpret_cast<UniformsDataIterator*>(reinterpret_cast<char*>(currentIter.current) + sizeof(scalar));
            }
            m_current.current = m_current.next;

            return shared_from_this();
        }

        virtual std::shared_ptr<IUniformBuffer> create() = 0;

        virtual std::shared_ptr<IUniformBuffer> bind() = 0;

        virtual std::shared_ptr<IUniformBuffer> updateLocations(const IShader& fromShader) = 0;

        virtual void destroy() = 0;
    };
}

#endif //SUNGEARENGINE_IUNIFORMBUFFER_H
