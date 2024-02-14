//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_LUT_H
#define SUNGEARENGINE_LUT_H

#include <unordered_map>

namespace SGCore
{
    template<typename InputT, typename OutputT>
    struct LUT
    {
        std::unordered_map<InputT, OutputT> m_lut;
    };
}

#endif //SUNGEARENGINE_LUT_H
