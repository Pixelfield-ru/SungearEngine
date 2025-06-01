//
// Created by ilya on 23.06.24.
//

#ifndef SUNGEARENGINE_SGCORE_PCH_H
#define SUNGEARENGINE_SGCORE_PCH_H

// STD
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <map>
#include <variant>
#include <initializer_list>
#include <cstddef>
#include <iostream>
#include <utility>
#include <cstdint>
#include <set>
#include <unordered_map>
#include <forward_list>
#include <cstring>
#include <atomic>
#include <algorithm>
#include <thread>
#include <mutex>
#include <csetjmp>
#include <unordered_set>
#include <functional>
#include <filesystem>
#include <stack>
#include <array>
#include <source_location>
#include <chrono>
#include <regex>
#include <any>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <tuple>
#include <codecvt>
#include <cwctype>
#include <locale>
#include <random>
#include <type_traits>

// ENTT
#include <entt/entt.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/observer.hpp>

// RAPIDJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>

// GLAD
#include <glad/glad.h>

// SPDLOG
#include <spdlog/spdlog.h>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/ext.hpp>

// IMGUI
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

// BULLET
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <LinearMath/btIDebugDraw.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <LinearMath/btIDebugDraw.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

// ASSIMP
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/types.h>

// GLFW 3
#include <GLFW/glfw3.h>

// FREETYPE
#include <freetype/freetype.h>

// LUNASVG
#include <lunasvg.h>

// PUGIXML
#include <pugixml.hpp>

// STB
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#endif // SUNGEARENGINE_SGCORE_PCH_H
