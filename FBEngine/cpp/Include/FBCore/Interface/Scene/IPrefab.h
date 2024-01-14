#ifndef __IPrefab_h__
#define __IPrefab_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace scene
    {

        /**
         * Interface for a prefab. Used to construct actors using pre-defined data.
         */
        class IPrefab : public IResource
        {
        public:
            /**
             * Virtual destructor.
             */
            ~IPrefab() override = default;

            /**
             * Creates an actor instance.
             *
             * @return The created actor instance.
             */
            virtual SmartPtr<IActor> createActor() = 0;

            /**
             * Gets the actor this prefab instantiates.
             *
             * @return The actor instance.
             */
            virtual SmartPtr<IActor> getActor() const = 0;

            /**
             * Sets the actor this prefab instantiates.
             *
             * @param actor The actor instance to set.
             */
            virtual void setActor( SmartPtr<IActor> actor ) = 0;

            /**
             * Gets the data associated with this prefab.
             *
             * @return The data.
             */
            virtual SmartPtr<ISharedObject> getData() const = 0;

            /**
             * Sets the data associated with this prefab.
             *
             * @param data The data to set.
             */
            virtual void setData( SmartPtr<ISharedObject> data ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // IResource_h__
