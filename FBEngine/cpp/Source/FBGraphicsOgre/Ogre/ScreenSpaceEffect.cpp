#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>

#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRenderTarget.h>
#include <OgrePixelFormat.h>
#include <OgreTextureManager.h>
#include <OgrePass.h>
#include <OgreSceneManager.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRoot.h>
#include <OgreRenderTexture.h>
#include <OgreRenderSystem.h>

#include "FBGraphicsOgre/Ogre/ScreenSpaceEffect.h"
#include "FBGraphicsOgre/Ogre/SSEffectRenderer.h"

namespace fb
{
    namespace render
    {

        ScreenSpaceEffect::ScreenSpaceEffect( SSEffectRenderer *r ) :
            _camera( NULL ),
            _viewport( NULL ),
            _texture( NULL ),
            _textureIsExtern( false ),
            _rt( NULL ),
            _pass( NULL ),
            _renderer( r ),
            _active( true )
        {
        }

        ScreenSpaceEffect::~ScreenSpaceEffect()
        {
            destroy();
        }

        void ScreenSpaceEffect::destroy()
        {
            if( !_textureIsExtern )
            {
                if( _texture )
                    Ogre::TextureManager::getSingleton().remove( _texture->getName() );
                if( _camera )
                    _camera->getSceneManager()->destroyCamera( _camera );
                _texture = NULL;
                _camera = NULL;
                _rt = NULL;
                _viewport = NULL;
            }
        }

        void ScreenSpaceEffect::create( const Ogre::String &name, size_t w, size_t h,
                                        Ogre::PixelFormat pf )
        {
            destroy();

            Ogre::TexturePtr midPtr = Ogre::TextureManager::getSingleton().createManual(
                name, "Internal", Ogre::TEX_TYPE_2D, w, h, 0,  // 0 mip maps
                pf, Ogre::TU_RENDERTARGET );

            _texture = midPtr.get();
            _textureIsExtern = false;
            _rt = _texture->getBuffer( 0 )->getRenderTarget( 0 );
            _rt->setAutoUpdated( false );

            _camera = _renderer->sceneMgr()->createCamera( name + "_camera" );

            _viewport = _rt->addViewport( _camera, _rt->getNumViewports() );
            _viewport->setBackgroundColour( Ogre::ColourValue::White );
            _viewport->setClearEveryFrame( false );
            _viewport->setShadowsEnabled( false );

            _camera->setAspectRatio( float( w ) / h );
            _camera->setFOVy( Ogre::Degree( 90 ) );
        }

        void ScreenSpaceEffect::create( Ogre::Texture *tex )
        {
            destroy();

            _texture = tex;
            _textureIsExtern = true;
            _rt = _texture->getBuffer( 0 )->getRenderTarget( 0 );

            _viewport = _rt->getViewport( 0 );
            _camera = _viewport->getCamera();
        }

        void ScreenSpaceEffect::update() const
        {
            if( !_active || !_pass )
                return;

            _renderer->fsquad( _pass, _viewport );
        }

        void ScreenSpaceEffect::clear() const
        {
            _renderer->renderSys()->_setViewport( _viewport );
            _renderer->renderSys()->clearFrameBuffer(
                Ogre::FBT_COLOUR | Ogre::FBT_DEPTH | Ogre::FBT_STENCIL,
                _viewport->getBackgroundColour() );
        }

        Ogre::Camera *ScreenSpaceEffect::camera() const
        {
            return _camera;
        }

        Ogre::Viewport *ScreenSpaceEffect::viewport() const
        {
            return _viewport;
        }

        Ogre::Texture *ScreenSpaceEffect::texture() const
        {
            return _texture;
        }

        Ogre::RenderTarget *ScreenSpaceEffect::rt() const
        {
            return _rt;
        }

        void ScreenSpaceEffect::pass( Ogre::Pass *p )
        {
            _pass = p;
        }

        Ogre::Pass *ScreenSpaceEffect::pass() const
        {
            return _pass;
        }

        void ScreenSpaceEffect::active( bool b )
        {
            _active = b;
        }

        bool ScreenSpaceEffect::active() const
        {
            return _active;
        }

        fb::SmartPtr<fb::render::IScreenSpaceEffectRenderer> ScreenSpaceEffect::getRenderer() const
        {
            return nullptr;
        }

        void ScreenSpaceEffect::setRenderer( SmartPtr<IScreenSpaceEffectRenderer> renderer )
        {
        }

    }  // namespace render
}  // namespace fb
