#ifndef CDebug_h__
#define CDebug_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {

        class CDebugOgreNext : public SharedGraphicsObject<IDebug>
        {
        public:
            CDebugOgreNext() = default;
            CDebugOgreNext( const CDebugOgreNext &other ) = delete;
            ~CDebugOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::preUpdate */
            void preUpdate() override;

            /** @copydoc ISharedObject::update */
            void update() override;

            /** @copydoc ISharedObject::postUpdate */
            void postUpdate() override;

            /** @copydoc IDebug::clear */
            void clear() override;

            /** @copydoc IDebug::drawPoint */
            void drawPoint( hash_type id, const Vector3<real_Num> &positon, u32 color ) override;

            /** @copydoc IDebug::drawLine */
            SmartPtr<IDebugLine> drawLine( hash_type id, const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                           u32 colour ) override;

            SmartPtr<IDebugCircle> drawCircle( hash_type id, const Vector3<real_Num> &position,
                                               const Quaternion<real_Num> &orientation, real_Num radius,
                                               u32 color );

            Ogre::HlmsUnlitDatablock *getDatablock() const;
            void setDatablock( Ogre::HlmsUnlitDatablock *datablock );

            void drawText( hash_type id, const Vector2<real_Num> &position, const String &text,
                           u32 color ) override;

        private:
            class StateListener : public IStateListener
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CDebugOgreNext *getOwner() const;

                void setOwner( CDebugOgreNext *owner );

            protected:
                CDebugOgreNext *m_owner = nullptr;
            };

            void createLineMaterial();

            SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> getDebugLines() const;
            void setDebugLines( SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> debugLines );

            SharedPtr<ConcurrentArray<SmartPtr<IDebugCircle>>> getDebugCircles() const;
            void setDebugCircles( SharedPtr<ConcurrentArray<SmartPtr<IDebugCircle>>> debugCircles );


            SmartPtr<IDebugLine> addLine( s32 id );
            void removeLine( s32 id );
            void removeLine( SmartPtr<IDebugLine> debugLine );
            SmartPtr<IDebugLine> getLine( s32 id ) const;

            SmartPtr<IDebugCircle> addCircle( s32 id );
            void removeCircle( s32 id );
            void removeCircle( SmartPtr<IDebugCircle> debugCircle );
            SmartPtr<IDebugCircle> getCircle( s32 id ) const;

            SmartPtr<IOverlay> getOverlay() const;

            void setOverlay( SmartPtr<IOverlay> overlay );

            SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> getOverlayElementsPtr() const;

            void setOverlayElementsPtr( SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> elements );

            void addOverlayElement( SmartPtr<IOverlayElement> element );
            SmartPtr<IOverlayElement> getElementById( s32 id ) const;

            ConcurrentQueue<SmartPtr<IDebugLine>> m_removeQueue;
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> m_debugLines;
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IDebugCircle>>> m_debugCircles;

            SmartPtr<IOverlay> m_overlay;
            SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> m_overlayElements;

            Ogre::HlmsUnlitDatablock *m_datablock = nullptr;

            Ogre::ObjectMemoryManager *m_memoryManager = nullptr;

            Ogre::v1::WireBoundingBox *m_wireBoundingBox = nullptr;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CDebug_h__
