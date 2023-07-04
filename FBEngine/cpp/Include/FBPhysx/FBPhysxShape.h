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
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateListener.h>

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

            void setLocalPose( const Transform3<real_Num> &pose ) override
            {
                m_localPose = pose;
            }

            Transform3<real_Num> getLocalPose() const override
            {
                return m_localPose;
            }

            void setSimulationFilterData( const PxFilterData &data ) override
            {
                m_filterData = data;
            }

            PxFilterData getSimulationFilterData() const override
            {
                return m_filterData;
            }

            void setActor( SmartPtr<IPhysicsBody3> body ) override
            {
                m_body = body;
            }

            SmartPtr<IPhysicsBody3> getActor() const override
            {
                return m_body;
            }

            physx::PxShape* getShape() const
            {
                auto p = m_shape.load();
                return p.get();
            }

            void setShape( physx::PxShape* shape )
            {
                m_shape = shape;
            }

            virtual bool hasShapeData() const override
            {
                return getShape() != nullptr;            
            }

            /** */
            virtual bool isTrigger() const override
            {
                return m_isTrigger;
            }

            /** */
            virtual void setTrigger( bool bIsTrigger ) override
            {
                m_isTrigger = bIsTrigger;
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

            SmartPtr<IStateContext> getStateObject() const
            {
                return m_stateObject;
            }

            void setStateObject( SmartPtr<IStateContext> stateObject )
            {
                m_stateObject = stateObject;
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

            FB_CLASS_REGISTER_TEMPLATE_DECL( PhysxShape, T );

        protected:
            class ShapeStateListener : public SharedObject<IStateListener>
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

            virtual void createShape()
            {
            }

            Transform3<real_Num> m_localPose;
            PxFilterData m_filterData;
            SmartPtr<IPhysicsBody3> m_body;
            SmartPtr<IPhysicsMaterial3> m_material;
            AtomicRawPtr<physx::PxShape> m_shape;

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;

            physx::PxRigidActor *m_pxActor = nullptr;

            bool m_isTrigger = false;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, PhysxShape, T, T );

    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxShape_h__
