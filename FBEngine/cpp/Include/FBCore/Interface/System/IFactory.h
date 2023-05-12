#ifndef _FBObjectFactory_H_
#define _FBObjectFactory_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
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

        /** Returns true if the object is derived from the given type. */
        template <class T>
        bool isObjectDerivedFrom();

        /** Returns the type name of the object this factory creates. */
        virtual String getTypeName() const = 0;

        /** Sets the type name of the object this factory creates. */
        virtual void setTypeName( const String &typeName ) = 0;

        /** Sets the size of the object's data. */
        virtual void setDataSize( u32 size ) = 0;

        /** Allocates memory for the object's data. */
        virtual void allocate_data() = 0;

        /** Allocates memory for the object. */
        virtual void *allocate() = 0;

        /** Deallocates memory for the object. */
        virtual void deallocate( void *ptr ) = 0;

        /** Creates an instance of the object this factory creates. */
        virtual SmartPtr<ISharedObject> createInstance() = 0;

        /** Creates an instance of the given type. */
        template <class T>
        SmartPtr<T> make_ptr();

        /** Creates an instance of the given type with the given arguments. */
        template <class T, class... _Types>
        SmartPtr<T> make_ptr( _Types &&..._Args );

        /** Creates a shared pointer instance of the given type. */
        template <class T>
        SharedPtr<T> make_shared();

        /** Creates an object of the given type. */
        template <class T>
        SmartPtr<T> createObjectByType();

        /** Destroys an instance of the object this factory creates. */
        virtual void destroyInstance( SmartPtr<ISharedObject> instance ) = 0;

        /** Creates an object instance. Returns the created object. */
        virtual void *createObject() = 0;

        /** Creates an object instance from a pool. Returns the created object. */
        virtual void *createObjectFromPool() = 0;

        /** Frees an object created by the pool. */
        virtual void freeObject( void *object ) = 0;

        /** Creates an array instance. Returns the created array. */
        virtual void *createArray( u32 numElements ) = 0;

        /** Returns the type name of the object this factory creates. */
        virtual String getObjectType() const = 0;

        /** Sets the type name of the object this factory creates. */
        virtual void setObjectType( const String &type ) = 0;

        /** Returns the type ID of the object this factory creates. */
        virtual hash64 getObjectTypeId() const = 0;

        /** Sets the type ID of the object this factory creates. */
        virtual void setObjectTypeId( hash64 id ) = 0;

        /** Gets the size of the object this factory creates. */
        virtual u32 getObjectSize() const = 0;

        /** Gets the amount of memory used. */
        virtual int getMemoryUsed() const = 0;

        /** Gets the instantiate objects. */
        virtual Array<SmartPtr<ISharedObject>> getInstanceObjects() const = 0;

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
#if 1
        auto ptr = static_cast<T *>( createObjectFromPool() );
        FB_ASSERT( ptr );

        if( !ptr )
        {
            FB_EXCEPTION( "Unable to allocate." );
        }

        SmartPtr<T> p( ptr );
        FB_ASSERT( p );

        ptr->removeReference();
        return p;
#else
        auto ptr = createInstance();
        FB_ASSERT( ptr );

        return fb::static_pointer_cast<T>( ptr );
#endif
    }

    template <class T, class... _Types>
    SmartPtr<T> IFactory::make_ptr( _Types &&..._Args )
    {
        auto ptr = FB_NEW T( std::forward<_Types>( _Args )... );
        auto p = SmartPtr<T>( ptr );
        ptr->removeReference();
        return p;
    }

    /** Creates an instance. */
    template <class T>
    SharedPtr<T> IFactory::make_shared()
    {
        return fb::make_shared<T>();
    }

    template <class T>
    SmartPtr<T> IFactory::createObjectByType()
    {
        auto instance = make_ptr<T>();
        if( !instance )
        {
            instance = createInstance();
            return fb::static_pointer_cast<T>( instance );
        }

        return instance;
    }

}  // end namespace fb

#endif
