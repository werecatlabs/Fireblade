#ifndef GarbageCollectorData_h__
#define GarbageCollectorData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/FixedArray.h>
#include <FBCore/Base/Handle.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/ConcurrentQueue.h>
#include <FBCore/Base/UtilityTypes.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{

    /**
     * The GarbageCollectorData class manages a single type of object in the garbage collector.
     */
    class GarbageCollectorData
    {
    public:
        /**
         * Constructor for the GarbageCollectorData class.
         */
        GarbageCollectorData();

        /**
         * Destructor for the GarbageCollectorData class.
         */
        ~GarbageCollectorData();

        /**
         * Loads all the garbage collector data for this type of object.
         */
        void load();

        /**
         * Unloads all the garbage collector data for this type of object.
         */
        void unload();

        /**
         * Updates the garbage collector for this type of object.
         * Should be called at regular intervals.
         */
        void update();

        /**
         * Adds an object for garbage collection.
         * @param object The object to add for garbage collection.
         * @return The ID of the object.
         */
        u32 addObject( ISharedObject *object );

        /**
         * Removes an object from garbage collection.
         * @param id The ID of the object to remove.
         */
        void removeObject( u32 id );

        /**
         * Destroys an object.
         * @param id The ID of the object to destroy.
         */
        void destroyObject( u32 id );

        /**
         * Gets an object's loading state.
         * @param id The ID of the object.
         * @return The object's loading state.
         */
        const Atomic<LoadingState> &getLoadingState( u32 id ) const;

        /**
         * Gets a pointer to an object's loading state.
         * @param id The ID of the object.
         * @return A pointer to the object's loading state.
         */
        Atomic<LoadingState> *getLoadingStatePtr( u32 id ) const;

        /**
         * Sets an object's loading state.
         * @param id The ID of the object.
         * @param state The new loading state of the object.
         */
        void setLoadingState( u32 id, const Atomic<LoadingState> &state );

        /**
         * Gets an object's listener.
         * @param id The ID of the object.
         * @return The object's listener.
         */
        ISharedObjectListener *getObjectListener( u32 id ) const;

        /**
         * Sets an object's listener.
         * @param id The ID of the object.
         * @param listener The new listener for the object.
         */
        void setObjectListener( u32 id, ISharedObjectListener *listener );

        /**
         * Gets an object's handle.
         * @param id The ID of the object.
         * @return The object's handle.
         */
        Handle *getHandle( u32 id ) const;

        /**
         * Gets an object's user data.
         * @param id The ID of the object.
         * @return The object's user data.
         */
        void *getUserData( u32 id ) const;

        /**
         * Sets an object's user data.
         * @param id The ID of the object.
         * @param userData The new user data for the object.
         */
        void setUserData( u32 id, void *userData );

        /**
         * Sets an object's creator data.
         * @param id The ID of the object.
         * @param creatorData The new creator data for the object.
         */
        void setCreatorData( u32 id, void *creatorData );

        /**
         * Gets an object's creator data.
         * @param id The ID of the object.
         * @return The object's creator data.
         */
        void *getCreatorData( u32 id ) const;

        /**
         * Gets an object's factory data.
         *
         * @param id The ID of the object to get the factory data for.
         *
         * @return The factory data of the object.
         */
        hash32 getFactoryData( u32 id ) const;

        /**
         * Sets an object's factory data.
         *
         * @param id The ID of the object to set the factory data for.
         * @param factoryData The new factory data for the object.
         */
        void setFactoryData( u32 id, hash32 factoryData );

        /**
         * Gets an object's reference count.
         *
         * @param id The ID of the object to get the reference count for.
         *
         * @return The reference count of the object.
         */
        s32 getReferences( u32 id ) const;

        /**
         * Sets an object's reference count.
         *
         * @param id The ID of the object to set the reference count for.
         * @param references The new reference count for the object.
         */
        void setReferences( u32 id, s32 references );

        /**
         * Adds an object reference.
         *
         * @param id The ID of the object to add the reference to.
         *
         * @return The new reference count of the object.
         */
        s32 addReference( u32 id );

        /**
         * Removes an object reference.
         *
         * @param id The ID of the object to remove the reference from.
         *
         * @return The new reference count of the object.
         */
        s32 removeReference( u32 id );

        /**
         * Updates the reference count of all objects in the garbage collector data.
         */
        void updateReferences();

        /**
         * Sets an object's flag.
         *
         * @param id The ID of the object to set the flag for.
         * @param flag The flag to set.
         * @param value The value of the flag.
         */
        void setFlag( u32 id, u8 flag, bool value );

        /**
         * Gets an object's flag.
         *
         * @param id The ID of the object to get the flag for.
         * @param flag The flag to get.
         *
         * @return The value of the flag.
         */
        bool getFlag( u32 id, u8 flag ) const;

        /**
         * Gets a pointer to an object's flag.
         *
         * @param id The ID of the object to get the flag pointer for.
         *
         * @return A pointer to the object's flag.
         */
        atomic_u8 *getFlagPtr( u32 id ) const;

        /**
         * Gets an array of objects.
         *
         * @return The array of objects.
         */
        Array<ISharedObject *> getObjects() const;

        /**
         * Gets the object pointer at the given index.
         *
         * @param id The id of the object.
         *
         * @return The object pointer.
         */
        ISharedObject **getObjectPtr( u32 id ) const;

        /**
         * Gets an object pointer at the given index.
         *
         * @param id The id of the object.
         *
         * @return The object pointer.
         */
        ISharedObject *getObject( u32 id ) const;

        /**
         * Sets the object pointer at the given index.
         *
         * @param id The id of the object.
         * @param object The object pointer to set.
         */
        void setObject( u32 id, ISharedObject *object );

        /**
         * Gets the pointer to the reference count for the given object.
         *
         * @param id The id of the object.
         *
         * @return The pointer to the reference count.
         */
        atomic_s32 *getReferencesPtr( u32 id );

        /**
         * Gets the garbage collector mode.
         *
         * @return The garbage collector mode.
         */
        GarbageCollectorMode getMode() const;

        /**
         * Sets the garbage collector mode.
         *
         * @param mode The garbage collector mode.
         */
        void setMode( GarbageCollectorMode mode );

        /**
         * Performs garbage collection.
         */
        void collect();

        /**
         * Checks whether a given slot is free.
         *
         * @param slot The slot to check.
         *
         * @return True if the slot is free; false otherwise.
         */
        bool isFreeSlot( u32 slot );

        /**
         * Queues the given object for destruction.
         *
         * @param id The id of the object to be destroyed.
         */
        void queueFreedObject( u32 id );

        /**
         * Swaps the object pointer at the given index with the specified object.
         *
         * @param id The id of the object to swap.
         * @param object The object to swap with.
         *
         * @return The old object pointer.
         */
        ISharedObject *swapObject( u32 id, ISharedObject *object );

        /**
         * Gets the loading state for this object.
         *
         * @return The loading state for this object.
         */
        Atomic<LoadingState> &getLoadingState();

        /**
         * Gets the loading state for this object.
         *
         * @return The loading state for this object.
         */
        const Atomic<LoadingState> &getLoadingState() const;

        /**
         * Sets the loading state for this object.
         *
         * @param state The new loading state.
         */
        void setLoadingState( Atomic<LoadingState> state );

        /**
         * Reserves memory for a number of objects.
         *
         * @param size The number of objects to reserve memory for.
         */
        void reserve( size_t size );

        /**
         * Gets the size of this data block in bytes.
         *
         * @return The size of this data block in bytes.
         */
        size_t getSize() const;

        /**
         * Gets the type group of this data block.
         *
         * @return The type group of this data block.
         */
        TypeGroups getTypeGroup() const;

        /** Sets the type group of this garbage collector data object.
         *  @param typeGroup The type group to set.
         */
        void setTypeGroup( TypeGroups typeGroup );

        /** Checks whether this garbage collector data object is valid.
         *  @return True if the object is valid, false otherwise.
         */
        bool isValid() const;

    protected:
        // Queues for adding, removing, and freeing object references.
        ConcurrentArray<atomic_u32> m_addReference;
        ConcurrentArray<atomic_u32> m_removeReference;
        ConcurrentArray<atomic_u32> m_removeQueue;

        // Arrays for storing objects and associated data.
        ConcurrentArray<Atomic<ISharedObject *>> m_objects;
        ConcurrentArray<String> m_objectNames;
        ConcurrentArray<Atomic<LoadingState>> m_loadingStates;
        ConcurrentArray<Atomic<ISharedObjectListener *>> m_listeners;
        ConcurrentArray<Handle> m_handles;
        ConcurrentArray<void *> m_creatorData;
        ConcurrentArray<void *> m_userData;
        ConcurrentArray<hash32> m_factoryData;
        ConcurrentArray<atomic_s32> m_references;
        ConcurrentArray<atomic_u8> m_flags;

        // Counters and state variables.
        atomic_u32 m_numObjects = 0;         // The number of active objects.
        atomic_u32 m_lastFreeSlot = 0;       // The index of the last freed object slot.
        atomic_u32 m_addReferenceCount = 0;  // The number of objects to add to the reference queue.
        atomic_u32 m_removeReferenceCount =
            0;                       // The number of objects to remove from the reference queue.
        atomic_u32 m_numRemove = 0;  // The number of objects to free.
        atomic_u32 m_size = 0;       // The total size of the object array.

        // Garbage collector mode and loading state.
        Atomic<GarbageCollectorMode> m_mode = GarbageCollectorMode::LowConsumption;
        Atomic<LoadingState> m_loadingState = LoadingState::Allocated;

        // Object type group and mutex for thread safety.
        TypeGroups m_typeGroup = TypeGroups::None;
        mutable RecursiveMutex m_mutex;
    };

}  // namespace fb

#endif  // GarbageCollectorData_h__
