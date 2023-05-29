//#define SUNGEAR_DEBUG

#include "CoreTest.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "stb_image.h"

#include <glad/glad.h>

std::shared_ptr<Core::Memory::Assets::Texture2DAsset> texture2DAsset;

std::shared_ptr<Core::Graphics::API::IShader> testShader;

std::shared_ptr<Core::Graphics::API::IVertexArray> testVertexArray;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testVerticesPositionsBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testVerticesBufferLayout;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testUVBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testUVBufferLayout;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testNormalsBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testNormalsBufferLayout;

std::shared_ptr<Core::Graphics::API::IIndexBuffer> testIndexBuffer;

void init()
{
    // найс это работает. TODO: убрать! просто ради теста ---------------------
    std::shared_ptr<Core::Memory::Assets::FileAsset> shaderAsset = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");
    texture2DAsset = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>("../SGResources/textures/horek.png");

    testShader = std::shared_ptr<Core::Graphics::API::IShader>(Core::Main::Core::getRenderer().createShader());
    testShader->compile(shaderAsset.get());
    testShader->addShaderDefine(Core::Graphics::API::ShaderDefine("FLIP_TEXTURES_Y", "1"));

    testVertexArray = std::shared_ptr<Core::Graphics::API::IVertexArray>(Core::Main::Core::getRenderer().createVertexArray());
    testVertexArray->create()->bind();

    // vertices pos --------------------------------------
    testVerticesPositionsBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    testVerticesPositionsBuffer
            ->setUsage(SGGBufferUsage::SGG_DYNAMIC)
            ->create()
            ->bind()
            ->putData({ // три вершины
                // ПЕРВАЯ - ПОЗИЦИЯ ВЕРШИНЫ
                // ВТОРАЯ - ТЕКСТУРНАЯ КООРДИНАТА
                // ну и третье - нормали
                              0, 0.0, 0.0,
                              0.0, 0.0, 0,
                              0.5, 0.1, 0,

                              0.0, 1.0, 0.0,
                              0, 1, 0,
                              0.1, 0.4, 0,

                              1, 1.0, 0.0,
                              1, 1, 0,
                              0.1, 0, 0.5,

                              1, 0.0, 0.0,
                              1, 0, 0,
                              1, 1, 1
                      });

    testVerticesBufferLayout = std::shared_ptr<Core::Graphics::API::IVertexBufferLayout>(Core::Main::Core::getRenderer().createVertexBufferLayout());
    testVerticesBufferLayout
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesBufferLayout->createVertexAttribute(0, "positionAttribute", SGGDataType::SGG_FLOAT3)))
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesBufferLayout->createVertexAttribute(1, "textureCoordsAttribute", SGGDataType::SGG_FLOAT3)))
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesBufferLayout->createVertexAttribute(2, "normalPositionAttribute", SGGDataType::SGG_FLOAT3)))
            ->prepare()
            ->enableAttributes();
    // ----------------------------------------------------

    testIndexBuffer = std::shared_ptr<Core::Graphics::API::IIndexBuffer>(Core::Main::Core::getRenderer().createIndexBuffer());
    testIndexBuffer->setUsage(SGGBufferUsage::SGG_DYNAMIC)->create()->bind()->putData({ 0, 1, 2, 3, 2, 0 });
}

void framePostRender()
{
    Core::Main::Core::getRenderer().renderMesh(texture2DAsset->getTexture2D().get(), testShader.get(), testVertexArray.get());
}

int main()
{
    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);

    Core::Main::Core::start();

    return 0;
}