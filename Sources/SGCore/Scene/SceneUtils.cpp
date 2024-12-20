//
// Created by stuka on 20.12.2024.
//
#include "SceneUtils.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Scene/EntityBaseInfo.h"

SGCore::entity_t SGCore::SceneUtils::pickEntity(const glm::vec2& cursorPos,
                                                const registry_t& fromRegistry,
                                                const IFrameBuffer* fromFrameBuffer,
                                                SGFrameBufferAttachmentType fromAttachment) noexcept
{
    const auto pickedColor = fromFrameBuffer->readPixelsFromAttachment(cursorPos, fromAttachment);

    std::cout << std::format("picked color: {}, {}, {}", pickedColor.x, pickedColor.y, pickedColor.z) << std::endl;

    entity_t pickedEntity = entt::null;

    auto entitiesBaseInfoView = fromRegistry.view<EntityBaseInfo>();
    entitiesBaseInfoView.each([&pickedColor, &pickedEntity](const entity_t& entity,
                                                            const EntityBaseInfo& entityBaseInfo)  {
        if(pickedEntity == entt::null && entityBaseInfo.getUniqueColor() == pickedColor)
        {
            pickedEntity = entity;
        }
    });

    return pickedEntity;
}

