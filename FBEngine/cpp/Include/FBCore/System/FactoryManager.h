#ifndef _FBFactoryManager_H_
#define _FBFactoryManager_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IFactoryManager.h>

#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Interface/System/IFactory.h>
#include <FBCore/Thread/RecursiveMutex.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    /** A factory to class to create objects.
     */
    class FactoryManager : public IFactoryManager
    {
    public:
        /** Constructor.
         */
        FactoryManager();

        /** */
        FactoryManager( FactoryManager &other ) = delete;

        /** Destructor.
         */
        ~FactoryManager() override;

        /** @copydoc IObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IFactoryManager::addFactory */
        void addFactory( SmartPtr<IFactory> factory ) override;

        /** @copydoc IFactoryManager::removeFactory */
        bool removeFactory( SmartPtr<IFactory> factory ) override;

        /** @copydoc IFactoryManager::removeAllFactories */
        void removeAllFactories() override;

        /** @copydoc IFactoryManager::getFactory */
        const SmartPtr<IFactory> getFactory( hash64 id ) const override;

        /** @copydoc IFactoryManager::getFactoryByName */
        SmartPtr<IFactory> getFactoryByName( const String &name ) const override;

        /** @copydoc IFactoryManager::getFactoryById */
        SmartPtr<IFactory> getFactoryById( hash64 id ) const override;

        /** @copydoc IFactoryManager::hasFactory */
        bool hasFactory( const String &typeName ) const override;

        /** @copydoc IFactoryManager::hasFactoryByName */
        bool hasFactoryByName( const String &typeName ) const override;

        /** @copydoc IFactoryManager::hasFactoryById */
        bool hasFactoryById( hash64 typeId ) const override;

        /** @copydoc IFactoryManager::getFactories */
        Array<SmartPtr<IFactory>> getFactories() const override;

        /** @copydoc IFactoryManager::_create */
        SmartPtr<ISharedObject> createById( hash64 typeId ) const;

        /** @copydoc IFactoryManager::_create */
        void *_create( const String &type ) override;

        /** @copydoc IFactoryManager::_createById */
        void *_createById( hash64 typeId ) override;

        /** @copydoc IFactoryManager::_createFromPool */
        void *_createFromPool( hash64 typeId ) override;

        /** @copydoc IFactoryManager::_createArrayById */
        void *_createArrayById( hash64 typeId, u32 numElements, u32 &objectSize ) override;

        /** @copydoc IFactoryManager::setPoolSize */
        void setPoolSize( hash64 typeId, size_t size ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        s32 getFactoryUnloadPriority( SmartPtr<IFactory> factory );

        SharedPtr<ConcurrentArray<SmartPtr<IFactory>>> getFactoriesArray() const;
        void setFactoriesArray( SharedPtr<ConcurrentArray<SmartPtr<IFactory>>> ptr );

        AtomicSharedPtr<ConcurrentArray<SmartPtr<IFactory>>> m_factories;
        bool m_autoCreateFactory = false;
        bool m_bEnableDebug = false;
    };
}  // end namespace fb

#endif
