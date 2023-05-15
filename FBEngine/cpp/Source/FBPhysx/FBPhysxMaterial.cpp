#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBCore/FBCore.h>
#include <PxMaterial.h>

namespace fb
{
    namespace physics
    {
        PhysxMaterial::PhysxMaterial()
        {
        }

        PhysxMaterial::~PhysxMaterial()
        {
        }

        void PhysxMaterial::setFriction( f32 friction, int direction )
        {
        }

        void PhysxMaterial::setRestitution( f32 restitution )
        {
        }

        Vector3<real_Num> PhysxMaterial::getContactPosition() const
        {
            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> PhysxMaterial::getContactNormal() const
        {
            return Vector3<real_Num>::zero();
        }

        SmartPtr<IRigidBody3> PhysxMaterial::getPhysicsBodyA() const
        {
            return nullptr;
        }

        SmartPtr<IRigidBody3> PhysxMaterial::getPhysicsBodyB() const
        {
            return nullptr;
        }

        RawPtr<physx::PxMaterial> PhysxMaterial::getMaterial() const
        {
            return m_material;
        }

        void PhysxMaterial::setMaterial( RawPtr<physx::PxMaterial> material )
        {
            m_material = material;
        }

        SmartPtr<Properties> PhysxMaterial::getProperties() const
        {
            try
            {
                auto properties = fb::make_ptr<Properties>();

                if(m_material)
                {
                    auto dynamicFriction = m_material->getDynamicFriction();
                    properties->setProperty( "dynamicFriction", dynamicFriction );

                    auto staticFriction = m_material->getStaticFriction();
                    properties->setProperty( "staticFriction", staticFriction );
                }

                return properties;
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void PhysxMaterial::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                if(m_material)
                {
                    auto dynamicFriction = m_material->getDynamicFriction();
                    properties->getPropertyValue( "dynamicFriction", dynamicFriction );
                    m_material->setDynamicFriction( dynamicFriction );

                    auto staticFriction = m_material->getStaticFriction();
                    properties->getPropertyValue( "staticFriction", staticFriction );
                    m_material->setStaticFriction( staticFriction );
                }
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    } // end namespace physics
}     // end namespace fb
