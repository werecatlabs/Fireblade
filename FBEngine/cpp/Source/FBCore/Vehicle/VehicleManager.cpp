#include <FBCore/FBCorePCH.h>
#include <FBCore/Vehicle/VehicleManager.h>
#include <FBCore/Vehicle/VehicleController.h>
#include <FBCore/Interface/Vehicle/IVehicleController.h>

namespace fb
{
    VehicleManager::VehicleManager()
    {
    }

    VehicleManager::~VehicleManager()
    {
    }

    void VehicleManager::preUpdate()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        for(auto vehicle : m_vehicles)
        {
            vehicle->preUpdate();
        }
    }

    void VehicleManager::update()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        for( auto vehicle : m_vehicles )
        {
            vehicle->update();
        }
    }

    void VehicleManager::postUpdate()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        for( auto vehicle : m_vehicles )
        {
            vehicle->postUpdate();
        }
    }

    SmartPtr<IVehicleController> VehicleManager::createVehicle( hash64 type )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        auto vehicleController = fb::make_ptr<VehicleController>();
        m_vehicles.push_back( vehicleController );
        return vehicleController;
    }

    void VehicleManager::destroyVehicle( SmartPtr<IVehicleController> vehicle )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_vehicles.erase( std::remove( m_vehicles.begin(), m_vehicles.end(), vehicle ) );
    }

    void VehicleManager::addVehicle( SmartPtr<IVehicleController> vehicle )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_vehicles.push_back( vehicle );
    }

    void VehicleManager::removeVehicle( SmartPtr<IVehicleController> vehicle )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_vehicles.erase( std::remove( m_vehicles.begin(), m_vehicles.end(), vehicle ) );
    }
}  // namespace fb
