//
// Created by stuka on 14.10.2026.
//

#include "pixelfield/core/utility/assert.h"

#include <iostream>
#include <print>
#include <cstdlib>

void pf::core::impl::assert_(const char* expr, bool expr_eval, const char* file, int line, const char* msg)
{
    if(!expr_eval)
    {
        std::println(std::cerr, "Assert failed:\t {}\nExpected:\t{}\nSource:\t\t{}, line: {}\n",
                     msg, expr, file, line);

        std::abort();
    }
}
