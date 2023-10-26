#ifndef _COverlayManager_H
#define _COverlayManager_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IOverlayManager.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        
        class COverlayManagerOgreNext : public IOverlayManager
        {
        public:
            COverlayManagerOgreNext();
            ~COverlayManagerOgreNext() override;

            void update() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IOverlay> addOverlay( const String &instanceName ) override;
            bool removeOverlay( const String &instanceName ) override;
            bool removeOverlay( SmartPtr<IOverlay> overlay ) override;

            SmartPtr<IOverlay> findOverlay( const String &instanceName ) override;
            bool hasOverlay( const String &instanceName ) override;

            SmartPtr<IOverlayElement> addElement( const String &typeName,
                                                  const String &instanceName ) override;
            SmartPtr<IOverlayElement> findElement( const String &instanceName ) override;
            bool removeElement( SmartPtr<IOverlayElement> element ) override;

            void _getObject( void **ppObject ) const override;

            Array<SmartPtr<IOverlay>> getOverlays() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            mutable RecursiveMutex m_mutex;

            Ogre::v1::OverlayManager *m_overlayMgr = nullptr;

            Array<SmartPtr<IOverlayElement>> m_overlayElements;

            Array<SmartPtr<IOverlay>> m_overlays;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
