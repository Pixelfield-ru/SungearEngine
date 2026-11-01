//
// Created by stuka on 18.10.2026.
//

#include <memory>
#include <iostream>
#include <print>
#include <ranges>

#include <sgf/runtime/system/system.h>
#include <sgf/runtime/system/system_graph.h>
#include <sgf/runtime/system/system_scheduler.h>
#include <sgf/runtime/system/frame_time_system.h>
#include <sgf/runtime/execution/thread_pool.h>

#include <sgf/assets/asset_handle.h>
#include <sgf/assets/asset_server.h>

#include <sgf/serialization/serializer.h>
#include <sgf/serialization/connection.h>
#include <sgf/serialization/types_connection.h>
#include <sgf/serialization/macroses.h>
#include <sgf/serialization/format/json/json_reader.h>
#include <sgf/serialization/format/json/json_writer.h>
#include <sgf/serialization/spec/specs.h>

struct Base
{
    int a { 1 };

    virtual ~Base() = default;

    virtual void foo()
    {
        std::println("hello base");
    }
};

struct Derived final : Base
{
    int b { 2 };
    sgf::core::UnorderedMap<int, sgf::core::String> c { { 0, "value" } };

    void foo() override
    {
        std::println("hello base");
    }
};

namespace sgf::serialization
{
    template<>
    struct SerializerSpec<Base> final
    {
        SGF_SERIALIZER_TYPE_NAME(Base)

        template<typename... ArgsT>
        static std::error_code serialize(Serializer& serializer, const Base& value, ArgsT&&... args)
        {
            return serializer.serializeField("a", value.a);
        }

        template<typename... ArgsT>
        static std::error_code deserialize(Serializer& serializer, Base& value, ArgsT&&... args)
        {
            if(const auto err = serializer.deserializeField("a", value.a)) return err;

            return {};
        }
    };
}

namespace sgf::serialization
{
    template<>
    struct SerializerSpec<Derived> final : core::base_types<Base>
    {
        SGF_SERIALIZER_TYPE_NAME(Derived)

        template<typename... ArgsT>
        static std::error_code serialize(Serializer& serializer, const Derived& value, ArgsT&&... args)
        {
            if(const auto err = serializer.serializeField("b", value.b)) return err;
            if(const auto err = serializer.serializeField("c", value.c)) return err;
            std::println("args count: {}", sizeof...(args));

            return {};
        }

        template<typename... ArgsT>
        static std::error_code deserialize(Serializer& serializer, Derived& value, ArgsT&&... args)
        {
            if(const auto err = serializer.deserializeField("b", value.b)) return err;
            if(const auto err = serializer.deserializeField("c", value.c)) return err;
            std::println("args count: {}", sizeof...(args));

            return {};
        }
    };
}

SGF_SERIALIZER_ADD_CONNECTION(Base, Derived)
SGF_SERIALIZER_ADD_CONNECTION(Base, Derived, int)

