#ifndef SUNGEARENGINE_IUNIFORMBUFFER_H
#define SUNGEARENGINE_IUNIFORMBUFFER_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <forward_list>
#include <cstring>

#include "IShaderUniform.h"
#include "SGCore/Logging/Log.h"
#include "IShader.h"

namespace SGCore
{
    class IShader;

    class IUniformBuffer : public std::enable_shared_from_this<IUniformBuffer>
    {
    protected:
        // buffer of scalar values
        char* m_buffer = nullptr;

        std::uint64_t m_bufferSize = 0;

        // uniforms are just description of data
        std::vector<IShaderUniform> m_uniforms;

        // location in shader
        std::uint16_t m_layoutLocation = 0;

        virtual void subDataOnGAPISide(const std::int64_t& offset, const int& size) = 0;
    public:
        // name of block in shader
        std::string m_blockName;

        IUniformBuffer() = default;

        template<typename ShaderUniform = IShaderUniform>
        explicit IUniformBuffer(std::vector<IShaderUniform>&& uniforms)
        {
            putUniforms(std::forward<ShaderUniform>(uniforms));
        }

        virtual ~IUniformBuffer();

        template<typename ShaderUniform = IShaderUniform>
        void putUniforms(std::vector<ShaderUniform>&& uniforms) noexcept
        {
            for(auto& uniform : uniforms)
            {
                m_uniforms.emplace_back(std::forward<ShaderUniform>(uniform));
            }

            std::uint64_t lastBufferSize = m_bufferSize;
            m_bufferSize = 0;
            // first iteration - prepare
            for(auto& uniform : m_uniforms)
            {
                uniform.m_dataSizeInUniformBuffer = getSGGDataTypeAlignedSizeInBytes(uniform.m_dataType);
                uniform.m_offsetInUniformBuffer = m_bufferSize;

                m_bufferSize += uniform.m_dataSizeInUniformBuffer;
            }

            //TODO: (possible) to add more memory allocation so as not to allocate frequently. allow only when there is not enough space

            // if buffer is not allocated (first put)
            if(!m_buffer)
            {
                // allocating buffer
                m_buffer = static_cast<char*>(malloc(m_bufferSize));

                char* curPtr = m_buffer;
                for(auto& uniform: m_uniforms)
                {
                    curPtr += uniform.m_dataSizeInUniformBuffer;
                }
            }
            else // if buffer already allocated (second etc. allocations)
            {
                char* newBuffer = static_cast<char*>(malloc(m_bufferSize));
                // copy last buffer to new buffer
                memcpy(newBuffer, m_buffer, lastBufferSize);
                // delete last buffer
                delete m_buffer;
                m_buffer = newBuffer;

                // updating buffer layout
                char* curPtr = m_buffer;
                for(auto& uniform: m_uniforms)
                {
                    curPtr += uniform.m_dataSizeInUniformBuffer;
                }
            }

            SGCF_INFO("uniforms buffer size: " + std::to_string(m_bufferSize), SG_LOG_CURRENT_SESSION_FILE);
        }

        /**
         * Method that allows you change some uniform`s data.\n
         * This method is slower than the putData method, but through putData you cannot substitute data of any uniform.\n
         * \n
         * Notice, that your data is copying to buffer.\n
         * Notice, that you do not need to call prepare after subData.\n
         *
         * @tparam Scalar - Scalars type
         * @param uniformName - The name of the uniform whose values need to be replaced
         * @param scalars - Values (buffer)
         * @param scalarsNum - Number of values in the buffer
         * @return This
         */
        template<typename Scalar>
        requires(std::is_scalar_v<Scalar>)
        std::shared_ptr<IUniformBuffer> subData(const std::string& uniformName, const Scalar* scalars, const int& scalarsNum)
        {
            const auto& foundUniformIter = std::find_if(m_uniforms.begin(), m_uniforms.end(), [&uniformName](const auto& shaderUniform) { return shaderUniform.m_name == uniformName; });
            /*if(foundUniformIter == m_uniforms.end())
            {
                SGCF_ERROR("Unable to subData for uniform \"" + uniformName + "\" in the uniform buffer. Uniform with that name was not found.", SG_LOG_CURRENT_SESSION_FILE);
            }*/
            if(foundUniformIter != m_uniforms.end())
            {
                const auto& uniform = *foundUniformIter;

                // uniform-local pointer to put scalars
                char* uniformScalarPtr = m_buffer + uniform.m_offsetInUniformBuffer;

                for(int i = 0; i < scalarsNum; i++)
                {
                    // copying scalar to current position (uniformScalarPtr)
                    std::memcpy(uniformScalarPtr, &scalars[i], sizeof(scalars[i]));
                    // offset
                    uniformScalarPtr += sizeof(scalars[i]);
                }

                // updating data on graphics api side
                subDataOnGAPISide((m_buffer + uniform.m_offsetInUniformBuffer) - m_buffer, uniform.m_dataSizeInUniformBuffer);
            }

            return shared_from_this();
        }

        /**
         * @see subData(const std::string& uniformName, const Scalar* scalars, const int& scalarsNum)
         *
         * @tparam Scalar - Scalars type
         * @param uniformName - The name of the uniform whose values need to be replaced
         * @param scalars - Values
         * @return This
         */
        template<typename Scalar>
        requires(std::is_scalar_v<Scalar>)
        std::shared_ptr<IUniformBuffer> subData(const std::string& uniformName, const std::initializer_list<Scalar>& scalars)
        {
            return subData(uniformName, data(scalars), scalars.size());
        }

        virtual std::shared_ptr<IUniformBuffer> bind() = 0;

        /**
         * Must be called after puData
         * Will reallocate buffer to resize. Be careful
         * @return This
         */
        virtual std::shared_ptr<IUniformBuffer> prepare() = 0;

        /**
         * Updates the locations of all uniforms of the shader
         * @param fromShader - The shader from which all locations will be updated
         * @return This
         */
        virtual std::shared_ptr<IUniformBuffer> updateLocations(const IShader& fromShader) = 0;

        std::uint16_t getLayoutLocation() const noexcept;
        /**
         * SET LOCATION ONLY BEFORE FIRST PREPARE
         * @param location
         */
        virtual void setLayoutLocation(const std::uint16_t& location) noexcept = 0;

        bool isBindPerUniform() const noexcept;

        /**
         * Destroys GPU object
         */
        virtual void destroy() = 0;
    };
}

#endif //SUNGEARENGINE_IUNIFORMBUFFER_H
