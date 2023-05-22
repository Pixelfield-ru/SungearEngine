#include "SGCore/Main/Core.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"

void init()
{
    /** УТЕЧЕК ПАМЯТИ ЗДЕСЬ НЕТ */
    // TODO: сделать мега абстрактный VertexBuffer. ну там немного осталось, самое сложно (VertexBufferLayout я сделал)

    // найс это работает. TODO: убрать! просто ради теста ---------------------
    std::shared_ptr<Core::Memory::Assets::FileAsset> s = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");

    /** ABSTRACT SHADER TEST */
    std::shared_ptr<Core::Graphics::API::IShader> testShader(Core::Main::Core::getRenderer().createShader());
    testShader->compile("test_shader", s->getData());

    /** ABSTRACT INDEX BUFFER TEST */
    std::shared_ptr<Core::Graphics::API::IIndexBuffer> testIndexBuffer(Core::Main::Core::getRenderer().createIndexBuffer());
    testIndexBuffer->create()->bind()->putData({ 0, 1, 2 });

    /** VERTEX BUFFER LAYOUT TEST */
    std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testVertexBufferLayout(Core::Main::Core::getRenderer().createVertexBufferLayout());
    testVertexBufferLayout
    ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVertexBufferLayout->createVertexAttribute(0, "positionAttribute", SGGDataType::SGG_DATA_TYPE_FLOAT3)))
    ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVertexBufferLayout->createVertexAttribute(1, "textureCoordsAttribute", SGGDataType::SGG_DATA_TYPE_FLOAT3)))
    ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVertexBufferLayout->createVertexAttribute(2, "normalPositionAttribute", SGGDataType::SGG_DATA_TYPE_FLOAT3)));
    testVertexBufferLayout->enableAttributes();
    // ------------------------------------------------------------------------
}

void framePostRender()
{

}

int main()
{
    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);

    Core::Main::Core::start();

    return 0;
}