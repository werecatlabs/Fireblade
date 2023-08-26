#ifndef ISharedObject_h__
#define ISharedObject_h__

#include <FBCore/Interface/Memory/IObject.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/System/RttiClassDefinition.h>

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
            ScopedLock( SmartPtr<ISharedObject> sharedObject );

            /**
             * Destructs a `ScopeLock` object and unlocks the specified shared object.
             *
             * @see ISharedObject, SmartPtr, IObject
             */
            ~ScopedLock();

        protected:
            /** A pointer to the shared object to lock. */
            SmartPtr<ISharedObject> m_sharedObject;
        };

        ISharedObject();

        /**
         * Virtual destructor for the `ISharedObject` class.
         *
         * The destructor is marked as `override` to ensure that it overrides the base class destructor.
         *
         * @see IObject
         */
        ~ISharedObject() override;

        /**
         * Adds a weak reference to the shared object and increments the reference counter.
         *
         * @returns The new reference count.
         */
        s32 addWeakReference();

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
        s32 addWeakReference( void *address, const c8 *file, u32 line, const c8 *func );

        /**
         * Removes a weak reference from the shared object and decrements the reference counter.
         *
         * @returns `true` if the object was deleted.
         */
        bool removeWeakReference();

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
        bool removeWeakReference( void *address, const c8 *file = "??", u32 line = 0,
                                  const c8 *func = "??" );

        /**
         * Adds a reference to the object and increments the reference counter.
         * @param address The address where the reference is being added.
         * @param file The source file where the reference is being added.
         * @param line The line number of the source file where the reference is being added.
         * @param func The name of the function where the reference is being added.
         * @returns The new reference count.
         */
        s32 addReference( void *address, const c8 *file, u32 line, const c8 *func );

        /**
         * Adds a reference to the object and increments the reference counter.
         *
         * @return The new reference count.
         */
        s32 addReference();

        /** Removes a reference to the object and decrements the reference counter.
        @returns
            Returns true if the object was deleted.
        */
        bool removeReference( void *address, const c8 *file = "??", u32 line = 0,
                              const c8 *func = "??" );

        /** Removes a reference to the object and decrements the reference counter.
        @return Returns true if the object was deleted.
        */
        bool removeReference();

        /** Gets the reference count.
        @return The number of references.
        */
        s32 getReferences() const;

        /** Gets the reference count.
        @return The number of references.
        */
        s32 getWeakReferences() const;

        /** Used to load the object.
        @param data An object containing the necessary data. Can be null.
        */
        virtual void load( SmartPtr<ISharedObject> data );

        /** Used to reload the object.
        @param data An object containing the necessary data. Can be null.
        */
        virtual void reload( SmartPtr<ISharedObject> data );

        /** Used to unload the object.
        @param data An object containing the necessary data. Can be null.
        */
        virtual void unload( SmartPtr<ISharedObject> data );

        /** Gets the current loading state.
        @returns
            Returns the object's loading state.*/
        virtual const Atomic<LoadingState> &getLoadingState() const;

        /** Set the current loading state.
        @param state An enum value to indicate the loading state.
        */
        virtual void setLoadingState( const Atomic<LoadingState> &state );

        /** Gets if the object is loading.
        @returns
            Returns true if the object is loading.*/
        virtual bool isLoading() const;

        /** Gets if the object is queued for loading.
        @returns
            Returns true if the object is loading.*/
        virtual bool isLoadingQueued() const;

        /** Gets if the object is loaded.
        @returns
            Returns true if the object was loaded.*/
        bool isLoaded() const;

        /** Gets if a function call is thread safe.
        @returns
            Returns true if the object was loaded.*/
        virtual bool isThreadSafe() const;

        /** Gets if the object is allocated.
        @returns
            Returns true if the object is allocated.*/
        bool isAlive() const;

        /**
         * Sets if the object is a pool element.
         *
         * @param poolElement A boolean indicating if the object is a pool element.
         */
        virtual void setPoolElement( bool poolElement );

        /**
         * Gets if the object is a pool element.
         *
         * @return True if the object is a pool element, false otherwise.
         */
        virtual bool isPoolElement() const;

        /** Get object data as a json structure.
        @return An object containing the object data.
        */
        virtual SmartPtr<ISharedObject> toData() const;

        /** Set object data from json structure.
        @param data An object containing the object data.
        */
        virtual void fromData( SmartPtr<ISharedObject> data );

        /** Used to get child objects that make up the object composition.
        @return An array with the child objects.
        */
        virtual Array<SmartPtr<ISharedObject>> getChildObjects() const;

        /** Gets the object listeners.
        @return The listener object.
        */
        virtual Array<SmartPtr<IEventListener>> getObjectListeners() const;

        /** Gets the number of listeners.
        @return The number of listener objects.
        */
        virtual u32 getNumListeners() const;

        /** Checks if object has listener.
        @param listener The listener object.
        */
        virtual bool hasObjectListener( SmartPtr<IEventListener> listener ) const;

        /** Adds an object listener.
        @param listener The listener object.
        */
        virtual void addObjectListener( SmartPtr<IEventListener> listener );

        /** Removes an object listener.
        @param listener The listener object.
        */
        virtual void removeObjectListener( SmartPtr<IEventListener> listener );

        /** Gets the object listener.
         */
        virtual SmartPtr<IEventListener> findObjectListener( const String &id ) const;

        /** Gets the object listener.
         */
        virtual ISharedObjectListener *getSharedObjectListener() const;

        /** Sets the object listener.
        @param listener The listener object.
        */
        virtual void setSharedObjectListener( ISharedObjectListener *listener );

        /** Gets if reference tracking is enabled. */
        virtual bool getEnableReferenceTracking() const;

        /** Sets if reference tracking is enabled. */
        virtual void setEnableReferenceTracking( bool referenceTracking );

        /** Gets if this object should be garbage collected.
        @return Returns a boolean indicating whether this object should be garbage collected.
        @author	Mczane Gabriel Desir
        @version 1.0
        */
        virtual bool isGarbageCollected() const;

        /** Sets if this object should be garbage collected.
        @param garbageCollected A boolean indicating whether this object should be garbage collected.
        @author	Mczane Gabriel Desir
        @version 1.0
        */
        virtual void setGarbageCollected( bool garbageCollected );

        /**
         * Gets script data associated with this object.
         * @return A pointer to the script data.
         */
        SmartPtr<ISharedObject> getScriptData() const;

        /**
         * Sets the script data associated with this object.
         * Script data is a `SmartPtr` to a shared object that can be used to store arbitrary data related to the object's behavior in a script.
         * @param data A `SmartPtr` to the shared object containing the script data.
         */
        void setScriptData( SmartPtr<ISharedObject> data );

        /** Locks the object for thread safety. */
        virtual void lock();

        /** Unlocks the object for thread safety. */
        virtual void unlock();

        /**
         * Gets the shared object from this instance.
         *
         * @tparam B The shared object type to return.
         * @return The shared object instance.
         */
        template <class B>
        const SmartPtr<B> getSharedFromThis() const;

        /**
         * Gets the shared object of type B from this instance.
         *
         * @tparam B The type of the shared object to return, which must be a derived class of ISharedObject.
         * @return A SmartPtr<B> pointing to the same object as the 'this' pointer.
         */
        template <class B>
        SmartPtr<B> getSharedFromThis();

        FB_CLASS_REGISTER_DECL;

    protected:
        void destroySharedObject();

        SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> getObjectListenersPtr() const;

        void setObjectListenersPtr( SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p );

        atomic_s32 m_references = 1;
        atomic_s32 m_weakReferences = 0;

        Atomic<LoadingState> m_loadingState;

        SmartPtr<ISharedObject> m_scriptData;

        AtomicRawPtr<ISharedObjectListener> m_sharedObjectListener;
        SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> m_sharedEventListeners;
    };

    inline s32 ISharedObject::addWeakReference()
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif
#endif

        return ++m_weakReferences;
    }

    inline s32 ISharedObject::addReference()
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_STRONG_REFERENCES
        
        auto references = gc.addReference( T::typeInfo(), m_objectId );

        // FB_ASSERT( isGarbageCollected() && references > 0 );
        FB_ASSERT( references < 1e10 );

        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );

        return references;
