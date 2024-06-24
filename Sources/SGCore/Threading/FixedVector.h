//
// Created by ilya on 08.03.24.
//

#ifndef SUNGEARENGINE_FIXEDVECTOR_H
#define SUNGEARENGINE_FIXEDVECTOR_H

#include <SGCore/pch.h>

namespace SGCore
{
    template<typename T>
    struct FixedVector
    {
        std::atomic<size_t> m_vectorLastSize = 0;
        std::vector<T> m_vector;
    };
}

#endif //SUNGEARENGINE_FIXEDVECTOR_H
