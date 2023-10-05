#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CRenderTexture.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        CRenderTexture::CRenderTexture()
        {
            setupStateObject();
        }

        CRenderTexture::~CRenderTexture()
        {
        }

        void CRenderTexture::update()
        {
            auto size = getSize();
            if( size.x > 0 && size.y > 0 )
            {
                CRenderTargetOgre<IRenderTexture>::update();
            }
        }

        void CRenderTexture::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );
                CRenderTargetOgre<IRenderTexture>::load( data );
                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CRenderTexture::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    CRenderTargetOgre<IRenderTexture>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // namespace render
}  // namespace fb
