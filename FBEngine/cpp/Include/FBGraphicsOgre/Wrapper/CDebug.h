#ifndef CDebug_h__
#define CDebug_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Thread/Threading.h>
#include <OgreMaterial.h>

namespace fb
{
    namespace render
    {
        class CDebug : public SharedObject<IDebug>
        {
        public:
            CDebug();
            ~CDebug() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void preUpdate() override;
            void update() override;
            void postUpdate() override;

            void drawPoint( s32 id, const Vector3<real_Num> &positon, u32 color ) override;
            void drawLine( s32 id, const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                           u32 colour ) override;

            void drawText( s32 id, const Vector2<real_Num> &position, const String &text, u32 color );

            FB_CLASS_REGISTER_DECL;

        private:
            SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> getDebugLines() const;
            void setDebugLines( SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> debugLines );

            SmartPtr<IDebugLine> addLine( s32 id );
            void removeLine( s32 id );
            void removeLine( SmartPtr<IDebugLine> visualForceVector );
            SmartPtr<IDebugLine> getLine( s32 id ) const;

            void clear() override;

            SmartPtr<IOverlay> getOverlay() const;

            void setOverlay( SmartPtr<IOverlay> overlay );

            SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> getOverlayElementsPtr() const;

            void setOverlayElementsPtr( SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> elements );

            void addOverlayElement( SmartPtr<IOverlayElement> element );
            SmartPtr<IOverlayElement> getElementById(s32 id) const;

            SmartPtr<IOverlay> m_overlay;
            SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> m_overlayElements;

            ConcurrentQueue<SmartPtr<IDebugLine>> m_removeQueue;
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> m_debugLines;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CDebug_h__
