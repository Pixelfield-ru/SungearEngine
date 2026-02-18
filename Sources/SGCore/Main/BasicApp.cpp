//
// Created by stuka on 18.02.2026.
//

#include "BasicApp.h"

#include "CoreMain.h"
#include "SGCore/Audio/AudioListener.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"

#include "SGCore/Serde/Serde.h"
#include "SGCore/Serde/StandardSerdeSpecs/SerdeSpecs.h"
#include "SGCore/Utils/StringInterpolation/InterpolationResolver.h"

SGCore::BasicApp::~BasicApp() noexcept
{
    CoreMain::onInit -= m_initSlot;
    CoreMain::getRenderTimer().onUpdate -= m_updateSlot;
    CoreMain::getFixedTimer().onUpdate -= m_fixedUpdateSlot;
}

void SGCore::BasicApp::start(bool createBasicScene) noexcept
{
    m_isBasicSceneMustBeCreated = createBasicScene;

    CoreMain::onInit += m_initSlot;
    CoreMain::getRenderTimer().onUpdate += m_updateSlot;
    CoreMain::getFixedTimer().onUpdate += m_fixedUpdateSlot;

    CoreMain::init();

    CoreMain::startCycle();

    onDestroy();
}

SGCore::ECS::entity_t SGCore::BasicApp::getCameraEntity() const noexcept
{
    return m_cameraEntity;
}

void SGCore::BasicApp::initImpl() noexcept
{
    m_config = CoreMain::loadConfig(m_configPath.resolved());
    PathInterpolationMarkupSpec::setKey("enginePath", m_config.m_enginePath.resolved());
    CoreMain::s_sungearEngineRootPath = m_config.m_enginePath.resolved();

    auto& renderPipelinesManager = RenderPipelinesManager::instance();

    // setting pipeline that will render our scene
    auto pbrrpPipeline = renderPipelinesManager.createRenderPipeline<PBRRenderPipeline>();
    renderPipelinesManager.registerRenderPipeline(pbrrpPipeline);
    renderPipelinesManager.setCurrentRenderPipeline<PBRRenderPipeline>();

    if(m_isBasicSceneMustBeCreated)
    {
        // creating scene
        auto scene = SGCore::MakeRef<Scene>();
        scene->createDefaultSystems();

        Scene::setCurrentScene(scene);

        auto ecsRegistry = scene->getECSRegistry();

        // =============================================

        auto assetManager = AssetManager::getInstance();

        auto cubeModel = assetManager->loadAssetWithAlias<ModelAsset>(
            "cube_model",
            "${enginePath}/Resources/models/standard/cube.obj"
        );

        // ============================== creating sky
        if(cubeModel)
        {
            auto standardCubemap = assetManager->getOrAddAssetByAlias<ICubemapTexture>("standard_skybox0");

            standardCubemap->m_parts.push_back(assetManager->loadAsset<ITexture2D>(
                    "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
            ));
            standardCubemap->m_parts.push_back(assetManager->loadAsset<ITexture2D>(
                    "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
            ));

            standardCubemap->m_parts.push_back(assetManager->loadAsset<ITexture2D>(
                    "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
            ));
            standardCubemap->m_parts.push_back(assetManager->loadAsset<ITexture2D>(
                    "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
            ));

            standardCubemap->m_parts.push_back(assetManager->loadAsset<ITexture2D>(
                    "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
            ));
            standardCubemap->m_parts.push_back(assetManager->loadAsset<ITexture2D>(
                    "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
            ));

            standardCubemap->create();

            auto standardSkyMaterial = assetManager->getOrAddAssetByAlias<IMaterial>("standard_sky_material0");
            standardSkyMaterial->m_shaders["GeometryPass"] =
                    assetManager->loadAsset<IShader>(
                            *renderPipelinesManager.getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
            standardSkyMaterial->m_meshRenderState.m_useFacesCulling = false;
            standardSkyMaterial->addTexture2D(SGTextureSlot::SGTT_SKYBOX, standardCubemap);

            // ================================================

            const auto skyboxEntities = cubeModel->m_rootNode->addOnScene(scene);

            const auto atmosphereEntity = skyboxEntities[2];

            auto& skyboxMesh = ecsRegistry->get<Mesh>(atmosphereEntity);
            auto& atmosphereScattering = ecsRegistry->emplace<Atmosphere>(atmosphereEntity);
            skyboxMesh.m_base.setMaterial(standardSkyMaterial);

            auto& skyboxTransform = ecsRegistry->get<Transform>(atmosphereEntity);

            skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };
        }

        // ================================================== creating camera

        // creating entity
        m_cameraEntity = ecsRegistry->create();

        // creating components for entity
        auto cameraTransform = ecsRegistry->emplace<Transform>(m_cameraEntity, SGCore::MakeRef<Transform>());
        ecsRegistry->emplace<NonSavable>(m_cameraEntity);
        ecsRegistry->emplace<Camera3D>(m_cameraEntity, SGCore::MakeRef<Camera3D>());
        ecsRegistry->emplace<RenderingBase>(m_cameraEntity, SGCore::MakeRef<RenderingBase>());
        ecsRegistry->emplace<Controllable3D>(m_cameraEntity);
        auto& cameraReceiver = ecsRegistry->emplace<LayeredFrameReceiver>(m_cameraEntity);
    }

    onInit();
}

void SGCore::BasicApp::updateImpl(double dt, double fixedDt) noexcept
{
    const auto currentScene = Scene::getCurrentScene();

    if(currentScene)
    {
        auto cameraTransform = currentScene->getECSRegistry()->tryGet<Transform>(m_cameraEntity);

        if(cameraTransform)
        {
            AudioListener::setPosition((*cameraTransform)->m_finalTransform.m_position);
            AudioListener::setOrientation((*cameraTransform)->m_finalTransform.m_forward, (*cameraTransform)->m_finalTransform.m_up);
        }

        currentScene->update(dt, fixedDt);
    }

    onUpdate(dt, fixedDt);

    if(!currentScene) return;

    auto frameReceiver = currentScene->getECSRegistry()->tryGet<LayeredFrameReceiver>(m_cameraEntity);
    if(frameReceiver)
    {
        auto attachment = frameReceiver->m_layersFXFrameBuffer->getAttachment(m_attachmentToDisplay);
        CoreMain::getRenderer()->renderTextureOnScreen(attachment.get(), m_flipDisplay);
    }
}

void SGCore::BasicApp::fixedUpdateImpl(double dt, double fixedDt) noexcept
{
    if(const auto currentScene = Scene::getCurrentScene())
    {
        currentScene->fixedUpdate(dt, fixedDt);
    }

    onFixedUpdate(dt, fixedDt);
}
