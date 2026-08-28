//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

namespace SGSmoke
{
    /// Triggers a RenderDoc capture of exactly the frame the smoke test captures, so the .rdc always
    /// contains the frame the reference image is compared against — no "press F12 in time" race.
    ///
    /// Does nothing unless the process was launched under RenderDoc (renderdoccmd capture / the GUI),
    /// because it only talks to an already-loaded renderdoc.dll through its in-app API.
    struct RenderDocCapture
    {
        /// True when renderdoc.dll is present in this process and its API answered.
        [[nodiscard]] static bool isAvailable() noexcept;

        static void beginFrame() noexcept;
        static void endFrame() noexcept;
    };
}
