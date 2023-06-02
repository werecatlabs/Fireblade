#ifndef TypeManager_h__
#define TypeManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Thread/SpinRWMutex.h>

namespace fb
{
    /**
     * @brief Manages object type information.
     */
    class TypeManager
    {
    public:
        /**
         * @brief Default constructor.
         */
        TypeManager();

        /**
         * @brief Destructor.
         */
        ~TypeManager();

        /**
         * @brief Get the name of the type with the given id.
         * @param id The id of the type to retrieve the name for.
         * @return Returns the name of the type as a String.
         */
        String getName( u32 id ) const;

        /**
         * @brief Get the hash value of the type with the given id.
         * @param id The id of the type to retrieve the hash for.
         * @return Returns the hash of the type as a 64-bit value.
         */
        hash64 getHash( u32 id ) const;

        /**
         * @brief Get the base type of the type with the given id.
         * @param id The id of the type to retrieve the base type for.
         * @return Returns the base type as a 32-bit unsigned integer.
         */
        u32 getBaseType( u32 id ) const;

        /**
         * @brief Check if two types are exactly the same.
         * @param a The id of the first type.
         * @param b The id of the second type.
         * @return Returns true if the types are exactly the same, false otherwise.
         */
        bool isExactly( u32 a, u32 b ) const;

        /**
         * @brief Check if one type is derived from another.
         * @param a The id of the first type.
         * @param b The id of the second type.
         * @return Returns true if the first type is derived from the second, false otherwise.
         */
        bool isDerived( u32 a, u32 b ) const;

        /**
         * @brief Get the class hierarchy of the type with the given id.
         * @param id The id of the type to retrieve the class hierarchy for.
         * @return Returns an array of Strings representing the class hierarchy.
         */
        Array<String> getClassHierarchy( u32 id ) const;

        /**
         * @brief Get the class hierarchy of the type with the given id.
         * @param id The id of the type to retrieve the class hierarchy for.
         * @return Returns an array of u32 representing the class hierarchy.
         */
        Array<u32> getClassHierarchyId( u32 id ) const;

        /**
         * @brief Get the type index of the type with the given id.
         * @param id The id of the type to retrieve the type index for.
         * @return Returns the type index as a 32-bit unsigned integer.
         */
        u32 getTypeIndex( u32 id ) const;

        /**
         * @brief Get the number of instances of the type with the given id.
         * @param id The id of the type to retrieve the number of instances for.
         * @return Returns the number of instances as a 32-bit unsigned integer.
         */
        u32 getNumInstances( u32 id ) const;

        /**
         * @brief Get the total number of types.
         * @return Returns the total number of types as a 32-bit unsigned integer.
         */
        u32 getTotalNumTypes();

        /**
         * @brief Get the data type of the type with the given id.
         * @param id The id of the type to retrieve the data type for.
         * @return Returns the data type as a 32-bit unsigned integer.
         */
        u32 getDataType( u32 id ) const;

        /**
         * @brief Set the data type of the type with the given id.
         * @param id The id of the type to set the data type for.
         * @param dataType The new data type as a 32-bit unsigned integer.
         */
        void setDataType( u32 id, u32 dataType );

        /**
         * @brief Set the data type of the type with the given id using an enumeration value.
         * @tparam B The base type of the class.
         * @tparam T The enumeration type.
         * @param dataType The enumeration value representing the data type.
         */
        template <class B, class T>
        void setDataTypeEnum( T dataType );

        /**
         * @brief Get the data type of the type with the given id as an enumeration value.
         * @tparam T The enumeration type.
         * @param id The id of the type to retrieve the data type for.
         * @return Returns the data type as an enumeration value.
         */
        template <class T>
        T getDataTypeEnum( u32 id ) const;

        /**
         * @brief Get a new unique type id.
         * @return Returns a new unique type id as a 32-bit unsigned integer.
         */
        u32 getNewTypeId();

        /**
         * @brief Get a new unique type id and associate it with the given name and base type.
         * @param name The name of the new type.
         * @param baseType The id of the base type.
         * @return Returns a new unique type id as a 32-bit unsigned integer.
         */
        u32 getNewTypeId( const String &name, u32 baseType );

        /**
         * @brief Get all the base types of the given type.
         * @param type The id of the type to get base types for.
         * @return Returns an array of base type ids.
         */
        Array<u32> getBaseTypes( u32 type ) const;

        /**
         * @brief Get the names of all the base types of the given type.
         * @param type The id of the type to get base type names for.
         * @return Returns an array of base type names.
         */
        Array<String> getBaseTypeNames( u32 type ) const;

        /**
         * @brief Get all the types derived from the given type.
         * @param type The id of the type to get derived types for.
         * @return Returns an array of derived type ids.
         */
        Array<u32> getDerivedTypes( u32 type ) const;

        /**
         * @brief Get the type group of the type with the given id.
         * @param id The id of the type to retrieve the type group for.
         * @return Returns the type group as a 32-bit unsigned integer.
         */
        u32 getTypeGroup( u32 id ) const;

        /**
         * @brief Get the singleton instance of the TypeManager.
         * @return Returns a pointer to the singleton instance of the TypeManager.
         */
        static TypeManager *instance();

    private:
        /**
         * @brief Get the current size of the TypeManager.
         * @return Returns the size as a 32-bit unsigned integer.
         */
        u32 getSize() const;

        /**
         * @brief Reserve space for the given size in the TypeManager.
         * @param size The size to reserve as a 32-bit unsigned integer.
         */
        void reserve( u32 size );

        /**
         * @brief Calculate the group index for the given type info.
         * @param typeInfo The id of the type to calculate the group index for.
         * @return Returns the group index as a 32-bit unsigned integer.
         */
        u32 calculateGroupIndex( u32 typeInfo ) const;

        /// Stores the names of the types, indexed by their ids.
        Array<String> m_name;

        /// Stores the hash values of the types, indexed by their ids.
        Array<atomic_u64> m_hash;

        /// Stores the type indices, indexed by their ids.
        Array<atomic_u32> m_typeIndex;

        /// Stores the type groups, indexed by their ids. Mutable because it can be updated by const methods.
        mutable Array<atomic_u32> m_typeGroup;

        /// Stores the base types, indexed by their ids.
        Array<atomic_u32> m_baseType;

        /// Reserved for the data-orientated systems. Stores custom data types, indexed by their ids.
        Array<atomic_u32> m_dataTypes;

        /// Stores the number of instances for each type, indexed by their ids.
        Array<atomic_u32> m_numInstances;

        /// Represents the current type count.
        atomic_u32 m_typeCount = 0;

        /// Represents the current size of the TypeManager.
        atomic_u32 m_size = 0;

        /// A mutable SpinRWMutex used to protect the access to the m_name member variable.
        mutable SpinRWMutex m_nameMutex;
    };

    template <class B, class T>
    void TypeManager::setDataTypeEnum( T dataType )
    {
        auto iTypeInfo = B::typeInfo();
        setDataType( iTypeInfo, static_cast<u32>( dataType ) );
    }

    template <class T>
    T TypeManager::getDataTypeEnum( u32 id ) const
    {
        return static_cast<T>( getDataType( id ) );
    }

}  // end namespace fb

#endif  // TypeManager_h__
