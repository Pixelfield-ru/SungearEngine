//#define SUNGEAR_DEBUG

#include "CoreTest.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Graphics/API/IShaderUniform.h"
#include "SGCore/Memory/Assets/Material.h"
#include "SGCore/Memory/Assets/ModelAsset.h"

#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SGCore/Main/Callbacks.h"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"

std::shared_ptr<Core::Memory::Assets::Texture2DAsset> texture2DAsset;

std::shared_ptr<Core::Graphics::API::IShader> testShader;

std::shared_ptr<Core::Memory::Assets::ModelAsset> testModel;

std::shared_ptr<Core::Graphics::API::IVertexArray> testVertexArray;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testVerticesPositionsBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testVerticesBufferLayout;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testUVBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testUVBufferLayout;

std::shared_ptr<Core::Graphics::API::IVertexBuffer> testNormalsBuffer;
std::shared_ptr<Core::Graphics::API::IVertexBufferLayout> testNormalsBufferLayout;

std::shared_ptr<Core::Graphics::API::IIndexBuffer> testIndexBuffer;

std::shared_ptr<Core::Graphics::API::IUniformBuffer> testUniformBuffer;

std::shared_ptr<Core::Memory::Assets::Material> testMaterial;

glm::mat4 cameraProjectionMatrix;
glm::mat4 cameraViewMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);


void init()
{
    // найс это работает. TODO: убрать! просто ради теста ---------------------
    std::shared_ptr<Core::Memory::Assets::FileAsset> shaderAsset = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");

    testVertexArray = std::shared_ptr<Core::Graphics::API::IVertexArray>(Core::Main::Core::getRenderer().createVertexArray());
    testVertexArray->create()->bind();

    // vertices pos --------------------------------------
    testVerticesPositionsBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    testVerticesPositionsBuffer
            ->setUsage(SGGUsage::SGG_DYNAMIC)
            ->create()
            ->bind()
            ->putData({ // позиция
                              -0.5, -0.5, 0.0,
                              // uv
                              1, 1, 0,
                              // нормаль
                              0.5, 0.1, 0,

                              // и т.д.

                              -0.5, 0.5, 0.0,
                              1, 0, 0,
                              0.1, 0.4, 0,

                              0.5, 0.5, 0.0,
                              0, 0, 0,
                              0.1, 0, 0.5,

                              0.5, -0.5, 0.0,
                              0, 1, 0,
                              1, 1, 1
                      });

    testVerticesBufferLayout = std::shared_ptr<Core::Graphics::API::IVertexBufferLayout>(Core::Main::Core::getRenderer().createVertexBufferLayout());
    testVerticesBufferLayout
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesBufferLayout->createVertexAttribute(0, "positionsAttribute", SGGDataType::SGG_FLOAT3)))
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesBufferLayout->createVertexAttribute(1, "UVAttribute", SGGDataType::SGG_FLOAT3)))
            ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(testVerticesBufferLayout->createVertexAttribute(2, "normalsAttribute", SGGDataType::SGG_FLOAT3)))
            ->prepare()
            ->enableAttributes();

    testIndexBuffer = std::shared_ptr<Core::Graphics::API::IIndexBuffer>(Core::Main::Core::getRenderer().createIndexBuffer());
    testIndexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData({0, 1, 2, 3, 2, 0 });

    int windowWidth;
    int windowHeight;

    Core::Main::Core::getWindow().getSize(windowWidth, windowHeight);

    // matrices init
    cameraProjectionMatrix = glm::perspective<float>(75.0f, (float) windowWidth / (float) windowHeight, 1, 200);

    cameraViewMatrix = glm::scale(cameraViewMatrix, glm::vec3(1, 1, 1));
    //glm::rotate(cameraViewMatrix, glm::radians(0.0f), glm::vec3(0, 0, 0));
    cameraViewMatrix = glm::translate(cameraViewMatrix, glm::vec3(0, 0, 0));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -10));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));

    // UNIFORM BUFFERS ARE WORKING NOW. SUBDATA WORKING TOO!!
    testUniformBuffer = std::shared_ptr<Core::Graphics::API::IUniformBuffer>(Core::Main::Core::getRenderer().createUniformBuffer());
    testUniformBuffer->putUniforms({
        Core::Graphics::API::IShaderUniform("cameraProjectionMatrix", SGGDataType::SGG_MAT4),
        Core::Graphics::API::IShaderUniform("cameraViewMatrix", SGGDataType::SGG_MAT4),
        Core::Graphics::API::IShaderUniform("objectModelMatrix", SGGDataType::SGG_MAT4),
        Core::Graphics::API::IShaderUniform("testColor", SGGDataType::SGG_FLOAT4)
    });
    testUniformBuffer->putData<float>({ });
    testUniformBuffer->putData<float>({ });
    testUniformBuffer->putData<float>({ });

    testUniformBuffer->putData<float>({ 1, 0, 0, 1 });

    testUniformBuffer->prepare();

    testUniformBuffer->subData<float>("testColor", { 0, 1, 0, 1 });
    testUniformBuffer->subData("cameraProjectionMatrix", glm::value_ptr(cameraProjectionMatrix), 16);

    testMaterial = std::make_shared<Core::Memory::Assets::Material>();
    testMaterial->createAsPBR();

    testMaterial->findAndSetTexture2D(SGMAT_BASE_COLOR, "../SGResources/textures/horek.png");
    testMaterial->findAndSetTexture2D(SGMAT_SPECULAR_COLOR, "../SGResources/textures/x.png");
    testMaterial->findAndSetTexture2D(SGMAT_ROUGHNESS, "../SGResources/textures/hedgehog.png");

    //testUniformBuffer->updateLocations(*testShader);
    // ----------------------------------------------------

    testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/plane.obj");
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------
float rotationCoeff = 0.075f;

