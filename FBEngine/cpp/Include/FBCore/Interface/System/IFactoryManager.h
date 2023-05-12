#ifndef IFactoryManager_h__
#define IFactoryManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/System/IFactory.h>

#ifdef _DEBUG
#    include <fstream>
#endif

namespace fb
{

    /** A factory to class to create objects.
     */
    class IFactoryManager : public ISharedObject
    {
    public:
        /** Destructor.
         */
        ~IFactoryManager() override = default;

        /** Adds factory to the manager. */
        virtual void addFactory( SmartPtr<IFactory> factory ) = 0;

        /** Removes a factory from the manager.
        @return Returns true if remove the factory is successful.
        */
        virtual bool removeFactory( SmartPtr<IFactory> factory ) = 0;

        /** Removes all the factories from the manager.
         */
        virtual void removeAllFactories() = 0;

        /** Gets a factory by id. */
        virtual const SmartPtr<IFactory> getFactory( hash64 id ) const = 0;

        /** Gets a factory by name. */
        virtual SmartPtr<IFactory> getFactoryByName( const String &name ) const = 0;

        /** Gets a factory by id. */
        virtual SmartPtr<IFactory> getFactoryById( hash64 id ) const = 0;

        /** Checks if the manager has a factory. */
        virtual bool hasFactory( const String &typeName ) const = 0;

        /** Checks if the manager has a factory using the name. */
        virtual bool hasFactoryByName( const String &name ) const = 0;

        /** Checks if the manager has a factory using the id. */
        virtual bool hasFactoryById( hash64 typeId ) const = 0;

        /** Gets an array of factories. */
        virtual Array<SmartPtr<IFactory>> getFactories() const = 0;

        /** Creates an object of the type passed.
        @return
            Returns the newly created object. Will return null if no object exists.
        */
        template <class T>
        SmartPtr<T> createObjectFromType( const String &type );

        /** Creates an object of the type passed.
        @return
            Returns the newly created object. Will return null if no object exists.
        */
        virtual void *_create( const String &type ) = 0;

        /** Creates an object of the type passed. */
        virtual SmartPtr<ISharedObject> createById( hash64 typeId ) const = 0;

        /** Creates an object of the type passed.
        @return
            Returns the newly created object. Will return null if no object exists.
        */
        template <class T>
        SmartPtr<T> createObjectById( hash64 typeId ) const;

        /** Creates an object of the type passed.
        @return
            Returns the newly created object. Will return null if no object exists.
        */
        virtual void *_createById( hash64 typeId ) = 0;

        /** Creates an object of the type passed.
        @return
            Returns the newly created object. Will return null if no object exists.
        */
        virtual void *_createFromPool( hash64 typeId ) = 0;

        /** Creates an object of the type passed. */
        virtual void *_createArrayById( hash64 typeId, u32 numElements, u32 &objectSize ) = 0;

        /** Creates an instance. */
        template <class T>
        SmartPtr<T> make_object();

        /** Creates an instance. */
        template <class T>
        SmartPtr<T> make_ptr();

        /** Creates an instance. */
        template <class T, class... _Types>
        SmartPtr<T> make_ptr( _Types &&..._Args );

        /** Creates an instance. */
        template <class T>
        SharedPtr<T> make_shared();

        /** Creates an instance. */
        template <class T, class... _Types>
        SharedPtr<T> make_shared( _Types &&..._Args );

        /** Sets the factory pool size. */
        virtual void setPoolSize( hash64 typeId, u32 size ) = 0;

        /** Sets the factory pool size. */
        template <class T>
        void setPoolSizeByType( u32 size );

        FB_CLASS_REGISTER_DECL;
    };

    template <class T>
    SmartPtr<T> IFactoryManager::make_object()
    {
        auto typeinfo = T::typeInfo();
        FB_ASSERT( typeinfo != 0 );

        auto p = createById( typeinfo );
        return fb::static_pointer_cast<T>( p );
    }

    template <class T>
    SmartPtr<T> IFactoryManager::make_ptr()
    {
        auto typeinfo = T::typeInfo();
        FB_ASSERT( typeinfo != 0 );

        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeHash = typeManager->getHash( typeinfo );
        FB_ASSERT( typeHash != 0 );

        auto factory = getFactoryById( typeHash );
        if( !factory )
        {
            auto typeName = typeManager->getName( typeinfo );
            FB_ASSERT( !typeName.empty() );

            factory = getFactoryByName( typeName );
        }

        if( factory )
        {
            auto p = factory->template make_ptr<T>();
            FB_ASSERT( fb::dynamic_pointer_cast<T>( p ) );
            return p;
        }

        return fb::make_ptr<T>();
    }

    template <class T, class... _Types>
    SmartPtr<T> IFactoryManager::make_ptr( _Types &&..._Args )
    {
        auto ptr = FB_NEW T( std::forward<_Types>( _Args )... );
        auto p = SmartPtr<T>( ptr );
        ptr->removeReference();
        return p;
    }

    template <class T>
    SharedPtr<T> IFactoryManager::make_shared()
    {
        return fb::make_shared<T>();
    }

    template <class T, class... _Types>
    SharedPtr<T> IFactoryManager::make_shared( _Types &&..._Args )
    {
        return fb::make_shared<T>( std::forward<_Types>( _Args )... );
    }

    template <class T>
    SmartPtr<T> IFactoryManager::createObjectFromType( const String &type )
    {
        auto factory = getFactoryByName( type );
        if( factory )
        {
            return factory->createObjectByType<T>();
        }

        return nullptr;
    }

    template <class T>
    void IFactoryManager::setPoolSizeByType( u32 size )
    {
        auto typeInfo = T::typeInfo();
        FB_ASSERT( typeInfo != 0 );

        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeHash = typeManager->getHash( typeInfo );
        FB_ASSERT( typeHash != 0 );
        setPoolSize( typeHash, size );
    }

}  // end namespace fb

#endif  // IFactoryManager_h__
