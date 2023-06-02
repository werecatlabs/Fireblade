#ifndef IVehicleAiManager_h__
#define IVehicleAiManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    class IVehicleAiManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IVehicleAiManager() override = default;

        /**  */
        virtual void addVehicle( SmartPtr<IVehicleController> vehicle ) = 0;

        /**  */
        virtual void removeVehicle( SmartPtr<IVehicleController> vehicle ) = 0;

        /**  */
        virtual Array<SmartPtr<IVehicleController>> getVehicles() const = 0;

        /**  */
        virtual void setVehicles( const Array<SmartPtr<IVehicleController>> &vehicles ) = 0;
    };
}  // end namespace fb

#endif  // IVehicleAiManager_h__
