#ifndef Handle_h__
#define Handle_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{

    /**
     * Used to reference objects.
     */
    class Handle
    {
    public:
        /** Constructs a new Handle with default values. */
        Handle();

        /** Constructs a new Handle by copying another handle.
         *  @param other: The handle to be copied.
         */
        Handle( const Handle &other );

        /** Constructs a new Handle with the given name.
         *  @param name: The name of the Handle.
         */
        explicit Handle( const String &name );

        /** Destroys this handle. */
        ~Handle();

        /** Returns the hash value of the handle name.
         *  @return The hash value of the handle name.
         */
        hash64 getHash() const;

        /** Sets the hash value of the handle name.
         *  @param val: The hash value to set.
         */
        void setHash( hash64 hash );

        /** Returns the name of the handle.
         *  @return The name of the handle.
         */
        String getName() const;

        /** Sets the name of the handle.
         *  @param name: The name to set.
         */
        void setName( const String &name );

        /** Returns the UUID of the handle.
         *  @return The UUID of the handle.
         */
        String getUUID() const;

        /** Sets the UUID of the handle.
         *  @param uuid: The UUID to set.
         */
        void setUUID( const String &uuid );

        /** Returns the ID of the handle.
         *  @return The ID of the handle.
         */
        u32 getId() const;

        /** Sets the ID of the handle.
         *  @param val: The ID to set.
         */
        void setId( u32 id );

        /** Returns the instance ID of the handle.
         *  @return The instance ID of the handle.
         */
        u32 getInstanceId() const;

        /** Sets the instance ID of the handle.
         *  @param instanceId: The instance ID to set.
         */
        void setInstanceId( u32 instanceId );

        /** Checks whether two handles are equal.
         *  @param b: The handle to compare with.
         *  @return True if the handles are equal, false otherwise.
         */
        bool operator==( const Handle &b );

        /** Assigns the value of another handle to this handle.
         *  @param other: The handle to be assigned.
         *  @return A reference to this handle.
         */
        Handle &operator=( const Handle &other );

        /** Returns a string representation of the handle.
         *  @return A string representation of the handle.
         */
        String toString() const;

        String getClassId() const;

        void setClassId( const String &classId );

        String getFileId() const;

        void setFileId( const String &fileId );

    private:
        /** The ID of the handle. */
        atomic_u32 m_id = std::numeric_limits<u32>::max();

        /** The ID of the handle that is incremented based on the object type. */
        atomic_u32 m_instanceId = std::numeric_limits<u32>::max();

        /** The hash value of the handle name. */
        hash64 m_hash = std::numeric_limits<hash64>::max();

        /** The name of the handle. */
        String m_name;

        /** The UUID of the handle. */
        String m_uuid;

        String m_classId;
        String m_fileId;

        /** The extension to use for autogenerated names. */
        static u32 m_nameExt;
    };

    inline u32 Handle::getInstanceId() const
    {
        return m_instanceId;
    }

}  // end namespace fb

#endif  // Handle_h__
