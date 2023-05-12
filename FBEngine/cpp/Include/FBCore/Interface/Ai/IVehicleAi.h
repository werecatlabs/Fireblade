#ifndef IVehicleAi_h__
#define IVehicleAi_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class IVehicleAi : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IVehicleAi() override = default;

        virtual SmartPtr<IVehicleController> getVehicleController() const = 0;
        virtual void setVehicleController( SmartPtr<IVehicleController> controller ) = 0;

        virtual SmartPtr<IVehicleAiManager> getVehicleManager() const = 0;
        virtual void setVehicleManager( SmartPtr<IVehicleAiManager> manager ) = 0;
    };
}  // end namespace fb

#endif  // IVehicleAi_h__
