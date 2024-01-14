#ifndef FBOgreVideoTexture_h__
#define FBOgreVideoTexture_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Video/IVideoTexture.h>
#include <FBCore/Graphics/ResourceGraphics.h>
//#include <OgreTexture.h>

namespace fb
{
    namespace render
    {
        
        class CVideoTextureOgreNext : public ResourceGraphics<IVideoTexture>
        {
        public:
            CVideoTextureOgreNext();
            ~CVideoTextureOgreNext() override;

            void initialise( const String &name, const Vector2I &size ) override;

            void update( const s32 &task, const time_interval &t, const time_interval &dt );

            void _copyFrameData( void *frameData, const Vector2I &size );

            void _getObject( void **object );

            void _getObject( void **ppObject ) const override;

            Vector2I getSize() const override;
            void setSize( const Vector2I &size ) override;

            SmartPtr<IRenderTarget> getRenderTarget() const override;

            void copyToTexture( SmartPtr<ITexture> &target ) override;

            void copyData( void *data, const Vector2I &size ) override;

            Vector2I getActualSize() const override;

        protected:
            // Ogre::TexturePtr m_texture;
            Vector2I m_size;
        };
    } // end namespace render
}     // end namespace fb

#endif  // FBOgreVideoTexture_h__
