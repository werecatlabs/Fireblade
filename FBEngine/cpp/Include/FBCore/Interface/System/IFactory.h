#ifndef _FBObjectFactory_H_
#define _FBObjectFactory_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{

    /** A factory to class to create objects.
     */
    class FBCore_API IFactory : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IFactory() override = default;

        /** Returns true if the object is derived from the given type info. */
        virtual bool isObjectDerivedFromByInfo( u32 typeInfo ) const = 0;

        /** Returns the type name of the object this factory creates. */
        virtual const c8 *getTypeName() const = 0;

        /** Sets the type name of the object this factory creates. */
        virtual void setTypeName( const String &typeName ) = 0;

        /** Gets the pool's grow size. */
        virtual u32 getGrowSize() const = 0;

        /** Sets the pool's grow size. */
        virtual void setGrowSize( u32 size ) = 0;

        /** Allocates memory for the pool data. */
        virtual void allocatePoolData() = 0;

        virtual void *allocateMemory() = 0;

        virtual void freeMemory( void *ptr ) = 0;

        /** Creates an object instance. Returns the created object. */
        virtual void *createObject() = 0;

        /** Frees an object created by the pool. */
        virtual void freeObject( void *object ) = 0;

        /** Creates an array instance. Returns the created array. */
        virtual void *createArray( u32 numElements ) = 0;

        /** Returns the type name of the object this factory creates. */
        virtual const c8 *getObjectType() const = 0;

        /** Sets the type name of the object this factory creates. */
        virtual void setObjectType( const String &type ) = 0;

        /** Returns the type ID of the object this factory creates. */
        virtual hash64 getObjectTypeId() const = 0;

        /** Sets the type ID of the object this factory creates. */
        virtual void setObjectTypeId( hash64 id ) = 0;

        /** Gets the size of the object this factory creates. */
        virtual u32 getObjectSize() const = 0;

        /** Sets the size of the object this factory creates. */
        virtual void setObjectSize( u32 objectSize ) = 0;

        /** Gets the amount of memory used. */
        virtual int getMemoryUsed() const = 0;

        virtual ISharedObjectListener *getListener() const = 0;

        /** Gets the instantiate objects. */
        virtual Array<SmartPtr<ISharedObject>> getInstanceObjects() const = 0;

        /** Creates an instance of the given type. */
        template <class T>
        SmartPtr<T> make_ptr();

        /** Creates an instance of the given type with the given arguments. */
        template <class T, class... _Types>
        SmartPtr<T> make_ptr( _Types &&..._Args );

        /** Creates a shared pointer instance of the given type. */
        template <class T>
        SharedPtr<T> make_shared();

        template <class T>
        SmartPtr<T> make_object();

        /** Returns true if the object is derived from the given type. */
        template <class T>
        bool isObjectDerivedFrom();

        FB_CLASS_REGISTER_DECL;
    };

    template <class T>
    bool IFactory::isObjectDerivedFrom()
    {
        auto typeInfo = T::typeInfo();
        return isObjectDerivedFromByInfo( typeInfo );
    }

    /** Creates an instance. */
    template <class T>
    SmartPtr<T> IFactory::make_ptr()
    {
        auto ptr = static_cast<T *>( createObject() );
        SmartPtr<T> p( ptr );
        ptr->removeReference();
        return p;
    }

    template <class T, class... _Types>
    SmartPtr<T> IFactory::make_ptr( _Types &&..._Args )
    {
        auto ptr = static_cast<T *>( allocateMemory() );
        if( !ptr )
        {
            FB_EXCEPTION( "Unable to allocate." );
        }

        auto object = new( ptr ) T( std::forward<_Types>( _Args )... );
        object->setSharedObjectListener( getListener() );

        SmartPtr<T> p( object );
        object->removeReference();
        return p;
    }

    /** Creates an instance. */
    template <class T>
    SharedPtr<T> IFactory::make_shared()
    {
        return fb::make_shared<T>();
    }

    template <class T>
    SmartPtr<T> IFactory::make_object()
    {
        auto object = static_cast<T *>( createObject() );
        FB_ASSERT( object );

        if( !object )
        {
            FB_EXCEPTION( "Unable to allocate." );
        }

        SmartPtr<T> p( object );
        object->removeReference();
        return p;
    }

}  // end namespace fb

#endif
