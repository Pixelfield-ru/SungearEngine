//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_METAINFO_H
#define SUNGEARENGINE_METAINFO_H

#ifdef _MSC_VER
#define MI_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define MI_NOINLINE __attribute__((noinline))
#endif

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>

namespace SGCore
{
    struct MetaInfo
    {
        struct Meta
        {
            Meta() = default;
            Meta(const Meta&) noexcept = default;
            Meta(Meta&&) noexcept = default;
            /*explicit Annotation(const std::string& value)
            {
                m_value = value;
            }*/

            Meta& operator[](const std::string& childName) noexcept
            {
                auto it = std::find_if(m_children.begin(), m_children.end(), [&childName](auto child) {
                    return childName == child->first;
                });

                if(it == m_children.end())
                {
                    m_children.emplace_back(std::make_shared<std::pair<std::string, Meta>>(childName, Meta()));
                    (*m_children.rbegin())->second.m_name = childName;
                    return (*m_children.rbegin())->second;
                }
                else
                {
                    return (*it)->second;
                }
            }

            [[nodiscard]] bool hasChild(const std::string& childName) const noexcept
            {
                auto it = std::find_if(m_children.begin(), m_children.end(), [&childName](const auto& child) {
                    return childName == child->first;
                });

                return it != m_children.end();
            }

            Meta& operator=(const std::string& value) noexcept
            {
                m_value = value;
                return *this;
            }

            Meta& operator=(const Meta&) noexcept = default;
            Meta& operator=(Meta&&) noexcept = default;

            [[nodiscard]] std::string getValue() const noexcept
            {
                return m_value;
            }

            auto& getChildren() noexcept
            {
                return m_children;
            }

            [[nodiscard]] std::string getName() const noexcept
            {
                return m_name;
            }

        private:
            std::string m_name;
            std::string m_value;
            std::vector<std::shared_ptr<std::pair<std::string, Meta>>> m_children;
        };

        MI_NOINLINE static auto& getMeta() noexcept
        {
            return m_meta;
        }

