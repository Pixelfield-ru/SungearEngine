//
// Created by ilya on 23.06.24.
//

#ifndef SUNGEARENGINEEDITOR_INSPECTORVIEW_H
#define SUNGEARENGINEEDITOR_INSPECTORVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>

#include "Window.h"

namespace SGE
{
    enum class InspectorViewType
    {
        INSPECT_ENTITY,
        INSPECT_MATERIAL
    };

    struct InspectorView : public SGCore::ImGuiWrap::IView
    {
        virtual ~InspectorView() = default;
        InspectorView() noexcept;

        InspectorViewType m_type = InspectorViewType::INSPECT_ENTITY;

        SGCore::ECS::entity_t m_currentChosenEntity { };
        SGCore::AssetRef<SGCore::IMaterial>  m_currentMaterial;

        bool begin() final;
        void renderBody() final;
        void end() final;

    private:
        SGCore::Ref<Window> m_assetChooseWindow;

        void inspectEntity() const noexcept;
        void inspectMaterial() const noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_INSPECTORVIEW_H
