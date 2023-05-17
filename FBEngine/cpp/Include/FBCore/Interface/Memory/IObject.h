#ifndef __FB_IObject_h__
#define __FB_IObject_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Rtti/RttiClass.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{

    /**
     * An interface for an object that can be managed by the system.
     * This interface is a base class for all managed objects in the system.
     */
    class IObject
    {
    public:
        /**
         * Virtual destructor for the class, to ensure that destructors of derived classes are called correctly.
         * The destructor is marked as `override` to ensure that it overrides the base class destructor.
         */
        virtual ~IObject() = default;

        /**
         * Called before the object is updated.
         * Implementations of this function can perform any necessary pre-update processing.
         */
        virtual void preUpdate() = 0;

        /**
         * Called to update the object.
         * Implementations of this function can perform any necessary updating of the object.
         */
        virtual void update() = 0;

        /**
         * Called after the object is updated.
         * Implementations of this function can perform any necessary post-update processing.
         */
        virtual void postUpdate() = 0;

        /**
         * Returns a pointer to the `Handle` object associated with this object.
         * @return A pointer to the `Handle` object.
         */
        virtual Handle *getHandle() const = 0;

        /**
         * Checks if the object is valid.
         * @return Returns a boolean indicating if the object is valid.
         */
        virtual bool isValid() const = 0;

        /**
         * Gets the creator data associated with this object.
         * @return A pointer to the creator data.
         */
        virtual void *getCreatorData() const = 0;

        /**
         * Sets the creator data associated with this object.
         * @param data A pointer to the creator data.
         */
        virtual void setCreatorData( void *data ) = 0;

        /**
         * Gets the factory data associated with this object.
         * @return An integer representing the factory data.
         */
        virtual hash32 getFactoryData() const = 0;

        /**
         * Sets the factory data associated with this object.
         * @param data An integer representing the factory data.
         */
        virtual void setFactoryData( hash32 data ) = 0;

        /**
         * Gets the object data as a string.
         * @return The object data as a `String` object.
         */
        virtual String toString() const = 0;

        /**
         * Gets the user data associated with this object.
         * @return A pointer to the user data.
         */
        virtual void *getUserData() const = 0;

        /**
         * Sets the user data associated with this object.
         * @param data A pointer to the user data.
         */
        virtual void setUserData( void *data ) = 0;

        /**
         * Gets the user data attached to this object with the specified ID.
         * @param id The ID of the user data.
         * @return A pointer to the user data.
         */
        virtual void *getUserData( hash32 id ) const = 0;

        /**
         * Sets the user data attached to this object with the specified ID.
         * @param id The ID of the user data.
         * @param userData A pointer to the user data.
         */
        virtual void setUserData( hash32 id, void *userData ) = 0;

        /**
         * @brief Check if the current object is derived from a specified type.
         *
         * This function checks if the current object is derived from the specified type.
         * It uses the TypeManager to retrieve the type information of the current object
         * and then calls the TypeManager's isDerived() function to perform the derivation check.
         *
         * @param type The type ID to check against.
         * @return True if the current object is derived from the specified type, false otherwise.
         */
        virtual bool derived( u32 type ) const
        {
            auto typeManager = TypeManager::instance();

            auto typeInfo = getTypeInfo();
            return typeManager->isDerived( typeInfo, type );
        }

        /**
         * @brief Check if the current object is of exactly the specified type.
         *
         * This function checks if the current object is of exactly the specified type.
         * It uses the TypeManager to retrieve the type information of the current object
         * and then calls the TypeManager's isExactly() function to perform the type check.
         *
         * @param type The type ID to check against.
         * @return True if the current object is of exactly the specified type, false otherwise.
         */
        virtual bool exactly( u32 type ) const
        {
            auto typeManager = TypeManager::instance();

            auto typeInfo = getTypeInfo();
            return typeManager->isExactly( typeInfo, type );
        }

        /**
         * Checks if the class represented by this `IObject` instance is derived from a specified class `B`.
         * This function uses type information to perform the check.
         * @return Returns `true` if the class is derived from `B`, `false` otherwise.
         */
#if FB_CPP_STANDARD >= FB_CPP_2020
        template <class B>
            requires requires { B::typeInfo(); } bool
        isDerived() const
        {
            auto typeInfo = getTypeInfo();
            if( typeInfo != 0 )
            {
                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto otherTypeInfo = B::typeInfo();
                return typeManager->isDerived( typeInfo, otherTypeInfo );
            }

            return false;
        }
#else
        template <class B>
        bool isDerived() const
        {
            auto typeInfo = getTypeInfo();
            if( typeInfo != 0 )
            {
                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto otherTypeInfo = B::typeInfo();
                return typeManager->isDerived( typeInfo, otherTypeInfo );
            }

            return false;
        }
#endif

        /**
         * Checks if the class represented by this `IObject` instance is exactly of type `B`.
         * This function uses type information to perform the check.
         * @return Returns `true` if the class is of type `B`, `false` otherwise.
         */
#if FB_CPP_STANDARD >= FB_CPP_2020
        template <class B>
            requires requires { B::typeInfo(); } bool
        isExactly() const
        {
            auto typeInfo = getTypeInfo();
            if( typeInfo != 0 )
            {
                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto otherTypeInfo = B::typeInfo();
                return typeManager->isExactly( typeInfo, otherTypeInfo );
            }

            return false;
        }
#else
        template <class B>
        bool isExactly() const
        {
            auto typeInfo = getTypeInfo();
            if( typeInfo != 0 )
            {
                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto otherTypeInfo = B::typeInfo();
                return typeManager->isExactly( typeInfo, otherTypeInfo );
            }

            return false;
        }
#endif

        FB_OBJECT_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // __FB_IObject_h__
