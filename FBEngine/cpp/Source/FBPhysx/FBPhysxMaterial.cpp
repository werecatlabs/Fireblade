#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBCore/FBCore.h>
#include <PxMaterial.h>

namespace fb::physics
{
    PhysxMaterial::PhysxMaterial() = default;

    PhysxMaterial::~PhysxMaterial() = default;

    void PhysxMaterial::setFriction( f32 friction, int direction )
    {
    }

    void PhysxMaterial::setRestitution( f32 restitution )
    {
    }

    auto PhysxMaterial::getContactPosition() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    auto PhysxMaterial::getContactNormal() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    auto PhysxMaterial::getPhysicsBodyA() const -> SmartPtr<IRigidBody3>
    {
        return nullptr;
    }

    auto PhysxMaterial::getPhysicsBodyB() const -> SmartPtr<IRigidBody3>
    {
        return nullptr;
    }

    auto PhysxMaterial::getMaterial() const -> RawPtr<physx::PxMaterial>
    {
        return m_material;
    }

    void PhysxMaterial::setMaterial( RawPtr<physx::PxMaterial> material )
    {
        m_material = material;
    }

    auto PhysxMaterial::getProperties() const -> SmartPtr<Properties>
    {
        try
        {
            auto properties = fb::make_ptr<Properties>();

            if( m_material )
            {
                auto dynamicFriction = m_material->getDynamicFriction();
                properties->setProperty( "dynamicFriction", dynamicFriction );

                auto staticFriction = m_material->getStaticFriction();
                properties->setProperty( "staticFriction", staticFriction );
            }

            return properties;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void PhysxMaterial::setProperties( SmartPtr<Properties> properties )
    {
        try
        {
            if( m_material )
            {
                auto dynamicFriction = m_material->getDynamicFriction();
                properties->getPropertyValue( "dynamicFriction", dynamicFriction );
                m_material->setDynamicFriction( dynamicFriction );

                auto staticFriction = m_material->getStaticFriction();
                properties->getPropertyValue( "staticFriction", staticFriction );
                m_material->setStaticFriction( staticFriction );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }
}  // namespace fb::physics
