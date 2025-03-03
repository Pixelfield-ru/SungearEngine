//
// Created by stuka on 28.01.2025.
//

#ifndef SUNGEARENGINE_UIRENDERPASS_H
#define SUNGEARENGINE_UIRENDERPASS_H

#include "SGCore/Render/IRenderPass.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/UI/UIComponent.h"

namespace SGCore
{
    struct UIRenderPass : public IRenderPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) noexcept final;

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) noexcept final;

    private:
        /**
         *
         * @param uiComponent
         * @param currentUIElement
         * @return New child transform node index. Returns -1 if new node was created. Else returns bigger then zero number.
         */
        void processUIElement(const LayeredFrameReceiver::reg_t& cameraReceiver,
                              UI::UIComponent::reg_t& uiComponent,
                              const Ref<UI::UIElement>& currentUIElement,
                              const size_t& currentTransformNodeIdx) noexcept;
    };
}

#endif //SUNGEARENGINE_UIRENDERPASS_H
