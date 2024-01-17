#ifndef __CGraphicsObject_h__
#define __CGraphicsObject_h__

#include <FBCore/Graphics/SharedGraphicsObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/State/States/GraphicsObjectState.h>

namespace fb
{
    namespace render
    {
        template <class T>
        class GraphicsObject : public SharedGraphicsObject<T>
        {
        public:
            GraphicsObject();

            virtual ~GraphicsObject() override;

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

            FB_CLASS_REGISTER_TEMPLATE_DECL( GraphicsObject, T );

        protected:
            virtual void setupStateObject();

            SmartPtr<GraphicsObjectState> m_graphicsObjectState;

            /** The scene node this is attached to. */
            WeakPtr<ISceneNode> m_owner;

            /// The scene manager that created the camera.
            WeakPtr<IGraphicsScene> m_creator;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, GraphicsObject, T, T );

        template <class T>
        GraphicsObject<T>::GraphicsObject()
        {
        }

        template <class T>
        GraphicsObject<T>::~GraphicsObject()
        {
        }

        template <class T>
        void GraphicsObject<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();

                if( auto state = getGraphicsObjectState() )
                {
                    state->unload( nullptr );
                    setGraphicsObjectState( nullptr );
                }

                if( auto stateContext = GraphicsObject<T>::getStateContext() )
                {
                    if( auto stateListener = GraphicsObject<T>::getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    stateContext->unload( nullptr );

                    if( stateManager )
                    {
                        stateManager->removeStateObject( stateContext );
                    }

                    GraphicsObject<T>::setStateContext( nullptr );
                }

                if( auto stateListener = GraphicsObject<T>::getStateListener() )
                {
                    stateListener->unload( nullptr );
                    GraphicsObject<T>::setStateListener( nullptr );
                }

                GraphicsObject<T>::m_owner = nullptr;
                GraphicsObject<T>::m_creator = nullptr;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void GraphicsObject<T>::attachToParent( SmartPtr<ISceneNode> parent )
        {
        }

        template <class T>
        void GraphicsObject<T>::detachFromParent( SmartPtr<ISceneNode> parent )
        {
        }

        template <class T>
        SmartPtr<GraphicsObjectState> GraphicsObject<T>::getGraphicsObjectState() const
        {
            return m_graphicsObjectState;
        }

        template <class T>
        void GraphicsObject<T>::setGraphicsObjectState(
            SmartPtr<GraphicsObjectState> graphicsObjectState )
        {
            m_graphicsObjectState = graphicsObjectState;
        }

        template <class T>
        AABB3F GraphicsObject<T>::getLocalAABB() const
        {
            return m_graphicsObjectState->getLocalAABB();
        }

        template <class T>
        void GraphicsObject<T>::setLocalAABB( const AABB3F &localAABB )
        {
            m_graphicsObjectState->setLocalAABB( localAABB );
        }

        template <class T>
        bool GraphicsObject<T>::isAttached() const
        {
            if( auto context = GraphicsObject<T>::getStateContext() )
            {
                if( auto state = context->template getStateByType<GraphicsObjectState>() )
                {
                    return state->isAttached();
                }
            }

            return false;
        }

        template <class T>
        void GraphicsObject<T>::setAttached( bool attached )
        {
            if( auto context = GraphicsObject<T>::getStateContext() )
            {
                if( auto state = context->template getStateByType<GraphicsObjectState>() )
                {
                    return state->setAttached( attached );
                }
            }
        }

        template <class T>
        hash32 GraphicsObject<T>::getRenderTechnique() const
        {
            return m_graphicsObjectState->getRenderTechnique();
        }

        template <class T>
        void GraphicsObject<T>::setRenderTechnique( hash32 val )
        {
            m_graphicsObjectState->setRenderTechnique( val );
        }

        template <class T>
        void GraphicsObject<T>::setRenderQueueGroup( u32 queueID )
        {
            m_graphicsObjectState->setRenderQueueGroup( queueID );
        }

        template <class T>
        u32 GraphicsObject<T>::getRenderQueueGroup() const
        {
            return m_graphicsObjectState->getRenderQueueGroup();
        }

        template <class T>
        void GraphicsObject<T>::setCastShadows( bool castShadows )
        {
            m_graphicsObjectState->setCastShadows( castShadows );
        }

        template <class T>
        bool GraphicsObject<T>::getCastShadows() const
        {
            return m_graphicsObjectState->getCastShadows();
        }

        template <class T>
        void GraphicsObject<T>::setReceiveShadows( bool recieveShadows )
        {
            m_graphicsObjectState->setReceiveShadows( recieveShadows );
        }

        template <class T>
        bool GraphicsObject<T>::getReceiveShadows() const
        {
            return m_graphicsObjectState->getReceiveShadows();
        }

        template <class T>
        void GraphicsObject<T>::setZOrder( u32 zOrder )
        {
            m_graphicsObjectState->setZorder( zOrder );
        }

        template <class T>
        u32 GraphicsObject<T>::getZOrder() const
        {
            return m_graphicsObjectState->getZorder();
        }

        template <class T>
        void GraphicsObject<T>::setVisibilityFlags( u32 flags )
        {
            m_graphicsObjectState->setVisibilityMask( flags );
        }

        template <class T>
        u32 GraphicsObject<T>::getVisibilityFlags() const
        {
            return m_graphicsObjectState->getVisibilityMask();
        }

        template <class T>
        void GraphicsObject<T>::setVisible( bool visible )
        {
            if( m_graphicsObjectState )
            {
                m_graphicsObjectState->setVisible( visible );
            }
        }

        template <class T>
        bool GraphicsObject<T>::isVisible() const
        {
            if( m_graphicsObjectState )
            {
                return m_graphicsObjectState->isVisible();
            }

            return false;
        }

        template <class T>
        void GraphicsObject<T>::setFlag( u32 flag, bool value )
        {
            auto flags = GraphicsObject<T>::m_graphicsObjectState->getFlags();
            auto newFlags = BitUtil::setFlagValue( flags, flag, value );
            m_graphicsObjectState->setFlags( newFlags );
        }

        template <class T>
        bool GraphicsObject<T>::getFlag( u32 flag ) const
        {
            auto flags = GraphicsObject<T>::m_graphicsObjectState->getFlags();
            return BitUtil::getFlagValue( flags, flag );
        }

        template <class T>
        SmartPtr<ISceneNode> GraphicsObject<T>::getOwner() const
        {
            return GraphicsObject<T>::m_owner.lock();
        }

        template <class T>
        void GraphicsObject<T>::setOwner( SmartPtr<ISceneNode> sceneNode )
        {
            detachFromParent( GraphicsObject<T>::m_owner.lock() );

            GraphicsObject<T>::m_owner = sceneNode;

            attachToParent( GraphicsObject<T>::m_owner.lock() );
        }

        template <class T>
        SmartPtr<Properties> GraphicsObject<T>::getProperties() const
        {
            auto properties = SharedGraphicsObject<T>::getProperties();

            auto name = String( "null" );

            if( auto owner = GraphicsObject<T>::getOwner() )
            {
                if( auto handle = owner->getHandle() )
                {
                    name = handle->getName();
                }
            }

            properties->setProperty( "owner", name );

            auto visible = GraphicsObject<T>::isVisible();
            properties->setProperty( "visible", visible );
            return properties;
        }

        template <class T>
        void GraphicsObject<T>::setProperties( [[maybe_unused]] SmartPtr<Properties> properties )
        {
            SharedGraphicsObject<T>::setProperties( properties );

            auto visible = GraphicsObject<T>::isVisible();
            properties->setProperty( "visible", visible );

            if( visible != GraphicsObject<T>::isVisible() )
            {
                GraphicsObject<T>::setVisible( visible );
            }
        }

        template <class T>
        SmartPtr<IGraphicsScene> GraphicsObject<T>::getCreator() const
        {
            return m_creator.lock();
        }

        template <class T>
        void GraphicsObject<T>::setCreator( SmartPtr<IGraphicsScene> creator )
        {
            m_creator = creator;
        }

        template <class T>
        void GraphicsObject<T>::setupStateObject()
        {
            try
            {
                auto stateContext = SharedGraphicsObject<T>::getStateContext();
                if( !stateContext )
                {
                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto factoryManager = applicationManager->getFactoryManager();
                    FB_ASSERT( factoryManager );

                    auto stateManager = applicationManager->getStateManager();
                    FB_ASSERT( stateManager );

                    auto stateContext = stateManager->addStateObject();
                    SharedGraphicsObject<T>::setStateContext( stateContext );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // namespace render
}  // namespace fb

#endif  // CGraphicsObject_h__
