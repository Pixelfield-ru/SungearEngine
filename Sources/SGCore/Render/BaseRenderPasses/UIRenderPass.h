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
    // yeah this struct violates SRP but...
    // if i will put transformations calculations in other system it will hit performance.
    // so it seems that there is no special need to put the calculation of transformations in a separate system,
    // since the transformation of a UI component cannot exist without the component itself.
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
        std::int64_t processUIElement(const LayeredFrameReceiver::reg_t& cameraReceiver,
                                      const std::int64_t& parentUITreeNodeIdx,
                                      UI::UIComponent::reg_t& uiComponent,
                                      const Ref<UI::UIElement>& currentUIElement) noexcept;

        std::int64_t m_currentProceedUIElements = 0;
    };
}

#endif //SUNGEARENGINE_UIRENDERPASS_H
