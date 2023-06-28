#pragma once

#ifndef SUNGEARENGINE_IUNIFORMBUFFER_H
#define SUNGEARENGINE_IUNIFORMBUFFER_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <forward_list>

#include "IShaderUniform.h"
#include "SGCore/Logging/Log.h"

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

        virtual void subDataOnGAPISide(const std::int64_t& offset, const int& size) = 0;
    public:
        IUniformBuffer() = default;

        explicit IUniformBuffer(const std::list<IShaderUniform>& uniforms);

        virtual ~IUniformBuffer();

        void putUniforms(const std::list<IShaderUniform>& uniforms) noexcept;
        /**
         * This method puts ONE uniform`s scalars to buffer\n
         * You need to put data strictly in accordance with UNIFORMS TYPES YOU PASSED TO
         * putUniforms (or constructor) (except SGG_STRUCT_START and SGG_STRUCT_END).\n
         * You can put zero scalars (just { }). This will give extra speed.\n
         * \n
         * For example: you can put data with more than zero scalars for constant uniforms or for uniforms with a long update time interval.
         * And for dynamically changing uniforms, you can put zero scalars.\n
         * \n
         * After put data you need to call prepare method and then you can use this buffer.\n
         * \n
         * This method recommended to use this method only after calling putUniforms.\n
         * This method is faster than subData, but you cannot use putData instead of subData.\n
         * \n
         * Notice, that your data is copying to buffer.\n
         *
         * @tparam Scalar - Scalars type
         * @param scalars - Uniform scalars
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
         * @return This (shared_ptr)
         */
        template<typename Scalar>
        requires(std::is_scalar_v<Scalar>)
        std::shared_ptr<IUniformBuffer> subData(const std::string& uniformName, const Scalar* scalars, const int& scalarsNum)
        {
            // bool to mark whether a uniform has been found
            bool uniformFound = false;
            // pointer to uniform in buffer
            char* uniformPtr = m_buffer;

            // aligned byte size of found uniform
            int uniformAlignedByteSize = 0;

            for(auto& uniform : m_uniforms)
            {
                // when the uniform was found
                if(uniform.m_name == uniformName)
                {
                    uniformAlignedByteSize = getSGGDataTypeAlignedSizeInBytes(uniform.m_dataType);
                    uniformFound = true;

                    break;
                }
                else
                {
                    // adding offset until we reach the uniform
                    uniformPtr += getSGGDataTypeAlignedSizeInBytes(uniform.m_dataType);
                }
            }

            // if not found then error
            if(!uniformFound)
            {
                SGCF_ERROR("Unable to subData for uniform \"" + uniformName + "\" in the uniform buffer. Uniform with that name was not found.", SG_LOG_CURRENT_SESSION_FILE);

                return shared_from_this();
            }

            // uniform-local pointer to put scalars
            char* uniformScalarPtr = uniformPtr;

            for(int i = 0; i < scalarsNum; i++)
            {
                // copying scalar to current position (uniformScalarPtr)
                memcpy(uniformScalarPtr, &scalars[i], sizeof(scalars[i]));
                // offset
                uniformScalarPtr += sizeof(scalars[i]);
            }

            // updating data on graphics api side
            subDataOnGAPISide(uniformPtr - m_buffer, uniformAlignedByteSize);

            return shared_from_this();
        }

        /**
         * @see std::shared_ptr<IUniformBuffer> subData(const std::string& uniformName, const Scalar* scalars, const int& scalarsNum)
         *
         * @tparam Scalar - Scalars type
         * @param uniformName - The name of the uniform whose values need to be replaced
         * @param scalars - Values
         * @return This (shared_ptr)
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
         * @return this
         */
        virtual std::shared_ptr<IUniformBuffer> prepare() = 0;

        virtual std::shared_ptr<IUniformBuffer> updateLocations(const IShader& fromShader) = 0;

        virtual void destroy() = 0;
    };
}

#endif //SUNGEARENGINE_IUNIFORMBUFFER_H
