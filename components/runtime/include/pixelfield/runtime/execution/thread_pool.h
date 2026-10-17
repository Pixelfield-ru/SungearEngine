//
// Created by stuka on 17.10.2026.
//

#pragma once

#include <optional>
#include <boost/asio/thread_pool.hpp>

namespace pf::runtime
{
    struct ThreadPool final
    {
        explicit ThreadPool(std::size_t thread_count);
        ThreadPool() noexcept = default;

        void post(std::move_only_function<void()> task);
        void wait();

    private:
        boost::asio::thread_pool pool_;
    };
}