struct MyResource final : std::pmr::memory_resource
{
    size_t memoryUsage() const noexcept
    {
        return mem_usage_;
    }

private:
    void* do_allocate(size_t bytes, size_t alignment) override
    {
        std::println("allocated {} bytes with alignment {}", bytes, alignment);

        mem_usage_ += bytes;

        if(alignment <= alignof(std::max_align_t))
        {
            return ::operator new(bytes);
        }

        return ::operator new(bytes, std::align_val_t{alignment});
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override
    {
        std::println("deallocated {} bytes with alignment {}", bytes, alignment);

        if(alignment <= alignof(std::max_align_t))
        {
            ::operator delete(p);
        }
        else
        {
            ::operator delete(p, std::align_val_t{alignment});
        }

        mem_usage_ -= bytes;
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }

    size_t mem_usage_ {};
};

int main()
{
    sgf::assets::Handle<int> handle;
    sgf::assets::WeakHandle<int> weak_handle = handle;
    sgf::assets::AnyHandle any_handle = handle;
    sgf::assets::AnyWeakHandle any_weak_handle = any_handle;
    any_handle = weak_handle.lock();
    any_weak_handle = any_handle;
    handle = weak_handle.lock();
    any_handle = any_weak_handle.lock();
    handle = any_handle.as<int>();

    sgf::assets::AssetServer asset_server(24);
    auto handle0 = asset_server.find(0, sgf::core::typeIndex<int>());
    if(!handle0)
    {
        std::cerr << "handle is invalid" << std::endl;
    }

    MyResource resource;
    std::pmr::polymorphic_allocator allocator(&resource);

    sgf::core::LogCallback logger;
    logger.enabled = true;
    logger.function = [](sgf::core::LogLevel level, std::string_view msg) {
        std::println("Level {}: {}", std::to_underlying(level), msg);
    };

    sgf::runtime::SystemContext system_context(&resource);
    system_context.resources.emplace<std::chrono::steady_clock::time_point>(std::chrono::steady_clock::now());

    sgf::core::Vector<sgf::core::UniquePtr<sgf::runtime::ISystem>> systems(&resource);
    systems.push_back(sgf::core::makeUnique<sgf::runtime::FrameTimeSystem>(&resource));

    for(const auto& system : systems)
    {
        if(const auto res = system->initialize(system_context))
        {
            return 1;
        }
    }

    const auto systems_pointer = systems
    | std::views::transform([](const auto& system) { return system.get(); })
    | std::ranges::to<std::vector>();

    sgf::runtime::SystemGraphBuildInfo system_graph_build_info;
    system_graph_build_info.context = &system_context;
    system_graph_build_info.debug_output = &logger;
    system_graph_build_info.systems = systems_pointer;

    sgf::runtime::SystemGraph system_graph(&resource);
    if(auto res = sgf::runtime::SystemGraph::build(system_graph_build_info, system_graph))
    {
        return 1;
    }

    sgf::runtime::ThreadPool thread_pool(std::thread::hardware_concurrency() * 2);

    sgf::runtime::SystemSchedulerCreateInfo scheduler_create_info;
    scheduler_create_info.context = &system_context;
    scheduler_create_info.debug_output = &logger;
    scheduler_create_info.graph = &system_graph;
    scheduler_create_info.thread_pool = &thread_pool;
    sgf::runtime::SystemScheduleState schedule;
    schedule.type = sgf::runtime::kFrame;
    // 120 frames per seconds
    schedule.interval = std::chrono::nanoseconds(1'000'000'000 / 120);
    scheduler_create_info.schedules.push_back(schedule);

    sgf::runtime::SystemScheduler system_scheduler(&resource);
    if(const auto res = sgf::runtime::SystemScheduler::create(scheduler_create_info, system_scheduler))
    {
        return 1;
    }

    // system_scheduler.run();
    if(const auto res = system_scheduler.runOnce())
    {
        return 1;
    }

    auto json_writer = sgf::core::makeUnique<sgf::serialization::JsonWriter>(allocator);
    if(const auto err = json_writer->setSettings({
        .pretty = true
    }))
    {
        return 1;
    }
    auto json_reader = sgf::core::makeUnique<sgf::serialization::JsonReader>(allocator);

    sgf::serialization::Serializer serializer(json_writer.get(), json_reader.get(), allocator);

    sgf::core::SharedPtr<Base> base = sgf::core::makeShared<Derived>(allocator);
    base->a = 20;
    static_cast<Derived*>(base.get())->c[1] = "2_test";

    if(const auto err = serializer.serialize(base.get(), 9))
    {
        std::println("Error in serialization: {}", err.message());
        return 1;
    }

    std::println("serialized data:\n{}", serializer.writer()->data());

    if(const auto err = serializer.reader()->parse(serializer.writer()->data()))
    {
        std::println("Cannot parse: {}", err.message());
        return 1;
    }

    sgf::core::SharedPtr<Base> deser {};
    if(const auto err = serializer.deserialize(deser, 3))
    {
        std::println("Cannot deserialize: {}", err.message());
        return 1;
    }

    std::println("Deserialized: {}", typeid(*deser).name());
    std::println("a is: {}", deser->a);

    std::println("memory usage: {}", resource.memoryUsage());

    return 0;
}