#ifndef ISharedObject_h__
#define ISharedObject_h__

#include <FBCore/Interface/Memory/IObject.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Base/Array.h>

#if FB_TRACK_REFERENCES
#    include <FBCore/Memory/ObjectTracker.h>
#endif

namespace fb
{

    /**
     * An interface for a shared object.
     *
     * This class is derived from `IObject` and defines an interface for a shared object.
     *
     * The class also contains a nested `ScopeLock` class which provides a scoped locking mechanism for the object.
     * The class is marked `final` to prevent further inheritance.
     *
     * @see IObject, ScopeLock
     */
    class ISharedObject : public IObject
    {
    public:
        /**
         * A nested class that provides a scoped locking mechanism for the shared object.
         *
         * The `ScopeLock` class locks the specified shared object during its construction and unlocks it during its destruction.
         * This class is used to ensure that a shared object is thread-safe by preventing concurrent access to the object's data.
         * The shared object is unlocked when the `ScopeLock` object is destructed.
         *
         * @note If the specified shared object is `nullptr`, the `ScopeLock` object is constructed without locking any object.
         *
         * @see ISharedObject, SmartPtr, IObject
         */
        class ScopedLock
        {
        public:
            /**
             * Constructs a `ScopeLock` object and locks the specified shared object.
             *
             * @param sharedObject A `SmartPtr` object that represents the shared object to lock.
             *
             * @see ISharedObject, SmartPtr, IObject
             */
            ScopedLock( SmartPtr<ISharedObject> sharedObject ) : m_sharedObject( sharedObject )
            {
                if( m_sharedObject )
                {
                    m_sharedObject->lock();
                }
            }

            /**
             * Destructs a `ScopeLock` object and unlocks the specified shared object.
             *
             * @see ISharedObject, SmartPtr, IObject
             */
            ~ScopedLock()
            {
                if( m_sharedObject )
                {
                    m_sharedObject->unlock();
                }
            }

        protected:
            /** A pointer to the shared object to lock. */
            SmartPtr<ISharedObject> m_sharedObject;
        };

        /**
         * Virtual destructor for the `ISharedObject` class.
         *
         * The destructor is marked as `override` to ensure that it overrides the base class destructor.
         *
         * @see IObject
         */
        ~ISharedObject() override = default;

        /**
         * Adds a weak reference to the shared object and increments the reference counter.
         *
         * @returns The new reference count.
         */
        virtual s32 addWeakReference() = 0;

        /**
         * Adds a weak reference to the shared object and increments the reference counter.
         *
         * @param address The memory address of the reference to add.
         * @param file The name of the file where the reference is being added.
         * @param line The line number where the reference is being added.
         * @param func The name of the function where the reference is being added.
         *
         * @returns The new reference count.
         */
        virtual s32 addWeakReference( void *address, const c8 *file, u32 line, const c8 *func ) = 0;

        /**
         * Removes a weak reference from the shared object and decrements the reference counter.
         *
         * @returns `true` if the object was deleted.
         */
        virtual bool removeWeakReference() = 0;

        /**
         * Decrements the weak reference count of the object and removes the weak reference tracking information.
         *
         * @param address A pointer to the address holding the reference. Can be null.
         * @param file The name of the file in which the reference occurs. Can be null.
         * @param line The line number of the reference in the file. Can be 0.
         * @param func The name of the function in which the reference occurs. Can be null.
         *
         * @return True if the reference count is 0 after decrementing, indicating that the object can be deleted.
         */
        virtual bool removeWeakReference( void *address, const c8 *file = "??", u32 line = 0,
                                          const c8 *func = "??" ) = 0;

        /**
         * Adds a reference to the object and increments the reference counter.
         * @param address The address where the reference is being added.
         * @param file The source file where the reference is being added.
         * @param line The line number of the source file where the reference is being added.
         * @param func The name of the function where the reference is being added.
         * @returns The new reference count.
         */
        virtual s32 addReference( void *address, const c8 *file, u32 line, const c8 *func ) = 0;

        /**
         * Adds a reference to the object and increments the reference counter.
         *
         * @return The new reference count.
         */
        virtual s32 addReference() = 0;

        /** Removes a reference to the object and decrements the reference counter.
        @returns
            Returns true if the object was deleted.
        */
        virtual bool removeReference( void *address, const c8 *file = "??", u32 line = 0,
                                      const c8 *func = "??" ) = 0;

        /** Removes a reference to the object and decrements the reference counter.
        @return Returns true if the object was deleted.
        */
        virtual bool removeReference() = 0;

        /** Gets the reference count.
        @return The number of references.
        */
        virtual s32 getReferences() const = 0;

        /** Gets the reference count.
        @return The number of references.
        */
        virtual s32 getWeakReferences() const = 0;

        /** Used to load the object.
        @param data An object containing the necessary data. Can be null.
        */
        virtual void load( SmartPtr<ISharedObject> data ) = 0;

        /** Used to reload the object.
        @param data An object containing the necessary data. Can be null.
        */
        virtual void reload( SmartPtr<ISharedObject> data ) = 0;

        /** Used to unload the object.
        @param data An object containing the necessary data. Can be null.
        */
        virtual void unload( SmartPtr<ISharedObject> data ) = 0;

