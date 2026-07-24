//
// Created by stuka on 24.07.2026.
//

#include "VehicleWheel.h"

#include "WheeledVehicle.h"

SGCore::VehicleWheel::~VehicleWheel() noexcept
{
    if(!m_parentVehicle) return;

    m_parentVehicle->removeWheelImpl(ComponentBase::getThisEntity(), false);
}