#    else
        
        return gc.addReference( T::typeInfo(), m_objectId );
#    endif
#else
        return ++m_references;
#endif
    }

    inline bool ISharedObject::removeReference()
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_STRONG_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#    endif
#endif

        if( --m_references == 0 )
        {
            destroySharedObject();
            return true;
        }

        return false;
    }

    inline s32 ISharedObject::getWeakReferences() const
    {
        return m_weakReferences;
    }

    inline s32 ISharedObject::getReferences() const
    {
        return m_references;
    }

    inline bool ISharedObject::isAlive() const
    {
        return ( m_objectFlags & GC_FLAG_OBJECT_ALIVE ) != 0;
    }

    inline bool ISharedObject::isLoaded() const
    {
        return m_loadingState == LoadingState::Loaded;
    }

    inline SmartPtr<ISharedObject> ISharedObject::getScriptData() const
    {
        return m_scriptData;
    }

    inline void ISharedObject::setScriptData( SmartPtr<ISharedObject> data )
    {
        m_scriptData = data;
    }

    template <class B>
    const SmartPtr<B> ISharedObject::getSharedFromThis() const
    {
        auto p = (B *)this;
        return SmartPtr<B>( p );
    }

    template <class B>
    SmartPtr<B> ISharedObject::getSharedFromThis()
    {
        auto p = static_cast<B *>( this );
        return SmartPtr<B>( p );
    }

}  // end namespace fb

#endif  // ISharedObject_h__
