#ifndef __CGraphicsObject_h__
#define __CGraphicsObject_h__

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/BitUtil.h>
#include <FBCore/Base/Exception.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/State/States/GraphicsObjectState.h>

namespace fb
{
    namespace render
    {
        template <class T>
        class CGraphicsObject : public CSharedGraphicsObject<T>
        {
        public:
            CGraphicsObject()
            {
            }

            ~CGraphicsObject() override
            {
                CGraphicsObject<T>::unload( nullptr );
            }

            virtual void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsObject::attachToParent */
            virtual void attachToParent( SmartPtr<ISceneNode> parent ) override;

            /** @copydoc IGraphicsObject::detachFromParent */
            virtual void detachFromParent( SmartPtr<ISceneNode> parent ) override;

            /** @copydoc IGraphicsObject::setCastShadows */
            virtual void setCastShadows( bool castShadows ) override;

            /** @copydoc IGraphicsObject::getCastShadows */
            virtual bool getCastShadows() const override;

            /** @copydoc IGraphicsObject::setReceiveShadows */
            virtual void setReceiveShadows( bool recieveShadows );

            /** @copydoc IGraphicsObject::getReceiveShadows */
            virtual bool getReceiveShadows() const;

            /** @copydoc IGraphicsObject::setVisible */
            virtual void setVisible( bool visible ) override;

            /** @copydoc IGraphicsObject::isVisible */
            virtual bool isVisible() const override;

            /** @copydoc IGraphicsObject::setZOrder */
            virtual void setZOrder( u32 zOrder ) override;

            /** @copydoc IGraphicsObject::getZOrder */
            virtual u32 getZOrder() const override;

            /** @copydoc IGraphicsObject::getLocalAABB */
            virtual AABB3F getLocalAABB() const override;

            /** @copydoc IGraphicsObject::setLocalAABB */
            virtual void setLocalAABB( const AABB3F &localAABB ) override;

            /** @copydoc IGraphicsObject::isAttached */
            virtual bool isAttached() const override;

            /** @copydoc IGraphicsObject::setAttached */
            virtual void setAttached( bool attached ) override;

            /** @copydoc IGraphicsObject::getRenderTechnique */
            virtual hash32 getRenderTechnique() const override;

            /** @copydoc IGraphicsObject::setRenderTechnique */
            virtual void setRenderTechnique( hash32 renderTechnique ) override;

            /** @copydoc IGraphicsObject::setRenderQueueGroup */
            virtual void setRenderQueueGroup( u32 queueID );

            /** @copydoc IGraphicsObject::getRenderQueueGroup */
            virtual u32 getRenderQueueGroup() const;

            /** @copydoc IGraphicsObject::setVisibilityFlags */
            virtual void setVisibilityFlags( u32 flags ) override;

            /** @copydoc IGraphicsObject::getVisibilityFlags */
            virtual u32 getVisibilityFlags() const override;

            SmartPtr<GraphicsObjectState> getGraphicsObjectState() const;

            void setGraphicsObjectState( SmartPtr<GraphicsObjectState> graphicsObjectState );

            /** @copydoc IGraphicsObject::setFlag */
            virtual void setFlag( u32 flag, bool value ) override;

            /** @copydoc IGraphicsObject::getFlag */
            virtual bool getFlag( u32 flag ) const override;

            /** @copydoc IGraphicsObject::getOwner */
            virtual SmartPtr<ISceneNode> getOwner() const override;

            /** @copydoc IGraphicsObject::setOwner */
            virtual void setOwner( SmartPtr<ISceneNode> sceneNode ) override;

            /** @copydoc IGraphicsObject::getProperties */
            virtual SmartPtr<Properties> getProperties() const;

            /** @copydoc IGraphicsObject::setProperties */
            virtual void setProperties( SmartPtr<Properties> properties );

            SmartPtr<IGraphicsScene> getCreator() const;
            void setCreator( SmartPtr<IGraphicsScene> creator );

            FB_CLASS_REGISTER_TEMPLATE_DECL( CGraphicsObject, T );

        protected:
            virtual void setupStateObject()
            {
                try
                {
                    auto stateObject = CSharedGraphicsObject<T>::getStateObject();
                    if( !stateObject )
                    {
                        auto applicationManager = core::IApplicationManager::instance();
                        FB_ASSERT( applicationManager );

                        auto factoryManager = applicationManager->getFactoryManager();
                        FB_ASSERT( factoryManager );

                        auto stateManager = applicationManager->getStateManager();
                        FB_ASSERT( stateManager );

                        auto stateObject = stateManager->addStateObject();
                        CSharedGraphicsObject<T>::setStateObject( stateObject );
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            SmartPtr<GraphicsObjectState> m_graphicsObjectState;

            /** The scene node this is attached to. */
            WeakPtr<ISceneNode> m_owner;

            /// The scene manager that created the camera.
            WeakPtr<IGraphicsScene> m_creator;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CGraphicsObject, T, CSharedObject<T> );

        template <class T>
        SmartPtr<IGraphicsScene> CGraphicsObject<T>::getCreator() const
        {
            return m_creator.lock();
        }

        template <class T>
        void CGraphicsObject<T>::setCreator( SmartPtr<IGraphicsScene> creator )
        {
            m_creator = creator;
        }

    }  // namespace render
}  // namespace fb

#include <FBGraphics/Wrapper/CGraphicsObject.inl>

#endif  // CGraphicsObject_h__
