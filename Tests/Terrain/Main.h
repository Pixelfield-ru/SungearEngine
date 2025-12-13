//
// Created by stuka on 19.04.2025.
//

#pragma once

void coreInit();

void saveTerrainDisplacementMap() noexcept;

void onUpdate(const double& dt, const double& fixedDt);
void onFixedUpdate(const double& dt, const double& fixedDt);

int main();
