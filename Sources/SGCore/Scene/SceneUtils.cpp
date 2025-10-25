//
// Created by stuka on 20.12.2024.
//
#include "SceneUtils.h"

#include <iostream>
#include <fmt/format.h>

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/ECS/Registry.h"

SGCore::ECS::entity_t SGCore::SceneUtils::pickEntity(const glm::vec2& cursorPos,
                                                     const ECS::registry_t& fromRegistry,
                                                     const IFrameBuffer* fromFrameBuffer,
                                                     SGFrameBufferAttachmentType fromAttachment) noexcept
{
    const auto pickedColor = fromFrameBuffer->readPixelsFromAttachment(cursorPos, fromAttachment);

    std::cout << fmt::format("picked color: {}, {}, {}", pickedColor.x, pickedColor.y, pickedColor.z) << std::endl;

    ECS::entity_t pickedEntity = entt::null;

    auto entitiesBaseInfoView = fromRegistry.view<EntityBaseInfo>();
    entitiesBaseInfoView.each([&pickedColor, &pickedEntity](const ECS::entity_t& entity,
                                                            const EntityBaseInfo::reg_t& entityBaseInfo)  {
        if(pickedEntity == entt::null && entityBaseInfo.getUniqueColor() == pickedColor)
        {
            pickedEntity = entity;
        }
    });

    return pickedEntity;
}

