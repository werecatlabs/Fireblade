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

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

#if FB_ENABLE_MEMORY_TRACKER
#    include <FBCore/Memory/MemoryTracker.h>
#endif

namespace fb
{

    /** Implementation for a shared object.
    @author	Zane Desir
    @version 1.0
    */
    template <typename T>
    class CSharedObject : public T
    {
    public:
        typedef T type;

        /** Default constructor.
         */
        CSharedObject();

        /** Copy constructor.
         */
        CSharedObject( CSharedObject<T> &other ) = delete;

        /** Virtual destructor.
         */
        virtual ~CSharedObject() override;

        /** @copydoc ISharedObject::preUpdate. */
        virtual void preUpdate() override;

        /** @copydoc ISharedObject::update. */
        virtual void update() override;

        /** @copydoc ISharedObject::postUpdate. */
        virtual void postUpdate() override;

        /** @copydoc ISharedObject::getHandle. */
        Handle *getHandle() const override;

        /** @copydoc ISharedObject::addWeakReference. */
        s32 addWeakReference() override;

        /** @copydoc ISharedObject::addWeakReference. */
        s32 addWeakReference( void *address, const c8 *file, u32 line, const c8 *func ) override;

        /** @copydoc ISharedObject::removeWeakReference. */
        bool removeWeakReference() override;

        /** @copydoc ISharedObject::removeWeakReference. */
        bool removeWeakReference( void *address, const c8 *file = "??", u32 line = 0,
                                  const c8 *func = "??" ) override;

        /** @copydoc ISharedObject::addReference */
        s32 addReference( void *address, const c8 *file, u32 line, const c8 *func ) override;

        /** @copydoc ISharedObject::addReference */
        s32 addReference() override;

        /** @copydoc ISharedObject::removeReference */
        bool removeReference( void *address, const c8 *file = "??", u32 line = 0,
                              const c8 *func = "??" ) override;

        /** @copydoc ISharedObject::removeReference */
        bool removeReference() override;

        /** @copydoc ISharedObject::getReferences */
        s32 getReferences() const override;

        /** @copydoc ISharedObject::getReferences */
        s32 getWeakReferences() const override;

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

        /** @copydoc ISharedObject::isLoading */
        virtual bool isLoading() const override;

        /** @copydoc ISharedObject::isLoadingQueued */
        virtual bool isLoadingQueued() const override;

        /** @copydoc ISharedObject::isLoaded */
        virtual bool isLoaded() const override;

        /** @copydoc ISharedObject::isThreadSafe */
        virtual bool isThreadSafe() const override;

        /** @copydoc ISharedObject::isAlive */
        bool isAlive() const override;

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
        SmartPtr<IEventListener> findObjectListener( const String &id ) const
        {
            return nullptr;
        }

        ISharedObjectListener *getSharedObjectListener() const
        {
            return m_sharedObjectListener.load();
        }

        void setSharedObjectListener( ISharedObjectListener *listener )
        {
            m_sharedObjectListener = listener;
        }

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

        void setupGarbageCollectorData();

        virtual void lock();

        virtual void unlock();

#if FB_USE_CUSTOM_NEW_DELETE
        void *operator new( size_t sz );
        void *operator new( size_t sz, void *ptr );
        void *operator new[]( size_t sz );
        void operator delete( void *ptr );
        void operator delete( void *ptr, void * );
        void operator delete[]( void *ptr );

        void *operator new( size_t sz, const char *file, int line, const char *func );
        void *operator new( size_t sz, void *ptr, const char *file, int line, const char *func );
        void *operator new[]( size_t sz, const char *file, int line, const char *func );
        void operator delete( void *reportedAddress, const char *file, int line,
                              const char *func ) throw();
        void operator delete( void *ptr, void *, const char *file, int line, const char *func );
        void operator delete[]( void *reportedAddress, const char *file, int line,
                                const char *func ) throw();
#endif

        FB_CLASS_REGISTER_TEMPLATE_DECL( CSharedObject, T );

    protected:
        SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> getObjectListenersPtr() const;

        void setObjectListenersPtr( SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p );

        SmartPtr<ISharedObject> m_scriptData;
        atomic_u32 m_objectId = std::numeric_limits<u32>::max();
        atomic_s32 m_weakReferences = 0;

        AtomicRawPtr<ISharedObjectListener> m_sharedObjectListener;
        SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> m_sharedEventListeners;

        atomic_s32 *m_references = nullptr;
        atomic_u8 *m_flags = nullptr;
        Handle *m_handle = nullptr;
        Atomic<LoadingState> *m_loadingState = nullptr;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CSharedObject, T, T );

    template <typename T>
    void CSharedObject<T>::setupGarbageCollectorData()
    {
        if( m_objectId != std::numeric_limits<u32>::max() )
        {
            auto &gc = GarbageCollector::instance();

            auto typeInfo = getTypeInfo();
            m_references = gc.getReferencesPtr( typeInfo, m_objectId );
            m_flags = gc.getFlagPtr( typeInfo, m_objectId );
            m_handle = gc.getHandle( typeInfo, m_objectId );
            m_loadingState = gc.getLoadingStatePtr( typeInfo, m_objectId );
        }
    }

    template <typename T>
    void CSharedObject<T>::lock()
    {
    }

    template <typename T>
    void CSharedObject<T>::unlock()
    {
    }

    template <typename T>
    SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> CSharedObject<T>::getObjectListenersPtr() const
    {
        return m_sharedEventListeners;
    }

    template <typename T>
    void CSharedObject<T>::setObjectListenersPtr(
        SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p )
    {
        m_sharedEventListeners = p;
    }

}  // end namespace fb

#include <FBCore/Memory/CSharedObject.inl>

#endif  // __CSharedObject_H_
