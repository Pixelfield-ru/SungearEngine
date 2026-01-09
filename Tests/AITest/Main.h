//
// Created by stuka on 13.12.2025.
//

#pragma once

#include <glm/vec3.hpp>

#include "SGCore/Coro/Task.h"
#include "SGCore/Main/CoreGlobals.h"

void regenerateNavMesh();

void coreInit();

SGCore::ECS::entity_t spawnVegetable(SGCore::ECS::registry_t& registry, const glm::vec3& position, const glm::vec3& scale = { 1, 1, 1 });
SGCore::Coro::Task<> startSpawnVegetables(SGCore::ECS::registry_t& registry, glm::vec3 areaPosition, float radius, std::pair<std::chrono::milliseconds, std::chrono::milliseconds> interval);

void onUpdate(const double& dt, const double& fixedDt);
void onFixedUpdate(const double& dt, const double& fixedDt);

int main();