        static void addStandardMetaInfo() noexcept
        {
            const char* sgSourcesPath = std::getenv("SUNGEAR_SOURCES_ROOT");
            if(!sgSourcesPath)
            {
                return;
            }

            const std::string sgSourcesPathStr = sgSourcesPath;

            // SGCore::AudioSource
            {
                Meta meta;
                meta["fullName"] = "SGCore::AudioSource";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Audio/AudioSource.h";
                meta["type"] = "component";

                meta["members"]["m_rolloffFactor"]["setter"] = "setRolloffFactor";
                meta["members"]["m_rolloffFactor"]["getter"] = "getRolloffFactor";

                meta["members"]["m_pitch"]["setter"] = "setPitch";
                meta["members"]["m_pitch"]["getter"] = "getPitch";

                meta["members"]["m_gain"]["setter"] = "setGain";
                meta["members"]["m_gain"]["getter"] = "getGain";

                meta["members"]["m_direction"]["setter"] = "setDirection";
                meta["members"]["m_direction"]["getter"] = "getDirection";

                meta["members"]["m_velocity"]["setter"] = "setVelocity";
                meta["members"]["m_velocity"]["getter"] = "getVelocity";

                meta["members"]["m_position"]["setter"] = "setPosition";
                meta["members"]["m_position"]["getter"] = "getPosition";

                meta["members"]["m_isLooping"]["setter"] = "setIsLooping";
                meta["members"]["m_isLooping"]["getter"] = "isLooping";

                // TODO: add
                // meta["members"]["m_attachedAudioTrack"];

                meta["members"]["m_lastState"]["setter"] = "setState";
                meta["members"]["m_lastState"]["getter"] = "getState";

                meta["members"]["m_type"]["setter"] = "setType";
                meta["members"]["m_type"]["getter"] = "getType";

                m_meta["structs"].push_back(meta);
            }

            // SGCore::UniqueName
            {
                Meta meta;
                meta["fullName"] = "SGCore::UniqueName";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Utils/UniqueName.h";

                meta["members"]["m_rawName"];
                meta["members"]["m_uniqueID"];
                meta["members"]["m_name"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::UniqueNameWrapper
            {
                Meta meta;
                meta["fullName"] = "SGCore::UniqueNameWrapper";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Utils/UniqueName.h";

                meta["members"]["m_uniqueName"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::AABB
            {
                Meta meta;
                meta["fullName"] = "SGCore::AABB";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Math/AABB.h";
                meta["type"] = "component";
                meta["getFromRegistryBy"] = "SGCore::AABB<float>";
                meta["template_args"]["ScalarT"] = "typename";

                meta["members"]["m_min"];
                meta["members"]["m_max"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::Layer
            {
                Meta meta;
                meta["fullName"] = "SGCore::Layer";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Scene/Layer.h";

                meta["members"]["m_name"];
                meta["members"]["m_isOpaque"];
                meta["members"]["m_index"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::TransformBase
            {
                Meta meta;
                meta["fullName"] = "SGCore::TransformBase";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Transformations/TransformBase.h";

                meta["members"]["m_blockTranslation"];
                meta["members"]["m_blockRotation"];
                meta["members"]["m_blockScale"];
                meta["members"]["m_aabb"];
                meta["members"]["m_position"];
                meta["members"]["m_rotation"];
                meta["members"]["m_yawPitchRoll"];
                meta["members"]["m_scale"];
                meta["members"]["m_right"];
                meta["members"]["m_forward"];
                meta["members"]["m_up"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::Transform
            {
                Meta meta;
                meta["fullName"] = "SGCore::Transform";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Transformations/Transform.h";
                meta["type"] = "component";
                meta["getFromRegistryBy"] = "SGCore::Ref<SGCore::Transform>";

                meta["members"]["m_finalTransform"];
                meta["members"]["m_ownTransform"];
                meta["members"]["m_followParentTRS"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::EntityBaseInfo
            {
                Meta meta;
                meta["fullName"] = "SGCore::EntityBaseInfo";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Scene/EntityBaseInfo.h";
                meta["type"] = "component";

                meta["baseTypes"]["SGCore::UniqueNameWrapper"];

                meta["members"]["m_parent"]["annotations"]["doNotSerialize"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::MeshDataRenderInfo
            {
                Meta meta;
                meta["fullName"] = "SGCore::MeshDataRenderInfo";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/ImportedScenesArch/MeshDataRenderInfo.h";

                meta["members"]["m_useIndices"];
                meta["members"]["m_enableFacesCulling"];
                meta["members"]["m_facesCullingFaceType"];
                meta["members"]["m_facesCullingPolygonsOrder"];
                meta["members"]["m_drawMode"];
                meta["members"]["m_linesWidth"];
                meta["members"]["m_pointsSize"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::RenderingBase
            {
                Meta meta;
                meta["fullName"] = "SGCore::RenderingBase";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/RenderingBase.h";
                meta["type"] = "component";

                meta["members"]["m_fov"];
                meta["members"]["m_aspect"];
                meta["members"]["m_zNear"];
                meta["members"]["m_zFar"];
                meta["members"]["m_left"];
                meta["members"]["m_right"];
                meta["members"]["m_bottom"];
                meta["members"]["m_top"];
                meta["members"]["m_projectionMatrix"];
                meta["members"]["m_orthographicMatrix"];
                meta["members"]["m_viewMatrix"];
                meta["members"]["m_projectionSpaceMatrix"];
                meta["members"]["m_orthographicSpaceMatrix"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::Camera3D
            {
                Meta meta;
                meta["fullName"] = "SGCore::Camera3D";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Camera3D.h";
                meta["type"] = "component";

                m_meta["structs"].push_back(meta);
            }

            // SGCore::GizmoBase
            {
                Meta meta;
                meta["fullName"] = "SGCore::GizmoBase";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/GizmoBase.h";

                meta["members"]["m_color"];
                meta["members"]["m_meshBase"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::SphereGizmo
            {
                Meta meta;
                meta["fullName"] = "SGCore::SphereGizmo";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/SphereGizmo.h";
                meta["type"] = "component";

                meta["members"]["m_base"];
                meta["members"]["m_radius"];
                meta["members"]["m_angleIncrement"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::BoxGizmo
            {
                Meta meta;
                meta["fullName"] = "SGCore::BoxGizmo";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/BoxGizmo.h";
                meta["type"] = "component";

                meta["members"]["m_base"];
                meta["members"]["m_size"];
                meta["members"]["m_lastSize"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::LineGizmo
            {
                Meta meta;
                meta["fullName"] = "SGCore::LineGizmo";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/LineGizmo.h";
                meta["type"] = "component";

                meta["members"]["m_base"];
                meta["members"]["m_start"];
                meta["members"]["m_end"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::MeshBase
            {
                Meta meta;
                meta["fullName"] = "SGCore::MeshBase";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/MeshBase.h";

                meta["members"]["m_meshDataRenderInfo"];

                /*meta["members"]["m_meshData"]["setter"] = "setMeshData";
                meta["members"]["m_meshData"]["getter"] = "getMeshData";*/

                // TODO: add
                /*meta["members"]["m_material"]["setter"] = "setMaterial";
                meta["members"]["m_material"]["getter"] = "getMaterial";*/

                m_meta["structs"].push_back(meta);
            }

            // SGCore::LightBase
            {
                Meta meta;
                meta["fullName"] = "SGCore::LightBase";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Lighting/LightBase.h";

                meta["members"]["m_color"];
                meta["members"]["m_intensity"];
                meta["members"]["m_samplesCount"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::DirectionalLight
            {
                Meta meta;
                meta["fullName"] = "SGCore::DirectionalLight";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Lighting/DirectionalLight.h";
                meta["type"] = "component";

                meta["members"]["m_base"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::Atmosphere
            {
                Meta meta;
                meta["fullName"] = "SGCore::Atmosphere";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Atmosphere/Atmosphere.h";
                meta["type"] = "component";

                meta["members"]["m_sunPosition"];

                meta["members"]["m_sunAmbient"];
                meta["members"]["m_lastSunAmbient"];

                meta["members"]["m_sunRotation"];
                meta["members"]["m_lastSunRotation"];

                meta["members"]["m_rayleighScatteringCoeff"];
                meta["members"]["m_lastRayleighScatteringCoeff"];

                meta["members"]["m_mieScatteringCoeff"];
                meta["members"]["m_lastMieScatteringCoeff"];

                meta["members"]["m_rayleighScaleHeight"];
                meta["members"]["m_lastRayleighScaleHeight"];

                meta["members"]["m_mieScaleHeight"];
                meta["members"]["m_lastMieScaleHeight"];

                meta["members"]["m_sunIntensity"];
                meta["members"]["m_lastSunIntensity"];

                meta["members"]["m_planetRadius"];
                meta["members"]["m_lastPlanetRadius"];

                meta["members"]["m_atmosphereRadius"];
                meta["members"]["m_lastAtmosphereRadius"];

                meta["members"]["m_miePreferredScatteringDirection"];
                meta["members"]["m_lastMiePreferredScatteringDirection"];

                meta["members"]["m_rayOrigin"];
                meta["members"]["m_lastRayOrigin"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::Controllable3D
            {
                Meta meta;
                meta["fullName"] = "SGCore::Controllable3D";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Transformations/Controllable3D.h";
                meta["type"] = "component";

                meta["members"]["m_movementSpeed"];
                meta["members"]["m_rotationSensitive"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::UICamera
            {
                Meta meta;
                meta["fullName"] = "SGCore::UICamera";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/UICamera.h";
                meta["type"] = "component";

                m_meta["structs"].push_back(meta);
            }

            // SGCore::ISystem
            {
                Meta meta;
                meta["fullName"] = "SGCore::ISystem";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Scene/ISystem.h";
                meta["type"] = "system";

                m_meta["structs"].push_back(meta);
            }

            // SGCore::IParallelSystem
            {
                Meta meta;
                meta["fullName"] = "SGCore::IParallelSystem";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Scene/IParallelSystem.h";
                meta["type"] = "system";

                meta["template_args"]["ParallelSystemT"] = "typename";

                // meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::PhysicsWorld3D
            {
                Meta meta;
                meta["fullName"] = "SGCore::PhysicsWorld3D";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Physics/PhysicsWorld3D.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::IParallelSystem<SGCore::PhysicsWorld3D>"];
                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::RenderingBasesUpdater
            {
                Meta meta;
                meta["fullName"] = "SGCore::RenderingBasesUpdater";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/RenderingBasesUpdater.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::AtmosphereUpdater
            {
                Meta meta;
                meta["fullName"] = "SGCore::AtmosphereUpdater";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Atmosphere/AtmosphereUpdater.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::DirectionalLightsUpdater
            {
                Meta meta;
                meta["fullName"] = "SGCore::DirectionalLightsUpdater";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Lighting/DirectionalLightsUpdater.h";
                meta["type"] = "system";

                meta["members"]["m_maxLightsCount"];

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::TransformationsUpdater
            {
                Meta meta;
                meta["fullName"] = "SGCore::TransformationsUpdater";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Transformations/TransformationsUpdater.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::IParallelSystem<SGCore::TransformationsUpdater>"];
                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::Controllables3DUpdater
            {
                Meta meta;
                meta["fullName"] = "SGCore::Controllables3DUpdater";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Transformations/Controllables3DUpdater.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::BoxGizmosRenderer
            {
                Meta meta;
                meta["fullName"] = "SGCore::BoxGizmosRenderer";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/BoxGizmosRenderer.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::LineGizmosRenderer
            {
                Meta meta;
                meta["fullName"] = "SGCore::LineGizmosRenderer";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/LineGizmosRenderer.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::SphereGizmosUpdater
            {
                Meta meta;
                meta["fullName"] = "SGCore::SphereGizmosUpdater";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Gizmos/SphereGizmosUpdater.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::DebugDraw
            {
                Meta meta;
                meta["fullName"] = "SGCore::DebugDraw";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/DebugDraw.h";
                meta["type"] = "system";

                meta["members"]["m_mode"];
                meta["members"]["m_linesRenderInfo"];
                // meta["members"]["m_maxLines"];

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::BatchesRenderer
            {
                Meta meta;
                meta["fullName"] = "SGCore::BatchesRenderer";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/Batching/BatchesRenderer.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::OctreesSolver
            {
                Meta meta;
                meta["fullName"] = "SGCore::OctreesSolver";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Render/SpacePartitioning/OctreesSolver.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::IParallelSystem<SGCore::OctreesSolver>"];
                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }

            // SGCore::AudioProcessor
            {
                Meta meta;
                meta["fullName"] = "SGCore::AudioProcessor";
                meta["filePath"] = sgSourcesPathStr + "/Sources/SGCore/Audio/AudioProcessor.h";
                meta["type"] = "system";

                meta["baseTypes"]["SGCore::ISystem"];

                m_meta["structs"].push_back(meta);
            }
        }

        static void clearAllMeta() noexcept
        {
            m_meta.clear();
        }

    private:
        static inline std::unordered_map<std::string, std::vector<Meta>> m_meta;
    };
}

#undef MI_NOINLINE

#endif //SUNGEARENGINE_METAINFO_H
