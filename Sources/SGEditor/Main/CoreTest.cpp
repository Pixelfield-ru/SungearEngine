#define SUNGEAR_DEBUG

#include "CoreTest.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"

#include <glad/glad.h>

std::shared_ptr<Core::Graphics::API::IShader> testShader;

std::shared_ptr<Core::Graphics::API::IVertexArray> testVertexArray;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testVerticesPositionsBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testVerticesPositionsBufferLayout;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testUVBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testUVBufferLayout;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testNormalsBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testNormalsBufferLayout;

std::shared_ptr<Core::Graphics::API::IIndexBuffer> testIndexBuffer;

void init()
{
    // найс это работает. TODO: убрать! просто ради теста ---------------------
    std::shared_ptr<Core::Memory::Assets::FileAsset> s = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");

    testShader = std::shared_ptr<Core::Graphics::API::IShader>(Core::Main::Core::getRenderer().createShader());
    testShader->compile("test_shader", s->getData());

    testVertexArray = std::shared_ptr<Core::Graphics::API::IVertexArray>(Core::Main::Core::getRenderer().createVertexArray());
    testVertexArray->create()->bind();

    // vertices pos --------------------------------------
    testVerticesPositionsBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    testVerticesPositionsBuffer
            ->setUsage(SGGBufferUsage::SGG_DYNAMIC)
            ->create()
            ->bind()
            ->putData({ // три вершины
                              -1, 0.0, 0.0,
                              0.0, 1.0, 0.0,
                              1, 0.0, 0.0
                      });

    testVerticesPositionsBufferLayout = std::shared_ptr<Core::Graphics::API::IVertexBufferLayout>(Core::Main::Core::getRenderer().createVertexBufferLayout());
    testVerticesPositionsBufferLayout
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesPositionsBufferLayout->createVertexAttribute(0, "positionAttribute", SGGDataType::SGG_FLOAT3)))
            ->prepare()
            ->enableAttributes();
    // ----------------------------------------------------

    // vertices uv --------------------------------------
    testUVBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    testUVBuffer
            ->setUsage(SGGBufferUsage::SGG_DYNAMIC)
            ->create()
            ->bind()
            ->putData({
                              -0.5, 0.0,
                              0.0, 1.0,
                              0.5, 0.0
                      });

    testUVBufferLayout = std::shared_ptr<Core::Graphics::API::IVertexBufferLayout>(Core::Main::Core::getRenderer().createVertexBufferLayout());
    testUVBufferLayout
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testUVBufferLayout->createVertexAttribute(1, "textureCoordsAttribute", SGGDataType::SGG_FLOAT2)))
            ->prepare()
            ->enableAttributes();
    // --------------------------------------------------

    // vertices normals --------------------------------------
    testNormalsBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    testNormalsBuffer
            ->setUsage(SGGBufferUsage::SGG_DYNAMIC)
            ->create()
            ->bind()
            ->putData({
                              -0.5, 0.0, 0.0,
                              0.0, 1.0, 0.0,
                              0.5, 0.0, 0.0
                      });

    testNormalsBufferLayout = std::shared_ptr<Core::Graphics::API::IVertexBufferLayout>(Core::Main::Core::getRenderer().createVertexBufferLayout());
    testNormalsBufferLayout
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testNormalsBufferLayout->createVertexAttribute(2, "normalPositionAttribute", SGGDataType::SGG_FLOAT3)))
            ->prepare()
            ->enableAttributes();
    // ------------------------------------------------------

    testIndexBuffer = std::shared_ptr<Core::Graphics::API::IIndexBuffer>(Core::Main::Core::getRenderer().createIndexBuffer());
    testIndexBuffer->setUsage(SGGBufferUsage::SGG_DYNAMIC)->create()->bind()->putData({ 0, 1, 2 });
}

void framePostRender()
{
    Core::Main::Core::getRenderer().renderMesh(testShader.get(), testVertexArray.get());
}

int main()
{
    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);

    Core::Main::Core::start();

    return 0;
}