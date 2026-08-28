//
// Created by 8bitniksis on 16.08.2026.
//

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <vector>

#include "SGCore/Main/BasicApp.h"

namespace SGSmoke
{
    struct SmokeOptions
    {
        /// Frame index at which the frame is captured. Early frames may still be warming up
        /// (shader compilation, first CSM update), so the default leaves a margin.
        std::uint32_t m_captureFrame = 60;
        /// Output PNG. If empty, "smoke_<gapi>.png" in the working directory is used.
        std::filesystem::path m_outputPath;
        /// If set, the captured frame is compared against this PNG and the exit code reflects the result.
        std::optional<std::filesystem::path> m_referencePath;
        /// Any channel differing by more than this value marks a pixel as different.
        std::uint8_t m_channelThreshold = 8;
        /// Test fails if the fraction of differing pixels exceeds this value.
        double m_maxDifferingPixelsFraction = 0.01;
        /// Capture the geometry pass output (m_layersFrameBuffer) instead of the post-processed one
        /// (m_layersFXFrameBuffer). Localizes at which stage two backends start to disagree.
        bool m_captureGeometryPass = false;
        /// Color attachment index to read back; the default is the one the app displays.
        std::optional<std::uint32_t> m_captureAttachment;
        /// Ask RenderDoc to capture exactly the frame this test reads back (needs the process to run
        /// under RenderDoc: renderdoccmd capture / the GUI).
        bool m_renderDocCapture { };
        /// Put the opaque meshes into a Batch instead of drawing them one by one. This is the only
        /// coverage the batching subsystem has: nothing else in the repository ever fills a Batch,
        /// and SunShadowsPass draws Batch entities exclusively, so shadows only appear in this mode.
        /// The frame differs from the default scene, so it has its own reference image.
        bool m_useBatching { };
        /// Draw the opaque meshes through Instancing instead of one by one. Like m_useBatching, this is
        /// the only coverage the instancing subsystem has: nothing else in the repository ever calls
        /// AutoInstancing::addEntity. Has its own reference image.
        bool m_useInstancing { };
    };

    /**
     * Reference scene for comparing graphics backends: PBR meshes, sun shadows (CSM), atmosphere,
     * transparent object, SSAO. Renders a fixed number of frames, reads the final camera attachment
     * back and writes it to PNG; optionally compares with a reference image.
     */
    struct SmokeApp : SGCore::BasicApp
    {
        explicit SmokeApp(SmokeOptions options) noexcept;

        [[nodiscard]] int getExitCode() const noexcept;

        void onInit() noexcept override;
        void onUpdate(double dt, double fixedDt) override;
        void onFixedUpdate(double dt, double fixedDt) override;

    private:
        SmokeOptions m_options;
        std::uint32_t m_frameIndex = 0;
        bool m_captured = false;
        int m_exitCode = 0;

        void buildScene() noexcept;
        void buildBatch(const std::vector<SGCore::ECS::entity_t>& entities) noexcept;
        void buildInstancing(const std::vector<SGCore::ECS::entity_t>& entities) noexcept;
        void captureAndFinish() noexcept;
    };
}
