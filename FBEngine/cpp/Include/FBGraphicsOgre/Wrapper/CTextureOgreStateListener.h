#ifndef CTextureOgreStateListener_h__
#define CTextureOgreStateListener_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBGraphics/Wrapper/CTexture.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {

        class CTextureOgreStateListener : public CSharedObject<IStateListener>
        {
        public:
            CTextureOgreStateListener() = default;
            ~CTextureOgreStateListener() override = default;

            void unload( SmartPtr<ISharedObject> data ) override;

            void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
            void handleStateChanged( SmartPtr<IState> &state ) override;
            void handleQuery( SmartPtr<IStateQuery> &query ) override;

            SmartPtr<CTextureOgre> getOwner() const;
            void setOwner( SmartPtr<CTextureOgre> owner );

            FB_CLASS_REGISTER_DECL;

        protected:
            AtomicWeakPtr<CTextureOgre> m_owner;
        };

    }  // namespace render
}  // namespace fb

#endif  // CTextureOgreStateListener_h__
