//
// Created by stuka on 30.10.2024.
//

#ifndef SUNGEARENGINE_ASSERT_H
#define SUNGEARENGINE_ASSERT_H

#include <iostream>

/*#ifndef NDEBUG
#   define SG_ASSERT(Expr, Msg) \
    sgAssert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define SG_ASSERT(Expr, Msg)
#endif*/

// i think we must abort in release too
#define SG_ASSERT(Expr, Msg) \
    sgAssert(#Expr, Expr, __FILE__, __LINE__, Msg)

#include "SGCore/Logger/Logger.h"

static void sgAssert(const char* exprStr, bool expr, const char* file, int line, const char* msg)
{
    if(!expr)
    {
        const std::string finalMsg = fmt::format("Assert failed:\t {}\nExpected:\t{}\nSource:\t\t{}, line: {}\n",
                                                 msg, exprStr, file, line);

        std::cerr << finalMsg;

        LOG_C_UNFORMATTED(SGCORE_TAG, finalMsg);

        abort();
    }
}

#endif //SUNGEARENGINE_ASSERT_H
