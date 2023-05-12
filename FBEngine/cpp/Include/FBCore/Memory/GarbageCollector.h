#ifndef __GarbageCollector_h__
#define __GarbageCollector_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/FixedArray.h>
#include <FBCore/Base/Handle.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/ConcurrentQueue.h>
#include <FBCore/Base/UtilityTypes.h>
#include <FBCore/Thread/RecursiveMutex.h>
#include <FBCore/Memory/GarbageCollectorData.h>

namespace fb
{

    /**
     * The GarbageCollector class is a singleton that manages objects as part of the engine's object system.
     * It provides a cache-friendly mechanism to manage objects in memory, and helps to combat the previous random access
     * with the per-object reference counting system.
     *
     * This class is a key part of the engine's object system, and is designed to be called at regular intervals to
     * manage objects in memory. It provides methods to load, update, and unload garbage collector data, as well as
     * to add and remove objects for garbage collection.
     *
     * The class maintains several data structures to store object-related information, including object listeners, handles,
     * user data, creator data, factory data, and reference counts. It provides methods to retrieve or set this data, as well
     * as to add or remove object references. The GarbageCollector class also provides functionality to set and get an object's
     * flag, and to destroy an object.
     *
     * @author    Zane Desir
     * @version   1.0
     */
    class GarbageCollector
    {
    public:
        /**
         * Constructor. Allocates necessary data.
         */
        GarbageCollector();

        /**
         * Destructor. Destroys necessary data. Ensure this is called last.
         */
        ~GarbageCollector();

        /**
         * Loads all the garbage collector data.
         */
        void load();

        /**
         * Unloads all the garbage collector data.
         */
        void unload();

        /**
         * Updates the garbage collector. Should be called at regular intervals.
         */
        void update();

        /**
         * Adds an object for garbage collection.
         *
         * @param object The object for garbage collection.
         * @return The id of the object.
         */
        u32 addObject( ISharedObject *object );

        /**
         * Removes an object for garbage collection.
         *
         * @param typeInfo The type of the object to be removed.
         * @param id The id of the object to be removed.
         */
        void removeObject( u32 typeInfo, u32 id );

        /**
         * Gets an object's loading state.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @return The atomic reference to the object's loading state.
         */
        const Atomic<LoadingState> &getLoadingState( u32 typeInfo, u32 id ) const;

        /**
         * Gets an object's loading state.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @return The atomic pointer to the object's loading state.
         */
        Atomic<LoadingState> *getLoadingStatePtr( u32 typeInfo, u32 id ) const;

        /**
         * Sets an object's loading state.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @param state Atomic reference to the object's loading state to be set.
         */
        void setLoadingState( u32 typeInfo, u32 id, const Atomic<LoadingState> &state );

        /**
         * Gets an object's listener.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @return Pointer to the object's listener.
         */
        ISharedObjectListener *getObjectListener( u32 typeInfo, u32 id ) const;

        /**
         * Sets an object's listener.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @param listener Pointer to the object's listener to be set.
         */
        void setObjectListener( u32 typeInfo, u32 id, ISharedObjectListener *listener );

        /**
         * Gets an object's handle.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @return Pointer to the object's handle.
         */
        Handle *getHandle( u32 typeInfo, u32 id ) const;

        /**
         * Gets an object's user data.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @return Pointer to the object's user data.
         */
        void *getUserData( u32 typeInfo, u32 id ) const;

        /**
         * Sets an object's user data.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @param userData Pointer to the object's user data to be set.
         */
        void setUserData( u32 typeInfo, u32 id, void *userData );

        /**
         * Sets an object's creator data.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @param userData Pointer to the object's creator data to be set.
         */
        void setCreatorData( u32 typeInfo, u32 id, void *userData );

        /**
         * Gets an object's creator data.
         *
         * @param typeInfo Type information of the object.
         * @param id ID of the object.
         * @return Pointer to the object's creator data.
         */
        void *getCreatorData( u32 typeInfo, u32 id ) const;

        /**
         * Gets an object's factory data.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @return The factory data of the object.
         */
        hash32 getFactoryData( u32 typeInfo, u32 id ) const;

        /**
         * Sets an object's factory data.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @param factoryData The factory data to be set for the object.
         */
        void setFactoryData( u32 typeInfo, u32 id, hash32 factoryData );

        /**
         * Gets an object's reference count.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @return The reference count of the object.
         */
        s32 getReferences( u32 typeInfo, u32 id ) const;

