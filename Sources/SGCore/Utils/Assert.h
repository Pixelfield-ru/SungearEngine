//
// Created by stuka on 30.10.2024.
//

#pragma once

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
        SG_LOG_C("Assert failed:\t {}\nExpected:\t{}\nSource:\t\t{}, line: {}\n",
                 msg, exprStr, file, line);

        abort();
    }
}