        /** Gets the current loading state.
        @returns
            Returns the object's loading state.*/
        virtual const Atomic<LoadingState> &getLoadingState() const = 0;

        /** Set the current loading state.
        @param state An enum value to indicate the loading state.
        */
        virtual void setLoadingState( const Atomic<LoadingState> &state ) = 0;

        /** Gets if the object is loading.
        @returns
            Returns true if the object is loading.*/
        virtual bool isLoading() const = 0;

        /** Gets if the object is queued for loading.
        @returns
            Returns true if the object is loading.*/
        virtual bool isLoadingQueued() const = 0;

        /** Gets if the object is loaded.
        @returns
            Returns true if the object was loaded.*/
        virtual bool isLoaded() const = 0;

        /** Gets if a function call is thread safe.
        @returns
            Returns true if the object was loaded.*/
        virtual bool isThreadSafe() const = 0;

        /** Gets if the object is allocated.
        @returns
            Returns true if the object is allocated.*/
        virtual bool isAlive() const = 0;

        /**
         * Sets if the object is a pool element.
         *
         * @param poolElement A boolean indicating if the object is a pool element.
         */
        virtual void setPoolElement( bool poolElement ) = 0;

        /**
         * Gets if the object is a pool element.
         *
         * @return True if the object is a pool element, false otherwise.
         */
        virtual bool isPoolElement() const = 0;

        /** Get object data as a json structure.
        @return An object containing the object data.
        */
        virtual SmartPtr<ISharedObject> toData() const = 0;

        /** Set object data from json structure.
        @param data An object containing the object data.
        */
        virtual void fromData( SmartPtr<ISharedObject> data ) = 0;

        /** Used to get child objects that make up the object composition.
        @return An array with the child objects.
        */
        virtual Array<SmartPtr<ISharedObject>> getChildObjects() const = 0;

        /** Gets the object listeners.
        @return The listener object.
        */
        virtual Array<SmartPtr<IEventListener>> getObjectListeners() const = 0;

        /** Gets the number of listeners.
        @return The number of listener objects.
        */
        virtual u32 getNumListeners() const = 0;

        /** Checks if object has listener.
        @param listener The listener object.
        */
        virtual bool hasObjectListener( SmartPtr<IEventListener> listener ) const = 0;

        /** Adds an object listener.
        @param listener The listener object.
        */
        virtual void addObjectListener( SmartPtr<IEventListener> listener ) = 0;

        /** Removes an object listener.
        @param listener The listener object.
        */
        virtual void removeObjectListener( SmartPtr<IEventListener> listener ) = 0;

        /** Gets the object listener.
         */
        virtual SmartPtr<IEventListener> findObjectListener( const String &id ) const = 0;

        /** Gets the object listener.
         */
        virtual ISharedObjectListener *getSharedObjectListener() const = 0;

        /** Sets the object listener.
        @param listener The listener object.
        */
        virtual void setSharedObjectListener( ISharedObjectListener *listener ) = 0;

        /** Gets if reference tracking is enabled. */
        virtual bool getEnableReferenceTracking() const = 0;

        /** Sets if reference tracking is enabled. */
        virtual void setEnableReferenceTracking( bool referenceTracking ) = 0;

        /** Gets if this object should be garbage collected.
        @return Returns a boolean indicating whether this object should be garbage collected.
        @author	Mczane Gabriel Desir
        @version 1.0
        */
        virtual bool isGarbageCollected() const = 0;

        /** Sets if this object should be garbage collected.
        @param garbageCollected A boolean indicating whether this object should be garbage collected.
        @author	Mczane Gabriel Desir
        @version 1.0
        */
        virtual void setGarbageCollected( bool garbageCollected ) = 0;

        /** The object id. */
        virtual u32 getObjectId() const = 0;

        /**
         * Gets script data associated with this object.
         * @return A pointer to the script data.
         */
        virtual SmartPtr<ISharedObject> getScriptData() const = 0;

        /**
         * Sets the script data associated with this object.
         * Script data is a `SmartPtr` to a shared object that can be used to store arbitrary data related to the object's behavior in a script.
         * @param data A `SmartPtr` to the shared object containing the script data.
         */
        virtual void setScriptData( SmartPtr<ISharedObject> data ) = 0;

        /** Sets up the garbage collector data. */
        virtual void setupGarbageCollectorData() = 0;

        /** Locks the object for thread safety. */
        virtual void lock() = 0;

        /** Unlocks the object for thread safety. */
        virtual void unlock() = 0;

        /**
         * Gets the shared object from this instance.
         *
         * @tparam B The shared object type to return.
         * @return The shared object instance.
         */
        template <class B>
        const SmartPtr<B> getSharedFromThis() const
        {
            return SmartPtr<B>( (B *)this );
        }

        /**
         * Gets the shared object of type B from this instance.
         *
         * @tparam B The type of the shared object to return, which must be a derived class of ISharedObject.
         * @return A SmartPtr<B> pointing to the same object as the 'this' pointer.
         */
        template <class B>
        SmartPtr<B> getSharedFromThis()
        {
            return SmartPtr<B>( (B *)this );
        }

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ISharedObject_h__
