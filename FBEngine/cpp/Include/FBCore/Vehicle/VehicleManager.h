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
        VehicleManager();
        ~VehicleManager();

        void preUpdate() override;
        void update() override;
        void postUpdate() override;

        SmartPtr<IVehicleController> createVehicle( hash64 type ) override;

        void destroyVehicle( SmartPtr<IVehicleController> vehicle ) override;

        void addVehicle( SmartPtr<IVehicleController> vehicle ) override;

        void removeVehicle( SmartPtr<IVehicleController> vehicle ) override;

    protected:
        mutable RecursiveMutex m_mutex;
        Array<SmartPtr<IVehicleController>> m_vehicles;
    };

}  // namespace fb

#endif  // VehicleManager_h__
