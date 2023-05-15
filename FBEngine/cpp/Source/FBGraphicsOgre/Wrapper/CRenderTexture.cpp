#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CRenderTexture.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        void CRenderTexture::update()
        {
            auto size = getSize();
            if( size.x > 0 && size.y > 0 )
            {
                CRenderTarget<IRenderTexture>::update();
            }
        }

        void CRenderTexture::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );
                CRenderTarget<IRenderTexture>::load( data );
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

                    CRenderTarget<IRenderTexture>::unload( data );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto fileSystem = applicationManager->getFileSystem();
                    FB_ASSERT( fileSystem );

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
