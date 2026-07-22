//
// Created by stuka on 22.07.2026.
//

#include "WheeledVehicle.h"

#include "PhysicsWorld3D.h"

SGCore::WheeledVehicle::~WheeledVehicle()
{
    auto lockedWorld = m_parentPhysicsWorld.lock();
    if(!lockedWorld) return;

    if(m_vehicle)
    {
        lockedWorld->removeVehicle(m_vehicle.get());
    }
}

void SGCore::WheeledVehicle::configure(WheeledVehicle& vehicle, Rigidbody3D& vehicleBody)
{

}
