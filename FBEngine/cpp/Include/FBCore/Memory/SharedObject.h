#ifndef __CSharedObject_H_
#define __CSharedObject_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Interface/Memory/IData.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Memory/ObjectTracker.h>
#include <FBCore/Memory/GarbageCollector.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/IApplicationManager.h>

#include <sstream>

namespace fb
{

    /** Implementation for a shared object.
    @author	Zane Desir
    @version 1.0
    */
    template <typename T>
    class SharedObject : public T
    {
    public:
        typedef T type;

        /** Default constructor.
         */
        SharedObject();

        /** Copy constructor.
         */
        SharedObject( SharedObject<T> &other ) = delete;

        /** Virtual destructor.
         */
        virtual ~SharedObject() override;

        /** @copydoc ISharedObject::preUpdate. */
        virtual void preUpdate() override;

        /** @copydoc ISharedObject::update. */
        virtual void update() override;

        /** @copydoc ISharedObject::postUpdate. */
        virtual void postUpdate() override;

        /** @copydoc ISharedObject::getHandle. */
        Handle *getHandle() const override;

        /** @copydoc ISharedObject::isValid */
        virtual bool isValid() const override;

        /** @copydoc ISharedObject::load */
        virtual void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::reload */
        virtual void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        virtual void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::getLoadingState */
        virtual const Atomic<LoadingState> &getLoadingState() const override;

        /** @copydoc ISharedObject::setLoadingState */
        virtual void setLoadingState( const Atomic<LoadingState> &state ) override;

        virtual void setPoolElement( bool poolElement );
        virtual bool isPoolElement() const;

        /** @copydoc ISharedObject::getCreatorData */
        void *getCreatorData() const override;

        /** @copydoc ISharedObject::setCreatorData */
        void setCreatorData( void *val ) override;

        /** @copydoc ISharedObject::getFactoryData */
        hash32 getFactoryData() const override;

        /** @copydoc ISharedObject::setFactoryData */
        void setFactoryData( hash32 factoryData ) override;

        /** @copydoc ISharedObject::toJson */
        String toJson() const;

        /** @copydoc ISharedObject::toXml */
        String toXml() const;

        /** @copydoc ISharedObject::toString */
        virtual String toString() const;

        /** @copydoc ISharedObject::toData */
        virtual SmartPtr<ISharedObject> toData() const;

        /** @copydoc ISharedObject::fromData */
        virtual void fromData( SmartPtr<ISharedObject> data );

        /** @copydoc ISharedObject::getChildObjects */
        virtual Array<SmartPtr<ISharedObject>> getChildObjects() const override;

        /** @copydoc ISharedObject::getUserData */
        virtual void *getUserData() const override;

        /** @copydoc ISharedObject::setUserData */
        virtual void setUserData( void *userData ) override;

        /** @copydoc ISharedObject::getUserData */
        virtual void *getUserData( hash32 id ) const override;

        /** @copydoc ISharedObject::setUserData */
        virtual void setUserData( hash32 id, void *userData ) override;

        /** @copydoc ISharedObject::getObjectListener */
        Array<SmartPtr<IEventListener>> getObjectListeners() const override;

        /** @copydoc ISharedObject::getNumListeners */
        u32 getNumListeners() const override;

        /** @copydoc ISharedObject::hasObjectListener */
        bool hasObjectListener( SmartPtr<IEventListener> listener ) const override;

        /** @copydoc ISharedObject::addObjectListener */
        void addObjectListener( SmartPtr<IEventListener> listener ) override;

        /** @copydoc ISharedObject::removeObjectListener */
        void removeObjectListener( SmartPtr<IEventListener> listener ) override;

        /** @copydoc ISharedObject::findElementListener */
        SmartPtr<IEventListener> findObjectListener( const String &id ) const;

        /** @copydoc ISharedObject::getSharedObjectListener */
        ISharedObjectListener *getSharedObjectListener() const;

        /** @copydoc ISharedObject::setSharedObjectListener */
        void setSharedObjectListener( ISharedObjectListener *listener );

        /** @copydoc ISharedObject::getEnableReferenceTracking */
        bool getEnableReferenceTracking() const override;

        /** @copydoc ISharedObject::setEnableReferenceTracking */
        void setEnableReferenceTracking( bool referenceTracking ) override;

        /** @copydoc ISharedObject::isGarbageCollected */
        bool isGarbageCollected() const override;

        /** @copydoc ISharedObject::setGarbageCollected */
        void setGarbageCollected( bool garbageCollected ) override;

        /** @copydoc ISharedObject::getObjectId */
        u32 getObjectId() const override;

        SmartPtr<ISharedObject> getScriptData() const;

        void setScriptData( SmartPtr<ISharedObject> data );

        virtual void lock();

        virtual void unlock();

        FB_CLASS_REGISTER_TEMPLATE_DECL( SharedObject, T );

    protected:
        SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> getObjectListenersPtr() const;

        void setObjectListenersPtr( SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p );
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, SharedObject, T, T );

    template <typename T>
    SmartPtr<IEventListener> SharedObject<T>::findObjectListener( const String &id ) const
    {
        return nullptr;
    }

    template <typename T>
    ISharedObjectListener *SharedObject<T>::getSharedObjectListener() const
    {
        return T::m_sharedObjectListener.load();
    }

    template <typename T>
    void SharedObject<T>::setSharedObjectListener( ISharedObjectListener *listener )
    {
        T::m_sharedObjectListener = listener;
    }

    template <typename T>
    void SharedObject<T>::lock()
    {
    }

    template <typename T>
    void SharedObject<T>::unlock()
    {
    }

    template <typename T>
    SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> SharedObject<T>::getObjectListenersPtr() const
    {
        return T::m_sharedEventListeners;
    }

    template <typename T>
    void SharedObject<T>::setObjectListenersPtr( SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p )
    {
        T::m_sharedEventListeners = p;
    }

}  // end namespace fb

#include <FBCore/Memory/SharedObject.inl>

#endif  // __CSharedObject_H_
