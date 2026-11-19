//
// Created by stuka on 18.10.2026.
//

#include <memory>
#include <iostream>
#include <print>
#include <ranges>

#include <sgf/core/thread_pool.h>

#include <sgf/runtime/system/system.h>
#include <sgf/runtime/system/system_graph.h>
#include <sgf/runtime/system/system_scheduler.h>
#include <sgf/runtime/system/frame_time_system.h>

#include <sgf/assets/handle.h>
#include <sgf/assets/server.h>
#include <sgf/assets/std/blob.h>
#include <sgf/assets/std/blob_loader.h>
#include <sgf/assets/std/image.h>
#include <sgf/assets/std/image_loader.h>
#include <sgf/assets/std/png_importer.h>

#include <sgf/serialization/serializer.h>
#include <sgf/serialization/connection.h>
#include <sgf/serialization/types_connection.h>
#include <sgf/serialization/macroses.h>
#include <sgf/serialization/format/json/json_reader.h>
#include <sgf/serialization/format/json/json_writer.h>
#include <sgf/serialization/spec/specs.h>

#include <sgf/platform/window.h>
#include <sgf/platform/platform.h>

#include "platform_polling_system.h"

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
    MyResource resource;

    /*std::byte stack[sgf::core::stack_optimal_size];
    std::pmr::monotonic_buffer_resource stack_resource(stack, sizeof(stack), &resource);
    std::pmr::polymorphic_allocator allocator(&stack_resource);*/
    std::pmr::polymorphic_allocator allocator(&resource);

    auto json_writer = sgf::core::makeUnique<sgf::serialization::JsonWriter>(allocator);
    if(const auto err = json_writer->setSettings({
        .pretty = true
    }))
    {
        return 1;
    }
    auto json_reader = sgf::core::makeUnique<sgf::serialization::JsonReader>(allocator);

    sgf::serialization::Serializer serializer(json_writer.get(), json_reader.get(), allocator);
    sgf::core::ThreadPool thread_pool(std::thread::hardware_concurrency() * 2, allocator);
    sgf::core::LogCallback logger;
    logger.enabled = true;
    logger.function = [](sgf::core::LogLevel level, std::string_view msg, std::source_location caller) {
        std::println("Level {}: {} ({}:{})", std::to_underlying(level), msg, caller.file_name(), caller.line());
    };

    sgf::assets::AssetServer asset_server(allocator);
    sgf::assets::AssetServerCreateInfo asset_server_create_info;
    asset_server_create_info.serializer = serializer;
    asset_server_create_info.thread_pool = &thread_pool;
    asset_server_create_info.debug_output = &logger;

    if(const auto err = sgf::assets::AssetServer::create(asset_server_create_info, asset_server))
    {
        std::println("error: {}", err.message());
        return 1;
    }

    if(const auto err = asset_server.mount("./"))
    {
        std::println("cannot mount asset server: {}", err.message());
        return 1;
    }

    auto blob_loader = sgf::core::makeUnique<sgf::assets::BlobLoader>(allocator);
    auto image_loader = sgf::core::makeUnique<sgf::assets::ImageLoader>(allocator);
    std::ignore = image_loader->addImporter(sgf::core::makeUnique<sgf::assets::PngImporter>(allocator));

    asset_server.storage<sgf::assets::Blob>().setLoader(std::move(blob_loader));
    asset_server.storage<sgf::assets::Image>().setLoader(std::move(image_loader));

    sgf::assets::Handle<sgf::assets::Blob> test_blob;
    if(auto val = asset_server.load<sgf::assets::Blob>("test.txt"))
    {
        test_blob = std::move(*val);
    }
    else
    {
        std::println("cannot load blob: {}", val.error().message());
        return 1;
    }

    sgf::assets::Handle<sgf::assets::Image> test_image;
    if(auto val = asset_server.load<sgf::assets::Image>("hi_flyer.jpg"))
    {
        test_image = std::move(*val);
    }
    else
    {
        std::println("cannot load image: {}", val.error().message());
        return 1;
    }

    // waiting for assets
    test_blob.wait();
    test_image.wait();

    if(test_image.error())
    {
        std::println("Cannot load image: {}", test_image.error().message());
        return 1;
    }

    const sgf::core::String test_str(reinterpret_cast<const char*>(test_blob->bytes.data()),
                                     test_blob->bytes.size(),
                                     test_blob->bytes.get_allocator());

    std::println("test.txt contains: {}", test_str);
    std::println("baking assets...");

    if(const auto err = asset_server.bake(test_image, "test_archive.sga", 12))
    {
        std::println("cannot bake image: {}", err.message());
        return 1;
    }

    if(const auto err = asset_server.bake(test_blob, "test_archive.sga", 12))
    {
        std::println("cannot bake blob: {}", err.message());
        return 1;
    }

    sgf::runtime::SystemContext system_context(allocator);
    auto& system_scheduler = system_context.resources.emplace<sgf::runtime::SystemScheduler>(allocator);

    sgf::core::Vector<sgf::core::UniquePtr<sgf::runtime::ISystem>> systems(allocator);
    systems.push_back(sgf::core::makeUnique<sgf::runtime::FrameTimeSystem>(allocator));
    systems.push_back(sgf::core::makeUnique<PlatformPollingSystem>(allocator));

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

    sgf::runtime::SystemGraph system_graph(allocator);
    if(auto res = sgf::runtime::SystemGraph::build(system_graph_build_info, system_graph))
    {
        return 1;
    }

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

    if(const auto res = sgf::runtime::SystemScheduler::create(scheduler_create_info, system_scheduler))
    {
        return 1;
    }

    // std::println("memory usage: {}", resource.memoryUsage());

    if(const auto err = sgf::platform::platform().init())
    {
        std::println("Cannot init the platform: {}", err.message());
        return 1;
    }

    sgf::platform::Window window0;
    sgf::platform::Window window1;

    sgf::platform::WindowCreateInfo window_create_info;
    window_create_info.debug_output = &logger;
    window_create_info.width = 400;
    window_create_info.height = 400;
    window_create_info.client_api = sgf::platform::WindowClientApi::kNoApi;
    window_create_info.fullscreen = false;
    window_create_info.title = "test0";

    if(const auto err = sgf::platform::Window::create(window_create_info, window0))
    {
        std::println("Cannot create the window: {}", err.message());
        return 1;
    }

    window_create_info.title = "test1";
    if(const auto err = sgf::platform::Window::create(window_create_info, window1))
    {
        std::println("Cannot create the window: {}", err.message());
        return 1;
    }

    system_scheduler.run();

    return 0;
}