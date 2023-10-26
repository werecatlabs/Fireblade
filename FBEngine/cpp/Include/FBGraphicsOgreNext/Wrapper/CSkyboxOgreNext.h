#ifndef CSkybox_h__
#define CSkybox_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/ISkybox.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        
        class CSkyboxOgreNext : public ISkybox
        {
        public:
            CSkyboxOgreNext();
            ~CSkyboxOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISkybox::setMaterial */
            void setMaterial( SmartPtr<IMaterial> material ) override;

            /** @copydoc ISkybox::getMaterial */
            SmartPtr<IMaterial> getMaterial() const override;

            /** @copydoc ISkybox::getSceneManager */
            SmartPtr<IGraphicsScene> getSceneManager() const override;

            /** @copydoc ISkybox::setSceneManager */
            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) override;

            /** @copydoc ISkybox::isVisible */
            bool isVisible() const override;

            /** @copydoc ISkybox::setVisible */
            void setVisible( bool visible ) override;

            f32 getDistance() const;

            void setDistance( f32 distance );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<SkyBoxRenderer> m_skyBoxRenderer;
            SmartPtr<IGraphicsScene> m_sceneManager;
            Array<SmartPtr<Quad>> m_quads;
            Array<SmartPtr<IMaterial>> m_materials;
            SmartPtr<IMaterial> m_material;
            f32 m_distance = 1.0f;
            bool m_isVisible = true;
        };
    } // end namespace render
}     // end namespace fb

#endif  // CSkybox_h__
