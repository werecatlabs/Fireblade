#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgreStateListener.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgre.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb, CTextureOgreStateListener, IStateListener );

        void CTextureOgreStateListener::unload( SmartPtr<ISharedObject> data )
        {
            m_owner = nullptr;
        }

        void CTextureOgreStateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( message->isDerived<StateMessageVector2I>() )
                {
                    auto vector2Message = fb::static_pointer_cast<StateMessageVector2I>( message );
                    auto value = vector2Message->getValue();
                    auto type = vector2Message->getType();

                    if( type == ITexture::STATE_MESSAGE_TEXTURE_SIZE )
                    {
                        owner->setSize( value );
                    }
                }
            }
        }

        void CTextureOgreStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                if( owner->isLoaded() )
                {
                    auto textureState = fb::static_pointer_cast<TextureState>( state );
                    auto size = textureState->getSize();
                    auto texture = owner->getTexture();

                    if( texture )
                    {
                        auto currentSize = Vector2I( texture->getWidth(), texture->getHeight() );

                        if( currentSize != size )
                        {
                            return; // todo: handle this case

                            texture->setWidth( size.x );
                            texture->setHeight( size.y );

                            auto usageFlags = owner->getUsageFlags();

                            if( ( usageFlags & ITexture::TU_RENDERTARGET ) != 0 )
                            {
#if defined FB_PLATFORM_WIN32
                                owner->load( nullptr );
#elif defined FB_PLATFORM_APPLE
                                owner->load( nullptr );
#endif
                            }
                        }
                    }

                    state->setDirty( false );
                }
            }
        }

        void CTextureOgreStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<CTextureOgre> CTextureOgreStateListener::getOwner() const
        {
            auto p = m_owner.load();
            return p.lock();
        }

        void CTextureOgreStateListener::setOwner( SmartPtr<CTextureOgre> owner )
        {
            m_owner = owner;
        }

    }  // namespace render
}  // namespace fb
