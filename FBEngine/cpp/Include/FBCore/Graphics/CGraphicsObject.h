#ifndef __CGraphicsObject_h__
#define __CGraphicsObject_h__

#include <FBCore/Graphics/CSharedGraphicsObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
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
            CGraphicsObject();

            virtual ~CGraphicsObject() override;

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
            virtual void setupStateObject();

            SmartPtr<GraphicsObjectState> m_graphicsObjectState;

            /** The scene node this is attached to. */
            WeakPtr<ISceneNode> m_owner;

            /// The scene manager that created the camera.
            WeakPtr<IGraphicsScene> m_creator;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CGraphicsObject, T, T );

        template <class T>
        CGraphicsObject<T>::CGraphicsObject()
        {
        }

        template <class T>
        CGraphicsObject<T>::~CGraphicsObject()
        {
        }

        template <class T>
        void CGraphicsObject<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();

                if( auto state = getGraphicsObjectState() )
                {
                    state->unload( nullptr );
                    setGraphicsObjectState( nullptr );
                }

                if( auto stateObject = CGraphicsObject<T>::getStateObject() )
                {
                    if( auto stateListener = CGraphicsObject<T>::getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                    }

                    stateObject->unload( nullptr );

                    if( stateManager )
                    {
                        stateManager->removeStateObject( stateObject );
                    }

                    CGraphicsObject<T>::setStateObject( nullptr );
                }

                if( auto stateListener = CGraphicsObject<T>::getStateListener() )
                {
                    stateListener->unload( nullptr );
                    CGraphicsObject<T>::setStateListener( nullptr );
                }

                CGraphicsObject<T>::m_owner = nullptr;
                CGraphicsObject<T>::m_creator = nullptr;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CGraphicsObject<T>::attachToParent( SmartPtr<ISceneNode> parent )
        {
        }

        template <class T>
        void CGraphicsObject<T>::detachFromParent( SmartPtr<ISceneNode> parent )
        {
        }

        template <class T>
        SmartPtr<GraphicsObjectState> CGraphicsObject<T>::getGraphicsObjectState() const
        {
            return m_graphicsObjectState;
        }

        template <class T>
        void CGraphicsObject<T>::setGraphicsObjectState(
            SmartPtr<GraphicsObjectState> graphicsObjectState )
        {
            m_graphicsObjectState = graphicsObjectState;
        }

        template <class T>
        AABB3F CGraphicsObject<T>::getLocalAABB() const
        {
            return m_graphicsObjectState->getLocalAABB();
        }

        template <class T>
        void CGraphicsObject<T>::setLocalAABB( const AABB3F &localAABB )
        {
            m_graphicsObjectState->setLocalAABB( localAABB );
        }

        template <class T>
        bool CGraphicsObject<T>::isAttached() const
        {
            if( auto context = CGraphicsObject<T>::getStateObject() )
            {
                if( auto state = context->template getStateByType<GraphicsObjectState>() )
                {
                    return state->isAttached();
                }
            }

            return false;
        }

        template <class T>
        void CGraphicsObject<T>::setAttached( bool attached )
        {
            if( auto context = CGraphicsObject<T>::getStateObject() )
            {
                if( auto state = context->template getStateByType<GraphicsObjectState>() )
                {
                    return state->setAttached( attached );
                }
            }
        }

        template <class T>
        hash32 CGraphicsObject<T>::getRenderTechnique() const
        {
            return m_graphicsObjectState->getRenderTechnique();
        }

        template <class T>
        void CGraphicsObject<T>::setRenderTechnique( hash32 val )
        {
            m_graphicsObjectState->setRenderTechnique( val );
        }

        template <class T>
        void CGraphicsObject<T>::setRenderQueueGroup( u32 queueID )
        {
            m_graphicsObjectState->setRenderQueueGroup( queueID );
        }

        template <class T>
        u32 CGraphicsObject<T>::getRenderQueueGroup() const
        {
            return m_graphicsObjectState->getRenderQueueGroup();
        }

        template <class T>
        void CGraphicsObject<T>::setCastShadows( bool castShadows )
        {
            m_graphicsObjectState->setCastShadows( castShadows );
        }

        template <class T>
        bool CGraphicsObject<T>::getCastShadows() const
        {
            return m_graphicsObjectState->getCastShadows();
        }

        template <class T>
        void CGraphicsObject<T>::setReceiveShadows( bool recieveShadows )
        {
            m_graphicsObjectState->setReceiveShadows( recieveShadows );
        }

        template <class T>
        bool CGraphicsObject<T>::getReceiveShadows() const
        {
            return m_graphicsObjectState->getReceiveShadows();
        }

        template <class T>
        void CGraphicsObject<T>::setZOrder( u32 zOrder )
        {
            m_graphicsObjectState->setZorder( zOrder );
        }

        template <class T>
        u32 CGraphicsObject<T>::getZOrder() const
        {
            return m_graphicsObjectState->getZorder();
        }

        template <class T>
        void CGraphicsObject<T>::setVisibilityFlags( u32 flags )
        {
            m_graphicsObjectState->setVisibilityMask( flags );
        }

        template <class T>
        u32 CGraphicsObject<T>::getVisibilityFlags() const
        {
            return m_graphicsObjectState->getVisibilityMask();
        }

        template <class T>
        void CGraphicsObject<T>::setVisible( bool visible )
        {
            if( m_graphicsObjectState )
            {
                m_graphicsObjectState->setVisible( visible );
            }
        }

        template <class T>
        bool CGraphicsObject<T>::isVisible() const
        {
            if( m_graphicsObjectState )
            {
                return m_graphicsObjectState->isVisible();
            }

            return false;
        }

        template <class T>
        void CGraphicsObject<T>::setFlag( u32 flag, bool value )
        {
            auto flags = CGraphicsObject<T>::m_graphicsObjectState->getFlags();
            auto newFlags = BitUtil::setFlagValue( flags, flag, value );
            m_graphicsObjectState->setFlags( newFlags );
        }

        template <class T>
        bool CGraphicsObject<T>::getFlag( u32 flag ) const
        {
            auto flags = CGraphicsObject<T>::m_graphicsObjectState->getFlags();
            return BitUtil::getFlagValue( flags, flag );
        }

        template <class T>
        SmartPtr<ISceneNode> CGraphicsObject<T>::getOwner() const
        {
            return CGraphicsObject<T>::m_owner.lock();
        }

        template <class T>
        void CGraphicsObject<T>::setOwner( SmartPtr<ISceneNode> sceneNode )
        {
            detachFromParent( CGraphicsObject<T>::m_owner.lock() );

            CGraphicsObject<T>::m_owner = sceneNode;

            attachToParent( CGraphicsObject<T>::m_owner.lock() );
        }

        template <class T>
        SmartPtr<Properties> CGraphicsObject<T>::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

            auto name = String( "null" );

            if( auto owner = CGraphicsObject<T>::getOwner() )
            {
                if( auto handle = owner->getHandle() )
                {
                    name = handle->getName();
                }
            }

            properties->setProperty( "owner", name );

            auto visible = CGraphicsObject<T>::isVisible();
            properties->setProperty( "visible", visible );
            return properties;
        }

        template <class T>
        void CGraphicsObject<T>::setProperties( [[maybe_unused]] SmartPtr<Properties> properties )
        {
        }

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

        template <class T>
        void CGraphicsObject<T>::setupStateObject()
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

    }  // namespace render
}  // namespace fb

#endif  // CGraphicsObject_h__
