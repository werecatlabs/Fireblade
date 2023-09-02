#ifndef SSEffect_h__
#define SSEffect_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IScreenSpaceEffect.h>

#include <OgrePixelFormat.h>
#include <boost/shared_ptr.hpp>
#include <OgrePrerequisites.h>
#include <OgreString.h>
#include <map>

namespace fb
{
    namespace render
    {

        class ScreenSpaceEffect : public IScreenSpaceEffect
        {
        public:
            ScreenSpaceEffect( SSEffectRenderer *r );
            ~ScreenSpaceEffect() override;

            virtual void create( const Ogre::String &name, size_t w = 256, size_t h = 256,
                                 Ogre::PixelFormat pf = Ogre::PF_R8G8B8 );
            virtual void create( Ogre::Texture *tex );
            virtual void destroy();

            virtual void update() const;
            virtual void clear() const;

            Ogre::Camera *camera() const;
            Ogre::Viewport *viewport() const;
            Ogre::Texture *texture() const;
            Ogre::RenderTarget *rt() const;

            Ogre::Pass *pass() const;
            void pass( Ogre::Pass *p );

            bool active() const;
            void active( bool b );

            SmartPtr<IScreenSpaceEffectRenderer> getRenderer() const override;
            void setRenderer( SmartPtr<IScreenSpaceEffectRenderer> renderer ) override;

        protected:
            Ogre::Camera *_camera;
            Ogre::Viewport *_viewport;
            Ogre::Texture *_texture;
            Ogre::RenderTarget *_rt;
            Ogre::Pass *_pass;

            SSEffectRenderer *_renderer;

            bool _active;
            bool _textureIsExtern;
        };
    }  // namespace render
}  // end namespace fb

#endif  // SSEffect_h__