float cameraMovementSpeed = 0.05f;

glm::vec3 cameraRotation;

glm::vec3 forward(0.0f, 0.0f, 1.0f);
glm::vec3 left(-1.0f, 0.0f, 0.0f);
glm::vec3 cameraPosition;

void framePostRender()
{
    cameraRotation.x += (float) -InputManager::getMainInputListener()->getCursorPositionDeltaY() * rotationCoeff;
    cameraRotation.y += (float) -InputManager::getMainInputListener()->getCursorPositionDeltaX() * rotationCoeff;

    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_R))
    {
        cameraRotation.x = cameraRotation.y = cameraRotation.z = cameraPosition.x = cameraPosition.y = cameraPosition.z = 0.0f;
    }

    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W))
    {
        glm::vec3 rotatedForward = forward;
        rotatedForward = glm::rotate(rotatedForward, glm::radians(-cameraRotation.x), glm::vec3(1, 0, 0));
        rotatedForward = glm::rotate(rotatedForward, glm::radians(-cameraRotation.y), glm::vec3(0, 1, 0));

        cameraPosition += rotatedForward * cameraMovementSpeed;
    }
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
    {
        glm::vec3 rotatedForward = forward;
        rotatedForward = glm::rotate(rotatedForward, glm::radians(-cameraRotation.x), glm::vec3(1, 0, 0));
        rotatedForward = glm::rotate(rotatedForward, glm::radians(-cameraRotation.y), glm::vec3(0, 1, 0));

        cameraPosition -= rotatedForward * cameraMovementSpeed;
    }
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A))
    {
        glm::vec3 rotatedLeft = left;
        rotatedLeft = glm::rotate(rotatedLeft, glm::radians(-cameraRotation.x), glm::vec3(1, 0, 0));
        rotatedLeft = glm::rotate(rotatedLeft, glm::radians(-cameraRotation.y), glm::vec3(0, 1, 0));

        cameraPosition += rotatedLeft * cameraMovementSpeed;
    }
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
    {
        glm::vec3 rotatedLeft = left;
        rotatedLeft = glm::rotate(rotatedLeft, glm::radians(-cameraRotation.x), glm::vec3(1, 0, 0));
        rotatedLeft = glm::rotate(rotatedLeft, glm::radians(-cameraRotation.y), glm::vec3(0, 1, 0));

        cameraPosition -= rotatedLeft * cameraMovementSpeed;
    }

    if(InputManager::getMainInputListener()->keyboardKeyReleased(KEY_ESCAPE))
    {
        Core::Main::Core::getWindow().setHideAndCentralizeCursor(!Core::Main::Core::getWindow().isHideAndCentralizeCursor());
    }

    glm::quat rotationQuat = glm::angleAxis(glm::radians(cameraRotation.x), glm::vec3(1, 0, 0));
    rotationQuat *= glm::angleAxis(glm::radians(cameraRotation.y), glm::vec3(0, 1, 0));

    cameraViewMatrix = glm::toMat4(rotationQuat);
    cameraViewMatrix = glm::translate(cameraViewMatrix, cameraPosition);

    testUniformBuffer->subData("cameraViewMatrix", glm::value_ptr(cameraViewMatrix), 16);
    testUniformBuffer->subData("objectModelMatrix", glm::value_ptr(modelMatrix), 16);

    Core::Main::Core::getRenderer().renderMesh(testMaterial, testUniformBuffer, testModel);
}

// --------------------------------------------

int main()
{
    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);

    Core::Main::Core::start();

    return 0;
}