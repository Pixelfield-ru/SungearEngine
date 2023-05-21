#include "SGCore/Main/Core.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/Shader.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Shader.h"

void init()
{
    // найс это работает. TODO: убрать! просто ради теста ---------------------
    std::shared_ptr<Core::Memory::Assets::FileAsset> s = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");

    //std::cout << s->getData() << std::endl;

    std::shared_ptr<Core::Graphics::API::Shader> testShader(Core::Main::Core::getRenderer().createShader());
    testShader->compile("test_shader", s->getData());

    std::shared_ptr<Core::Graphics::API::IIndexBuffer> testIndexBuffer(Core::Main::Core::getRenderer().createIndexBuffer());
    testIndexBuffer->create()->bind()->putData({ 0, 1, 2 });
    // ------------------------------------------------------------------------
}

int main()
{
    sgSetCoreInitCallback(init);

    Core::Main::Core::start();

    return 0;
}