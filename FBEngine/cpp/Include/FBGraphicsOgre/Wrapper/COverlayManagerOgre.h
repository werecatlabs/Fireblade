#ifndef _COverlayManager_H
#define _COverlayManager_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IOverlayManager.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace render
    {

        /** Implements IOverlayManager interface for Ogre. */
        class COverlayManagerOgre : public CSharedObject<IOverlayManager>
        {
        public:
            /** Constructor. */
            COverlayManagerOgre();

            /** Destructor. */
            ~COverlayManagerOgre() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IOverlayManager::addOverlay */
            SmartPtr<IOverlay> addOverlay( const String &instanceName ) override;

            /** @copydoc IOverlayManager::removeOverlay */
            bool removeOverlay( const String &instanceName ) override;

            /** @copydoc IOverlayManager::removeOverlay */
            bool removeOverlay( SmartPtr<IOverlay> overlay ) override;

            /** @copydoc IOverlayManager::findOverlay */
            SmartPtr<IOverlay> findOverlay( const String &instanceName ) override;

            /** @copydoc IOverlayManager::hasOverlay */
            bool hasOverlay( const String &instanceName ) override;

            /** @copydoc IOverlayManager::addElement */
            SmartPtr<IOverlayElement> addElement( const String &typeName,
                                                  const String &instanceName ) override;

            /** @copydoc IOverlayManager::findElement */
            SmartPtr<IOverlayElement> findElement( const String &instanceName ) override;

            /** @copydoc IOverlayManager::removeElement */
            bool removeElement( SmartPtr<IOverlayElement> element ) override;

            /** @copydoc IOverlayManager::_getObject */
            void _getObject( void **ppObject ) const override;

            /** @copydoc IOverlayManager::getOverlays */
            Array<SmartPtr<IOverlay>> getOverlays() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /** The mutex used to synchronize access to the overlay manager. */
            mutable RecursiveMutex m_mutex;

            /** The array of overlay elements. */
            Array<SmartPtr<IOverlayElement>> m_overlayElements;

            /** The array of overlays. */
            Array<SmartPtr<IOverlay>> m_overlays;

            /** The name extension used to create unique names for overlays. */
            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
