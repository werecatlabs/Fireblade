#ifndef _CTextureManager_H
#define _CTextureManager_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ITextureManager.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {
        class CTextureManagerOgre : public CSharedObject<ITextureManager>
        {
        public:
            CTextureManagerOgre();
            ~CTextureManagerOgre() override;

            /** @copydoc ISharedObject::update */
            void preUpdate() override;

            /** @copydoc ISharedObject::update */
            void update() override;

            /** @copydoc ISharedObject::postUpdate */
            void postUpdate() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &name ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid, const String &name ) override;

            /** @copydoc IResourceManager::load */
            SmartPtr<IResource> load( const String &name ) override;

            /** @copydoc IResourceManager::getByName */
            SmartPtr<IResource> getByName( const String &name ) override;

            /** @copydoc IResourceManager::getById */
            SmartPtr<IResource> getById( const String &uuid ) override;

            /** @copydoc IResourceManager::createOrRetrieve */
            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                              const String &type ) override;

            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path );


            /** @copydoc IResourceManager::saveToFile */
            void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

            /** @copydoc IResourceManager::loadFromFile */
            SmartPtr<IResource> loadFromFile( const String &filePath ) override;

            /** @copydoc ITextureManager::createManual */
            SmartPtr<ITexture> createManual( const String &name, const String &group, u8 texType,
                                             u32 width, u32 height, u32 depth, s32 num_mips, u8 format,
                                             s32 usage = 0 ) override;

            /** @copydoc ITextureManager::createVideoTexture */
            SmartPtr<IVideoTexture> createVideoTexture( const String &name ) override;

            /** @copydoc ITextureManager::addCubemap */
            SmartPtr<ICubemap> addCubemap() override;

            /** @copydoc ITextureManager::createRenderTexture */
            SmartPtr<ITexture> createRenderTexture() override;

            /** @copydoc ITextureManager::destroyRenderTexture */
            void destroyRenderTexture( SmartPtr<ITexture> texture ) override;

            /** @copydoc ITextureManager::createSkyBoxCubeMap */
            SmartPtr<ITexture> createSkyBoxCubeMap( Array<String> skyboxTextures ) override;

            /** @copydoc ITextureManager::createSkyBoxCubeMap */
            SmartPtr<ITexture> createSkyBoxCubeMap( Array<SmartPtr<ITexture>> skyboxTextures ) override;

            /** @copydoc ITextureManager::createSkyBoxCubeMap */
            SmartPtr<ITexture> createSkyBoxCubeMap( SmartPtr<IMaterial> material ) override;

            /** @copydoc ITextureManager::_getObject */
            void _getObject( void **ppObject ) const override;

        protected:
            class TextureListener : public CSharedObject<IEventListener>
            {
            public:
                TextureListener() = default;
                ~TextureListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object,
                                       SmartPtr<IEvent> event );

                void loadingStateChanged(ISharedObject* sharedObject, LoadingState oldState, LoadingState newState);

                bool destroy( void *ptr );
            };

            void addTexture( SmartPtr<ITexture> texture );
            void removeTexture( SmartPtr<ITexture> texture );

            void setTextures( SharedPtr<ConcurrentArray<SmartPtr<ITexture>>> p );
            SharedPtr<ConcurrentArray<SmartPtr<ITexture>>> getTextures() const;

            void addRenderTexture(SmartPtr<ITexture> texture);

            void removeRenderTexture( SmartPtr<ITexture> texture );

            void createRenderTextures();

            mutable RecursiveMutex m_mutex;

            AtomicSharedPtr<ConcurrentArray<SmartPtr<ITexture>>> m_textures;
            Array<SmartPtr<ITexture>> m_renderTargets;

            SmartPtr<TextureListener> m_textureListener;

            // ConcurrentQueue<SmartPtr<ITexture>> m_textures;
        };
    }  // end namespace render
}  // end namespace fb

#endif
