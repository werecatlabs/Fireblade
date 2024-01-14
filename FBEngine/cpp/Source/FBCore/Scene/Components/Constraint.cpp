#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Constraint.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/IConstraintD6.h>
#include <FBCore/Interface/Physics/IConstraintFixed3.h>
#include <FBCore/Interface/Physics/IConstraintLinearLimit.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IRigidDynamic3.h>
#include <FBCore/Interface/Physics/IRigidStatic3.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Constraint, Component );

    Constraint::~Constraint()
    {
        Constraint::unload( nullptr );
    }

    void Constraint::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Component::load( data );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Constraint::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            unload( data );
            load( data );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Constraint::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            auto physicsManager = applicationManager->getPhysicsManager();

            Component::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Constraint::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = Component::getProperties() )
        {
            auto mass = 0.0f;
            properties->setProperty( "mass", mass );

            properties->setProperty( "bodyA", fb::static_pointer_cast<IComponent>( m_body0 ) );
            properties->setProperty( "bodyB", fb::static_pointer_cast<IComponent>( m_body1 ) );

            return properties;
        }

        return nullptr;
    }

    void Constraint::setProperties( SmartPtr<Properties> properties )
    {
        SmartPtr<IComponent> bodyA;
        SmartPtr<IComponent> bodyB;

        properties->getPropertyValue( "bodyA", bodyA );
        properties->getPropertyValue( "bodyB", bodyB );

        SmartPtr<IActor> actorA;
        SmartPtr<IActor> actorB;

        properties->getPropertyValue( "bodyA", actorA );
        properties->getPropertyValue( "bodyB", actorB );

        if( actorA )
        {
            auto body = actorA->getComponent<Rigidbody>();
            setBodyA( body );
        }
        else if( bodyA )
        {
            auto body = fb::static_pointer_cast<Rigidbody>( bodyA );
            setBodyA( body );
        }

        if( actorB )
        {
            auto body = actorB->getComponent<Rigidbody>();
            setBodyB( body );
        }
        else if( bodyB )
        {
            auto body = fb::static_pointer_cast<Rigidbody>( bodyB );
            setBodyB( body );
        }

        setupConstraint();
    }

    auto Constraint::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Component::getChildObjects();

        if( m_body0 )
        {
            objects.emplace_back( m_body0 );
        }

        if( m_body1 )
        {
            objects.emplace_back( m_body1 );
        }

        return objects;
    }

    auto Constraint::getType() const -> Constraint::Type
    {
        return m_type;
    }

    void Constraint::setType( Type type )
    {
        m_type = type;
    }

    auto Constraint::getConstraint() const -> SmartPtr<physics::IPhysicsConstraint3>
    {
        return m_constraint;
    }

    void Constraint::setConstraint( SmartPtr<physics::IPhysicsConstraint3> constraint )
    {
        m_constraint = constraint;
    }

    void Constraint::setBodyA( SmartPtr<Rigidbody> body )
    {
        if( m_body0 )
        {
            m_body0->removeConstraint( this );
        }

        m_body0 = body;

        if( m_body0 )
        {
            m_body0->addConstraint( this );
        }
    }

    void Constraint::setBodyB( SmartPtr<Rigidbody> body )
    {
        if( m_body1 )
        {
            m_body1->removeConstraint( this );
        }

        m_body1 = body;

        if( m_body1 )
        {
            m_body1->addConstraint( this );
        }
    }

    void Constraint::setupConstraint()
    {
        auto bodyA = getBodyA();
        auto bodyB = getBodyB();

        auto transformA = Transform3<real_Num>();
        auto transformB = Transform3<real_Num>();

        auto applicationManager = core::ApplicationManager::instance();
        auto physics = applicationManager->getPhysicsManager();

        SmartPtr<physics::IPhysicsBody3> pBodyA;
        SmartPtr<physics::IPhysicsBody3> pBodyB;

        Transform3<real_Num> tA;
        Transform3<real_Num> tB;

        auto constraint =
            physics->addConstraintD6( bodyA ? bodyA->getRigidDynamic() : nullptr, transformA,
                                      bodyB ? bodyB->getRigidDynamic() : nullptr, transformB );

        constraint->setMotion( physics::D6Axis::eX, physics::D6Motion::eLOCKED );
        constraint->setMotion( physics::D6Axis::eY, physics::D6Motion::eLOCKED );
        constraint->setMotion( physics::D6Axis::eZ, physics::D6Motion::eLOCKED );

        constraint->setMotion( physics::D6Axis::eSWING1, physics::D6Motion::eLOCKED );
        constraint->setMotion( physics::D6Axis::eSWING2, physics::D6Motion::eLOCKED );
        constraint->setMotion( physics::D6Axis::eTWIST, physics::D6Motion::eLOCKED );

        auto maxForce = std::numeric_limits<f32>::max();
        constraint->setBreakForce( maxForce, maxForce );

        constraint->setConstraintFlag( physics::ConstraintFlag::eVISUALIZATION, true );
        setConstraint( constraint );
    }

    auto Constraint::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        switch( eventType )
        {
        case IFSM::Event::Change:
        {
        }
        break;
        case IFSM::Event::Enter:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                updateConstraint();
            }
            break;
            case State::Destroyed:
            {
            }
            break;
            default:
            {
            }
            }
        }
        break;
        case IFSM::Event::Leave:
        {
        }
        break;
        case IFSM::Event::Pending:
        {
        }
        break;
        case IFSM::Event::Complete:
        {
        }
        break;
        case IFSM::Event::NewState:
        {
        }
        break;
        case IFSM::Event::WaitForChange:
        {
        }
        break;
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    void Constraint::updateConstraint()
    {
        auto bodyA = getBodyA();
        auto bodyB = getBodyB();

        if( auto constraint = getConstraint() )
        {
            if( bodyA )
            {
                if( auto body = bodyA->getRigidDynamic() )
                {
                    constraint->setBodyA( body );
                }

                if( auto body = bodyA->getRigidStatic() )
                {
                    constraint->setBodyA( body );
                }
            }

            if( bodyB )
            {
                if( auto body = bodyB->getRigidDynamic() )
                {
                    constraint->setBodyB( body );
                }

                if( auto body = bodyB->getRigidStatic() )
                {
                    constraint->setBodyB( body );
                }
            }
        }
    }

    auto Constraint::getBodyA() const -> SmartPtr<Rigidbody>
    {
        return m_body0;
    }

    auto Constraint::getBodyB() const -> SmartPtr<Rigidbody>
    {
        return m_body1;
    }
}  // namespace fb::scene