        /**
         * Sets an object's reference count.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @param references The reference count to be set for the object.
         */
        void setReferences( u32 typeInfo, u32 id, s32 references );

        /**
         * Adds an object reference.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @return The new reference count of the object after adding the reference.
         */
        s32 addReference( u32 typeInfo, u32 id );

        /**
         * Removes an object reference.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @return The new reference count of the object after removing the reference.
         */
        s32 removeReference( u32 typeInfo, u32 id );

        /**
         * Destroys the object with the given type info and ID.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         */
        void destroyObject( u32 typeInfo, u32 id );

        /**
         * Sets an object's flag.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @param flag The flag to be set.
         * @param value The value to set for the flag.
         */
        void setFlag( u32 typeInfo, u32 id, u8 flag, bool value );

        /**
         * Gets an object's flag.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @param flag The flag to get.
         * @return The value of the flag.
         */
        bool getFlag( u32 typeInfo, u32 id, u8 flag ) const;

        /**
         * Gets the pointer to the atomic flag for the object with the given type info and ID.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @return The pointer to the atomic flag of the object.
         */
        atomic_u8 *getFlagPtr( u32 typeInfo, u32 id ) const;

        /**
         * Gets all objects.
         *
         * @return An array of all objects.
         */
        Array<ISharedObject *> getObjects() const;

        /**
         * Gets the pointer to the object with the given type info and ID.
         *
         * @param typeInfo The type info of the object.
         * @param id The ID of the object.
         * @return The pointer to the object.
         */
        ISharedObject **getObjectPtr( u32 typeInfo, u32 id ) const;

        /**
         * Gets an object pointer.
         * @param typeInfo The type of the object.
         * @param id The id of the object.
         * @return A pointer to the object.
         */
        ISharedObject *getObject( u32 typeInfo, u32 id ) const;

        /**
         * Sets an object pointer.
         * @param typeInfo The type of the object.
         * @param id The id of the object.
         * @param object The object to be set.
         */
        void setObject( u32 typeInfo, u32 id, ISharedObject *object );

        /**
         * Gets a pointer to the reference count of the object.
         * @param typeInfo The type of the object.
         * @param id The id of the object.
         * @return A pointer to the reference count.
         */
        atomic_s32 *getReferencesPtr( u32 typeInfo, u32 id );

        /**
         * Gets the garbage collector mode.
         * @return The garbage collector mode.
         */
        GarbageCollectorMode getMode() const;

        /**
         * Sets the garbage collector mode.
         * @param mode The new garbage collector mode.
         */
        void setMode( GarbageCollectorMode mode );

        /**
         * Checks if the garbage collector is valid.
         * @return Whether the garbage collector is valid.
         */
        bool isValid() const;

        /**
         * Gets the garbage collector instance.
         * @return A reference to the garbage collector instance.
         */
        static GarbageCollector &instance();

    protected:
        /**
         * Gets the GarbageCollectorData for the given type.
         * @param typeInfo The type identifier.
         * @return The GarbageCollectorData.
         */
        GarbageCollectorData &getObjectData( u32 typeInfo );

        /**
         * Gets the GarbageCollectorData for the given type.
         * @param typeInfo The type identifier.
         * @return The GarbageCollectorData.
         */
        const GarbageCollectorData &getObjectData( u32 typeInfo ) const;

        /**
         * Updates the object references.
         */
        void updateReferences();

        /**
         * Queues the object to be freed.
         * @param typeInfo The type identifier.
         * @param id The object identifier.
         */
        void queueFreedObject( u32 typeInfo, u32 id );

        /**
         * The queue to add references.
         */
        ConcurrentQueue<Pair<u32, u32>> m_addReference;

        /**
         * The queue to remove references.
         */
        ConcurrentQueue<Pair<u32, u32>> m_removeReference;

        /**
         * The array of GarbageCollectorData.
         */
        FixedArray<GarbageCollectorData, (u32)TypeGroups::Count> m_objectData;

        /**
         * The GarbageCollectorMode atomic variable.
         */
        Atomic<GarbageCollectorMode> m_mode = GarbageCollectorMode::LowConsumption;

        /**
         * The GarbageCollector instance.
         */
        static GarbageCollector m_sGarbageCollector;
    };

    inline GarbageCollector &GarbageCollector::instance()
    {
        return m_sGarbageCollector;
    }

}  // end namespace fb

#endif  // GarbageCollector_h__
