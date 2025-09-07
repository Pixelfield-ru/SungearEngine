//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include <glm/glm.hpp>

namespace SGCore::Serde
{
    // ======================================================== impl glm::vec

    template<glm::length_t Length, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    void SerdeSpec<glm::vec<Length, T, Qualifier>, TFormatType>::serialize(SerializableValueView<const glm::vec<Length, T, Qualifier>, TFormatType>& valueView) noexcept
    {
        valueView.container().setAsArray();

        for(std::size_t i = 0; i < Length; ++i)
        {
            valueView.container().pushBack((*valueView.m_data)[i]);
        }
    }

    template<glm::length_t Length, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    void SerdeSpec<glm::vec<Length, T, Qualifier>, TFormatType>::deserialize(DeserializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView) noexcept
    {
        const std::vector<T> vec =
                valueView.container().template getAsArray<T>();

        for(std::size_t i = 0; i < Length; ++i)
        {
            (*valueView.m_data)[i] = vec[i];
        }
    }

    // ======================================================== impl glm::qua

    template<typename T, glm::qualifier Qualifier, FormatType TFormatType>
    void SerdeSpec<glm::qua<T, Qualifier>, TFormatType>::serialize(SerializableValueView<const glm::qua<T, Qualifier>, TFormatType>& valueView) noexcept
    {
        valueView.container().setAsArray();

        valueView.container().pushBack(valueView.m_data->x);
        valueView.container().pushBack(valueView.m_data->y);
        valueView.container().pushBack(valueView.m_data->z);
        valueView.container().pushBack(valueView.m_data->w);
    }

    template<typename T, glm::qualifier Qualifier, FormatType TFormatType>
    void SerdeSpec<glm::qua<T, Qualifier>, TFormatType>::deserialize(DeserializableValueView<glm::qua<T, Qualifier>, TFormatType>& valueView) noexcept
    {
        const std::vector<T> vec =
                valueView.container().template getAsArray<T>();

        valueView.m_data->x = vec[0];
        valueView.m_data->y = vec[1];
        valueView.m_data->z = vec[2];
        valueView.m_data->w = vec[3];
    }

    // ======================================================== impl glm::mat

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    void SerdeSpec<glm::mat<C, R, T, Qualifier>, TFormatType>::serialize(SerializableValueView<const glm::mat<C, R, T, Qualifier>, TFormatType>& valueView) noexcept
    {
        valueView.container().setAsArray();

        for(std::size_t c = 0; c < C; ++c)
        {
            for(std::size_t r = 0; r < R; ++r)
            {
                valueView.container().pushBack((*valueView.m_data)[c][r]);
            }
        }
    }

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    void SerdeSpec<glm::mat<C, R, T, Qualifier>, TFormatType>::deserialize(DeserializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView) noexcept
    {
        const std::vector<T> vec =
                valueView.container().template getAsArray<T>();

        size_t i = 0;
        for(std::size_t c = 0; c < C; ++c)
        {
            for(std::size_t r = 0; r < R; ++r)
            {
                (*valueView.m_data)[c][r] = vec[i];
                ++i;
            }
        }
    }
}