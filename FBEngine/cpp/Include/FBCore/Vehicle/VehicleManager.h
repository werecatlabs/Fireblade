#ifndef __VehicleManager_h__
#define __VehicleManager_h__

#include <FBCore/Interface/Vehicle/IVehicleManager.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{

    /** Simple vehicle manager implementation. */
    class VehicleManager : public IVehicleManager
    {
    public:
        /** Constructor. */
        VehicleManager();

        /** Destructor. */
        ~VehicleManager() override;

        /** @copydoc IVehicleManager::preUpdate */
        void preUpdate() override;

        /** @copydoc IVehicleManager::update */
        void update() override;

        /** @copydoc IVehicleManager::postUpdate */
        void postUpdate() override;

        /** @copydoc IVehicleManager::createVehicle */
        SmartPtr<IVehicleController> createVehicle( hash64 type ) override;

        /** @copydoc IVehicleManager::destroyVehicle */
        void destroyVehicle( SmartPtr<IVehicleController> vehicle ) override;

        /** @copydoc IVehicleManager::addVehicle */
        void addVehicle( SmartPtr<IVehicleController> vehicle ) override;

        /** @copydoc IVehicleManager::removeVehicle */
        void removeVehicle( SmartPtr<IVehicleController> vehicle ) override;

    protected:
        // The mutex to protect the vehicles array.
        mutable RecursiveMutex m_mutex;

        // The vehicles array.
        Array<SmartPtr<IVehicleController>> m_vehicles;
    };

}  // namespace fb

#endif  // VehicleManager_h__
