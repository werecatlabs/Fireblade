#ifndef Factory_h__
#define Factory_h__

#include <FBCore/Interface/System/IFactory.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{
    class Factory : public IFactory
    {
    public:
        Factory();
        ~Factory() override;

        void load( SmartPtr<ISharedObject> data ) override;

        void unload( SmartPtr<ISharedObject> data ) override;

        u32 getGrowSize() const override;

        void setGrowSize( u32 size ) override;

        void allocatePoolData() override;

        void *allocateMemory() override;

        void freeMemory( void *ptr ) override;

        void *createObject() override;

        void freeObject( void *object ) override;

        u32 getObjectSize() const override;

        void setObjectSize( u32 objectSize ) override;

        ISharedObjectListener *getListener() const override;

        bool isObjectDerivedFromByInfo( u32 typeInfo ) const override;

        const c8 *getTypeName() const override;

        void setTypeName( const String &typeName ) override;

        void *createArray( u32 numElements ) override;

        const c8 *getObjectType() const override;

        void setObjectType( const String &type ) override;

        hash64 getObjectTypeId() const override;

        void setObjectTypeId( hash64 id ) override;

        int getMemoryUsed() const override;

        Array<SmartPtr<ISharedObject>> getInstanceObjects() const override;

    protected:
        class Listener : public ISharedObjectListener
        {
        public:
            Listener();

            explicit Listener( Factory *factory );

            ~Listener() override;

            bool destroy( void *ptr ) override;

            Factory *getFactory() const;

            void setFactory( Factory *factory );

        protected:
            Factory *m_factory = nullptr;
        };

        ISharedObjectListener *m_listener = nullptr;

        /// Instances
        AtomicSharedPtr<ConcurrentArray<void *>> m_instances;
        mutable RecursiveMutex m_instancesMutex;

        /// The type of object this factory creates
        FixedString<128> m_objectType;

        /// The name of the object type.
        FixedString<128> m_typeName;

        /// The id of the type of object this factory creates
        hash64 m_objectTypeId = 0;

        // The number of instances created
        atomic_u32 m_instanceCount = 0;

        // The grow size for the pool
        atomic_u32 m_nextSize = 0;

        // The size of the object.
        u32 m_objectSize = 0;
    };
}  // namespace fb

#endif  // Factory_h__
