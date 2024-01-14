#ifndef FBPhysxShape_h__
#define FBPhysxShape_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBPhysx/FBPhysxSharedObject.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IPhysicsBody3.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/State/States/ShapeState.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBPhysx/FBPhysxManager.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        template <class T>
        class PhysxShape : public PhysxSharedObject<T>
        {
        public:
            PhysxShape() = default;
            ~PhysxShape() override = default;

            void unload( SmartPtr<ISharedObject> data );

            SmartPtr<IPhysicsMaterial3> getMaterial() const override
            {
                return m_material;
            }

            void setMaterial( SmartPtr<IPhysicsMaterial3> material ) override
            {
                m_material = material;
            }

            void _getObject( void **ppObject ) const override
            {
                *ppObject = getShape();
            }

            void setLocalPose( const Transform3<real_Num> &pose ) override;

            Transform3<real_Num> getLocalPose() const override;

            void setSimulationFilterData( const FilterData &data ) override
            {
                m_filterData = data;
            }

            FilterData getSimulationFilterData() const override
            {
                return m_filterData;
            }

            void setActor( SmartPtr<IPhysicsBody3> body ) override
            {
                m_body = body;
                m_body->_getObject( (void **)&m_pxActor );
            }

            SmartPtr<IPhysicsBody3> getActor() const override
            {
                return m_body;
            }

            physx::PxShape *getShape() const
            {
                auto p = m_shape.load();
                return p.get();
            }

            void setShape( physx::PxShape *shape )
            {
                m_shape = shape;
            }

            virtual bool hasShapeData() const override
            {
                return getShape() != nullptr;
            }

            /** @copydoc IPhysicsShape::isTrigger */
            virtual bool isTrigger() const override
            {
                return m_isTrigger;
            }

            /** @copydoc IPhysicsShape::setTrigger */
            virtual void setTrigger( bool trigger ) override
            {
                m_isTrigger = trigger;
            }

            /** */
            virtual SmartPtr<IPhysicsShape3> clone()
            {
                return nullptr;
            }

            Array<SmartPtr<ISharedObject>> getChildObjects() const override
            {
                Array<SmartPtr<ISharedObject>> objects;
                objects.reserve( 10 );

                objects.push_back( m_body );
                objects.push_back( m_material );
                return objects;
            }

            virtual SmartPtr<Properties> getProperties() const
            {
                try
                {
                    auto properties = fb::make_ptr<Properties>();
                    properties->setProperty( "isTrigger", m_isTrigger );
                    return properties;
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }

                return nullptr;
            }

            virtual void setProperties( SmartPtr<Properties> properties )
            {
                try
                {
                    properties->getPropertyValue( "isTrigger", m_isTrigger );
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            SmartPtr<IStateContext> getStateContext() const
            {
                return m_stateContext;
            }

            void setStateContext( SmartPtr<IStateContext> stateContext )
            {
                m_stateContext = stateContext;
            }

            SmartPtr<IStateListener> getStateListener() const
            {
                return m_stateListener;
            }

            void setStateListener( SmartPtr<IStateListener> stateListener )
            {
                m_stateListener = stateListener;
            }

            physx::PxRigidActor *getPxActor() const
            {
                return m_pxActor;
            }

            void setPxActor( physx::PxRigidActor *pxActor )
            {
                m_pxActor = pxActor;
            }

            /** @copydoc IPhysicsShape::setEnabled */
            virtual void setEnabled( bool enabled )
            {
            }

            /** @copydoc IPhysicsShape::setEnabled */
            virtual bool isEnabled() const
            {
                return false;
            }

            bool isValid() const override
            {
                return true;
            }

            virtual void handleStateChanged( SmartPtr<IState> &state )
            {
            }

            FB_CLASS_REGISTER_TEMPLATE_DECL( PhysxShape, T );

        protected:
            class ShapeStateListener : public IStateListener
            {
            public:
                ShapeStateListener() = default;
                ~ShapeStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override
                {
                    if( auto owner = getOwner() )
                    {
                        auto type = message->getType();
                        if( type == physics::IPhysicsShape::CREATE_SHAPE_HASH )
                        {
                            owner->createShape();
                        }
                    }
                }

                void handleStateChanged( SmartPtr<IState> &state ) override
                {
                }

                void handleQuery( SmartPtr<IStateQuery> &query ) override
                {
                }

                PhysxShape<T> *getOwner() const
                {
                    return m_owner;
                }

                void setOwner( PhysxShape<T> *owner )
                {
                    m_owner = owner;
                }

            protected:
                PhysxShape<T> *m_owner = nullptr;
            };

            virtual void createStateObject()
            {
            }

            virtual void createShape();

            FilterData m_filterData;
            SmartPtr<IPhysicsBody3> m_body;
            SmartPtr<IPhysicsMaterial3> m_material;
            AtomicRawPtr<physx::PxShape> m_shape;

            SmartPtr<IStateContext> m_stateContext;
            SmartPtr<IStateListener> m_stateListener;

            physx::PxRigidActor *m_pxActor = nullptr;

            bool m_isTrigger = false;
        };

        template <class T>
        void PhysxShape<T>::unload( SmartPtr<ISharedObject> data )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager =
                    fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
                FB_ASSERT( physicsManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto physics = physicsManager->getPhysics();
                FB_ASSERT( physics );

                if( auto shape = getShape() )
                {
                    if( auto actor = getPxActor() )
                    {
                        actor->detachShape( *shape, false );
                    }

                    shape->release();
                    setShape( nullptr );
                }

                PhysxSharedObject<T>::unload( data );
            }

        template <class T>
        void PhysxShape<T>::setLocalPose( const Transform3<real_Num> &pose )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<ShapeState>() )
                {
                    state->setLocalPose( pose );
                }
            }
        }

        template <class T>
        Transform3<real_Num> PhysxShape<T>::getLocalPose() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<ShapeState>() )
                {
                    return state->getLocalPose();
                }
            }

            return Transform3<real_Num>();
        }

        template <class T>
        void PhysxShape<T>::createShape()
        {
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, PhysxShape, T, T );

    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxShape_h__
