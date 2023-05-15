#ifndef FBOgreVideoTexture_h__
#define FBOgreVideoTexture_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Video/IVideoTexture.h>
#include <FBGraphics/Wrapper/CResourceGraphics.h>
#include <FBCore/Base/Handle.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {
        class CVideoTextureOgre : public CResourceGraphics<IVideoTexture>
        {
        public:
            CVideoTextureOgre();
            ~CVideoTextureOgre() override;

            void initialise( const String &name, const Vector2I &size ) override;

            void update() override;

            void _copyFrameData( void *frameData, const Vector2I &size );

            void _getObject( void **object );

            void _getObject( void **ppObject ) const override;

            Vector2I getSize() const override;
            void setSize( const Vector2I &size ) override;

            SmartPtr<IRenderTarget> getRenderTarget() const override;
            void setRenderTarget(SmartPtr<IRenderTarget> rt);

            void copyToTexture( SmartPtr<ITexture> &target ) override;

            void copyData( void *data, const Vector2I &size ) override;

            Vector2I getActualSize() const override;

            /** Gets the gpu texture. */
            void getTextureGPU( void **ppTexture ) const override;

            /** Gets the final gpu texture. This object depends on the graphics api. */
            void getTextureFinal( void **ppTexture ) const override;

            /** @copydoc ITexture::getTextureHandle */
            size_t getTextureHandle() const override;

            u32 getUsageFlags() const override;

            void setUsageFlags( u32 usageFlags ) override;

            void load();

            void reload();

            void unload();

            bool isLoaded() const override;

            void save() override;

            hash64 getFileSystemId() const override;

            void setFileSystemId( hash64 id ) override;

            SmartPtr<Properties> getProperties() const override;

            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<IStateContext> getStateObject() const override;

            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

        protected:
            SmartPtr<IStateContext> m_stateObject;
            Ogre::TexturePtr m_texture;
            Vector2I m_size;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // FBOgreVideoTexture_h__
