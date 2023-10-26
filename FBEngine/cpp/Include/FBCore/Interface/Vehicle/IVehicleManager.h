#ifndef IVehicleManager_h__
#define IVehicleManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{

    /**
     * @class IVehicleManager
     * @brief Interface for a vehicle manager, responsible for managing vehicle controllers and their interactions
     */
    class IVehicleManager : public ISharedObject
    {
    public:
        /**
         * @brief Destructor
         */
        ~IVehicleManager() override = default;

        /**
         * @brief Creates a vehicle controller of the specified type
         * @param type The type of vehicle controller to create
         * @return A smart pointer to the newly created vehicle controller
         */
        virtual SmartPtr<IVehicleController> createVehicle( hash64 type ) = 0;

        /**
         * @brief Destroys the specified vehicle controller
         * @param vehicle The vehicle controller to destroy
         */
        virtual void destroyVehicle( SmartPtr<IVehicleController> vehicle ) = 0;

        /**
         * @brief Adds a vehicle controller to the manager
         * @param vehicle The vehicle controller to add
         */
        virtual void addVehicle( SmartPtr<IVehicleController> vehicle ) = 0;

        /**
         * @brief Removes a vehicle controller from the manager
         * @param vehicle The vehicle controller to remove
         */
        virtual void removeVehicle( SmartPtr<IVehicleController> vehicle ) = 0;

        /**
         * @brief Creates a vehicle controller by type
         * @tparam T The type of vehicle controller to create
         * @return A smart pointer to the newly created vehicle controller
         */
        template <class T>
        SmartPtr<T> createVehicleByType();

        FB_CLASS_REGISTER_DECL;
    };

    template <class T>
    SmartPtr<T> IVehicleManager::createVehicleByType()
    {
        auto typeInfo = T::typeInfo();
        FB_ASSERT( typeInfo != 0 );

        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeHash = typeManager->getHash( typeInfo );
        FB_ASSERT( typeHash != 0 );

        auto vehicle = createVehicle( typeHash );
        FB_ASSERT( fb::dynamic_pointer_cast<T>( vehicle ) );
        return fb::static_pointer_cast<T>( vehicle );
    }

}  // end namespace fb

#endif  // IVehicleManager_h__
