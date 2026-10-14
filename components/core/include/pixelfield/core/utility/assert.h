//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <cassert>
#include <source_location>

#ifndef NDEBUG
#define pf_assert(expr, msg) \
    pf::core::impl::assert_(#expr, expr, __FILE__, __LINE__, msg)
#else
#define pf_assert(expr, msg) ((void)0)
#endif

namespace pf::core::impl
{
    void assert_(const char* expr, bool expr_eval, const char* file, int line, const char* msg);
}