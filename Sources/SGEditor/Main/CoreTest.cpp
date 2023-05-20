#include "SGCore/Main/Core.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/Shader.h"
#include "SGCore/Graphics/API/GL46/GLShader.h"

void init()
{
    // найс это работает. TODO: убрать! просто ради теста ---------------------
    std::shared_ptr<Core::Memory::Assets::FileAsset> s = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");

    //std::cout << s->getData() << std::endl;

    std::shared_ptr<Core::Graphics::API::Shader> gl46Shader = std::make_shared<Core::Graphics::API::GL46::GLShader>();

    gl46Shader->compile("test_gl46Shader", s->getData());
    // ------------------------------------------------------------------------
}

int main()
{
    sgSetCoreInitCallback(init);

    Core::Main::Core::start();

    return 0;
}