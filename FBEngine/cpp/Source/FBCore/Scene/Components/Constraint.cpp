#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Constraint.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/IConstraintD6.h>
#include <FBCore/Interface/Physics/IConstraintFixed3.h>
#include <FBCore/Interface/Physics/IConstraintLinearLimit.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IRigidDynamic3.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITransformNode.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Constraint, Component );
        const String Constraint::constraints[4] = { "6dof", "6dofSpring", " ", " " };
        const hash_type Constraint::STIFFNESS = StringUtil::getHash( "stiffness" );
        const hash_type Constraint::SET_BREAK_FORCE_HASH = StringUtil::getHash( "SET_BREAK_FORCE_HASH" );

        Constraint::~Constraint()
        {
            Constraint::unload( nullptr );
        }

        void Constraint::reset( const Vector3<real_Num> &loc )
        {
            try
            {
                makeUnbreakable( true );

                if( isMotor() )
                {
                    if( auto d6joint = getD6Joint() )
                    {
                        auto pose = Transform3<real_Num>();
                        d6joint->setDrivePosition( pose );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::restore()
        {
            if( auto d6joint = getD6Joint() )
            {
                auto constraintType = getConstraintType();
                setConstraintType( constraintType, tLimits, aLimits );

                restoreFlags();
            }
        }

        void Constraint::setConstraintType( const String &constraintType, Vector3<real_Num> &tLimits,
                                            Vector3<real_Num> &aLimits )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto physicsManager = applicationManager->getPhysicsManager();

            FB_ASSERT( !StringUtil::isNullOrEmpty( constraintType ) );
            FB_ASSERT( MathUtil<real_Num>::isFinite( tLimits ) );
            FB_ASSERT( MathUtil<real_Num>::isFinite( aLimits ) );

            if( auto d6joint = getD6Joint() )
            {
                FB_ASSERT( constraintType.size() >= 6 );

                auto transX = constraintType[0];
                auto transY = constraintType[1];
                auto transZ = constraintType[2];

                auto rotX = constraintType[3];
                auto rotY = constraintType[4];
                auto rotZ = constraintType[5];

                switch( transX )
                {
                case 'L':
                    d6joint->setMotion( physics::D6Axis::eX, physics::D6Motion::eLOCKED );
                    break;
                case 'F':
                    d6joint->setMotion( physics::D6Axis::eX, physics::D6Motion::eFREE );
                    break;
                case 'R':
                {
                    d6joint->setMotion( physics::D6Axis::eX, physics::D6Motion::eLIMITED );
                    d6joint->setLinearLimit(
                        physicsManager->createConstraintLinearLimit( tLimits.x, tLimits.y ) );
                }
                break;
                }

                switch( transY )
                {
                case 'L':
                    d6joint->setMotion( physics::D6Axis::eY, physics::D6Motion::eLOCKED );
                    break;
                case 'F':
                    d6joint->setMotion( physics::D6Axis::eY, physics::D6Motion::eFREE );
                    break;
                case 'R':
                    d6joint->setMotion( physics::D6Axis::eY, physics::D6Motion::eLIMITED );
                    d6joint->setLinearLimit(
                        physicsManager->createConstraintLinearLimit( tLimits.x, tLimits.y ) );
                    break;
                }

                switch( transZ )
                {
                case 'L':
                    d6joint->setMotion( physics::D6Axis::eZ, physics::D6Motion::eLOCKED );
                    break;
                case 'F':
                    d6joint->setMotion( physics::D6Axis::eZ, physics::D6Motion::eFREE );
                    break;
                case 'R':
                    d6joint->setMotion( physics::D6Axis::eZ, physics::D6Motion::eLIMITED );
                    d6joint->setLinearLimit(
                        physicsManager->createConstraintLinearLimit( tLimits.x, tLimits.y ) );
                    break;
                }

                switch( constraintType[3] )
                {
                case 'L':
                    d6joint->setMotion( physics::D6Axis::eTWIST, physics::D6Motion::eLOCKED );
                    break;
                case 'F':
                    d6joint->setMotion( physics::D6Axis::eTWIST, physics::D6Motion::eFREE );
                    break;
                case 'R':
                {
                }
                break;
                }                
            }
        }

        void Constraint::forceBreak()
        {
            if( auto d6joint = getD6Joint() )
            {
                d6joint->unload( nullptr );
            }
        }

        void Constraint::resetPositions()
        {
            auto localPose0 = getLocalPose0();
            auto localPose1 = getLocalPose1();

            auto d6joint = getD6Joint();
            if( d6joint )
            {
                d6joint->setLocalPose( physics::JointActorIndex::eACTOR0, localPose0 );
                d6joint->setLocalPose( physics::JointActorIndex::eACTOR1, localPose1 );
            }

            auto fixedJoint = getFixedJoint();
            if( fixedJoint )
            {
                fixedJoint->setLocalPose( physics::JointActorIndex::eACTOR0, localPose0 );
                fixedJoint->setLocalPose( physics::JointActorIndex::eACTOR1, localPose1 );
            }
        }

        void Constraint::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                auto bodyA = getBodyA();
                auto bodyB = getBodyB();

                Transform3<real_Num> transformA;
                Transform3<real_Num> transformB;

                m_positionJoint = physicsManager->d6JointCreate(
                    bodyA ? bodyA->getRigidDynamic() : nullptr, transformA,
                    bodyB ? bodyB->getRigidDynamic() : nullptr, transformB );

                m_positionJoint->setMotion( physics::D6Axis::eX, physics::D6Motion::eLOCKED );
                m_positionJoint->setMotion( physics::D6Axis::eY, physics::D6Motion::eFREE );
                m_positionJoint->setMotion( physics::D6Axis::eZ, physics::D6Motion::eLOCKED );

                m_positionJoint->setMotion( physics::D6Axis::eSWING1, physics::D6Motion::eLOCKED );
                m_positionJoint->setMotion( physics::D6Axis::eSWING2, physics::D6Motion::eLOCKED );
                m_positionJoint->setMotion( physics::D6Axis::eTWIST, physics::D6Motion::eFREE );

                constexpr auto maxForce = std::numeric_limits<f32>::max();
                m_positionJoint->setBreakForce( maxForce, maxForce );

                m_positionJoint->setConstraintFlag( physics::ConstraintFlag::eVISUALIZATION, true );

                m_body0 = bodyA;
                m_body1 = bodyB;

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

                auto applicationManager = core::IApplicationManager::instance();
                auto taskManager = applicationManager->getTaskManager();
                if( !taskManager )
                {
                    return;
                }

                auto scene = getScene();

                if( m_d6joint )
                {
                    m_d6joint->unload( nullptr );
                    m_d6joint = nullptr;
                }

                if( m_positionJoint )
                {
                    m_positionJoint->unload( nullptr );
                    m_positionJoint = nullptr;
                }

                if( m_fixedJoint )
                {
                    m_fixedJoint->unload( nullptr );
                    m_fixedJoint = nullptr;
                }

                if( m_body0 )
                {
                    // m_bodyA->removeConstraint(this);
                    m_body0 = nullptr;
                }

                if( m_body1 )
                {
                    // m_bodyB->removeConstraint(this);
                    m_body1 = nullptr;
                }

                m_actor0 = nullptr;
                m_actor1 = nullptr;
                m_d6joint = nullptr;
                m_positionJoint = nullptr;
                m_fixedJoint = nullptr;
                m_body0 = nullptr;
                m_body1 = nullptr;

                m_motorDirection = 0;
                m_rotate = 0;
                m_pos = 0;
                m_breakForce = 0;
                m_breakTorque = 0;

                setChannel( -1 );
                m_effectNameHash = 0;

                m_isMotor = false;
                setFlag( false );
                m_linear = false;
                m_bServoIsNone = true;

                m_makeBreakable = nullptr;
                m_makeUnbreakable = nullptr;

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::setupDrive()
        {
            Transform3<real_Num> position;

            position = m_d6joint->getDrivePosition();
        }

        SmartPtr<Properties> Constraint::getProperties() const
        {
            if( auto properties = Component::getProperties() )
            {
                auto mass = 0.0f;
                properties->setProperty( "mass", mass );

                auto bodyHashA = 0;
                properties->setProperty( "bodyA", bodyHashA );

                auto bodyHashB = 0;
                properties->setProperty( "bodyB", bodyHashB );

                return properties;
            }

            return nullptr;
        }

        void Constraint::setProperties( SmartPtr<Properties> properties )
        {
        }

        Array<SmartPtr<ISharedObject>> Constraint::getChildObjects() const
        {
            auto objects = Component::getChildObjects();

            if( m_body0 )
            {
                objects.push_back( m_body0 );
            }

            if( m_body1 )
            {
                objects.push_back( m_body1 );
            }

            return objects;
        }

        Constraint::Type Constraint::getType() const
        {
            return m_type;
        }

        void Constraint::setType( Type type )
        {
            m_type = type;
        }

        void Constraint::create( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB,
                                 const Transform3<real_Num> &transformA,
                                 const Transform3<real_Num> &transformB )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto physics = applicationManager->getPhysicsManager();

            SmartPtr<physics::IPhysicsBody3> pBodyA;
            SmartPtr<physics::IPhysicsBody3> pBodyB;

            Transform3<real_Num> tA;
            Transform3<real_Num> tB;

            m_positionJoint =
                physics->d6JointCreate( bodyA ? bodyA->getRigidDynamic() : nullptr, transformA,
                                        bodyB ? bodyB->getRigidDynamic() : nullptr, transformB );

            m_positionJoint->setMotion( physics::D6Axis::eX, physics::D6Motion::eLOCKED );
            m_positionJoint->setMotion( physics::D6Axis::eY, physics::D6Motion::eFREE );
            m_positionJoint->setMotion( physics::D6Axis::eZ, physics::D6Motion::eLOCKED );

            m_positionJoint->setMotion( physics::D6Axis::eSWING1, physics::D6Motion::eLOCKED );
            m_positionJoint->setMotion( physics::D6Axis::eSWING2, physics::D6Motion::eLOCKED );
            m_positionJoint->setMotion( physics::D6Axis::eTWIST, physics::D6Motion::eFREE );

            auto maxForce = std::numeric_limits<f32>::max();
            m_positionJoint->setBreakForce( maxForce, maxForce );

            m_positionJoint->setConstraintFlag( physics::ConstraintFlag::eVISUALIZATION, true );

            m_body0 = bodyA;
            m_body1 = bodyB;
        }

        void Constraint::setupSuspension( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB )
        {
        }

        void Constraint::setupConstraint( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB )
        {
        }

        s32 Constraint::lookup( String shape )
        {
            return 0;
        }

        void Constraint::preUpdate()
        {
        }

        void Constraint::setDrivePosition( f32 position )
        {
            FB_ASSERT( MathF::isFinite( position ) );
            FB_ASSERT( Thread::getCurrentTask() != Thread::Task::Primary );

            SmartPtr<physics::IPhysicsScene3> scene = getScene();

            f32 rotate = 0.005f;
            Quaternion<real_Num> orietation;
            orietation.fromAngleAxis( rotate, Vector3<real_Num>::UNIT_X );

            if( m_actor0 )
            {
                m_actor0->wakeUp();
            }

            if( m_actor1 )
            {
                m_actor1->wakeUp();
            }

            Transform3<real_Num> pose = Transform3<real_Num>(
                Quaternion<real_Num>( orietation.X(), orietation.Y(), orietation.Z(), orietation.W() ) );
            m_d6joint->setDrivePosition( pose );
        }

        void Constraint::setBodyA( SmartPtr<Rigidbody> val )
        {
            m_body0 = val;
        }

        void Constraint::setBodyB( SmartPtr<Rigidbody> val )
        {
            m_body1 = val;
        }

        void Constraint::setBreakForce( f32 force, f32 torque )
        {
        }

        void Constraint::makeBroken()
        {
            setBrokenState( true );
        }

        void Constraint::makeBreakable( bool bNow )
        {
        }

        void Constraint::makeUnbreakable( bool bNow )
        {
        }

        void Constraint::setBrokenState( bool bBroken )
        {
        }
        
        void Constraint::setBreakState( bool state )
        {
        }

        SmartPtr<physics::IConstraintD6> Constraint::getD6Joint()
        {
            return m_d6joint;
        }

        void Constraint::setOriginalBreakForce( f32 breakForce )
        {
            m_breakForce = breakForce;
        }

        void Constraint::setOriginalBreakTorque( f32 breakTorque )
        {
            m_breakTorque = breakTorque;
        }

        bool Constraint::isBroken() const
        {
            if( m_d6joint )
            {
                if( ( m_d6joint->getConstraintFlags() & physics::ConstraintFlag::eBROKEN ) != 0 )
                {
                    return true;
                }
            }

            return false;
        }

        void Constraint::restoreFlags()
        {
            SmartPtr<Rigidbody> rbA = getBodyA();
        }

        void Constraint::setLocalPosition( physics::JointActorIndex::Enum actor,
                                           const Vector3<real_Num> &pos )
        {
        }

        Transform3<real_Num> Constraint::getLocalPose( physics::JointActorIndex::Enum actor ) const
        {
            return Transform3<real_Num>();
        }

        void Constraint::setLocalPose( physics::JointActorIndex::Enum actor,
                                       const Transform3<real_Num> &localPose )
        {
            auto physicsScene = getScene();

            if( m_d6joint )
            {
                m_d6joint->setLocalPose( actor, localPose );
            }

            if( m_positionJoint )
            {
                m_positionJoint->setLocalPose( actor, localPose );
            }

            if( m_fixedJoint )
            {
                m_fixedJoint->setLocalPose( actor, localPose );
            }
        }

        SmartPtr<physics::IPhysicsScene3> Constraint::getScene() const
        {
            SmartPtr<physics::IPhysicsScene3> scene = nullptr;
            if( m_body0 && m_body0->getScene() )
            {
                scene = m_body0->getScene();
            }
            else if( m_body1 && m_body1->getScene() )
            {
                scene = m_body1->getScene();
            }

            return nullptr;
        }

        void Constraint::setMotion( physics::D6Axis::Enum axis, physics::D6Motion::Enum type )
        {
            if( m_positionJoint )
            {
                m_positionJoint->setMotion( axis, type );
            }
        }

        void Constraint::updateComponentState()
        {
        }

        void Constraint::setServoHash( s32 val )
        {
            m_servoHash = val;
        }

        SmartPtr<physics::IConstraintFixed3> Constraint::getFixedJoint() const
        {
            return m_fixedJoint;
        }

        void Constraint::setFixedJoint( SmartPtr<physics::IConstraintFixed3> val )
        {
            m_fixedJoint = val;
        }

        bool Constraint::isMotor() const
        {
            return m_isMotor;
        }

        void Constraint::setMotor( bool motor )
        {
            m_isMotor = motor;
        }

        SmartPtr<ITransformNode> Constraint::getAnchor0() const
        {
            return m_anchor0;
        }

        void Constraint::setAnchor0( SmartPtr<ITransformNode> val )
        {
            m_anchor0 = val;
        }

        SmartPtr<ITransformNode> Constraint::getAnchor1() const
        {
            return m_anchor1;
        }

        void Constraint::setAnchor1( SmartPtr<ITransformNode> val )
        {
            m_anchor1 = val;
        }

        SmartPtr<ITransformNode> Constraint::getRotatingGraphicsNode() const
        {
            return m_rotatingGraphicsNode;
        }

        Constraint::MakeBreakable::~MakeBreakable()
        {
        }

        Constraint::MakeBreakable::MakeBreakable( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::MakeBreakable::MakeBreakable()
        {
        }

        void Constraint::MakeBreakable::execute()
        {
            if( m_constraint )
            {
                f32 fBreakForce = m_constraint->getOriginalBreakForce();
                f32 fBreakTorque = m_constraint->getOriginalBreakTorque();
                m_constraint->setBreakForce( fBreakForce, fBreakTorque );
            }
        }

        void Constraint::MakeBreakable::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
        }

        String Constraint::getConstraintType() const
        {
            return m_constraintType;
        }

        String Constraint::getClfSenenode() const
        {
            return m_clf_scenenode;
        }

        String Constraint::getServo() const
        {
            return m_servo;
        }

        f64 Constraint::timeSinceBreakable() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();
            return timer->now() - m_breakableTime;
        }

        void Constraint::setRotatingGraphicsNode( SmartPtr<ITransformNode> val )
        {
            m_rotatingGraphicsNode = val;
        }

        String Constraint::getRotorName() const
        {
            return m_rotorName;
        }

        void Constraint::setRotorName( const String &val )
        {
            m_rotorName = val;
        }

        void Constraint::setChannel( s32 val )
        {
            m_channel = val;
        }

        bool Constraint::getFlag() const
        {
            return m_flag;
        }

        void Constraint::setFlag( bool val )
        {
            m_flag = val;
        }

        bool Constraint::getEnableProjection() const
        {
            return m_enableProjection;
        }

        void Constraint::setEnableProjection( bool val )
        {
            m_enableProjection = val;
        }

        Constraint::MakeUnbreakable::MakeUnbreakable( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::MakeUnbreakable::MakeUnbreakable()
        {
        }

        Constraint::MakeUnbreakable::~MakeUnbreakable()
        {
        }

        void Constraint::MakeUnbreakable::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            while( timer->now() > ( m_constraint->m_breakableTime + 1.0 ) )
            {
                Thread::yield();
            }
        }

        void Constraint::MakeUnbreakable::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();
        }

        Constraint::FlagSetJob::FlagSetJob( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::FlagSetJob::FlagSetJob()
        {
        }

        Constraint::FlagSetJob::~FlagSetJob()
        {
        }

        void Constraint::FlagSetJob::execute()
        {
            Thread::sleep( 3.0 );
        }

        void Constraint::FlagSetJob::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            SmartPtr<Constraint> constraint = getConstraint();
            if( !constraint )
            {
                rYield->stop();
                return;
            }

            if( constraint )
            {
                if( constraint->getLoadingState() != LoadingState::Loaded )
                {
                    rYield->stop();
                }
                }
        }

        physics::ConstraintFlag::Enum Constraint::FlagSetJob::getFlag() const
        {
            return m_flag;
        }

        void Constraint::FlagSetJob::setFlag( physics::ConstraintFlag::Enum val )
        {
            m_flag = val;
        }

        bool Constraint::FlagSetJob::getFlagValue() const
        {
            return m_flagValue;
        }

        void Constraint::FlagSetJob::setFlagValue( bool val )
        {
            m_flagValue = val;
        }

        SmartPtr<Constraint> Constraint::FlagSetJob::getConstraint() const
        {
            return m_constraint;
        }

        void Constraint::FlagSetJob::setConstraint( SmartPtr<Constraint> val )
        {
            m_constraint = val;
        }

        Transform3<real_Num> Constraint::getLocalPose0() const
        {
            return m_localPose0;
        }

        Transform3<real_Num> Constraint::getLocalPose1() const
        {
            return m_localPose1;
        }

        void Constraint::setLocalPose0( Transform3<real_Num> t )
        {
            m_localPose0 = t;
        }

        void Constraint::setLocalPose1( Transform3<real_Num> t )
        {
            m_localPose1 = t;
        }

        Transform3<real_Num> Constraint::getLocalFrame1() const
        {
            return m_localFrame1;
        }

        void Constraint::setLocalFrame1( Transform3<real_Num> t )
        {
            m_localFrame1 = t;
        }

        Transform3<real_Num> Constraint::getLocalFrame0() const
        {
            return m_localFrame0;
        }

        void Constraint::setLocalFrame0( Transform3<real_Num> t )
        {
            m_localFrame0 = t;
        }

        Constraint::SetupLocalPose::SetupLocalPose()
        {
        }

        Constraint::SetupLocalPose::SetupLocalPose( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::SetupLocalPose::~SetupLocalPose()
        {
        }

        void Constraint::SetupLocalPose::execute()
        {
        }

        void Constraint::SetupLocalPose::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            SmartPtr<Constraint> constraint = m_constraint;
            if( !constraint )
            {
                rYield->stop();
                return;
            }

            if( constraint )
            {
                if( constraint->getLoadingState() != LoadingState::Loaded )
                {
                    rYield->stop();
                }
            }
        }

        SmartPtr<Rigidbody> Constraint::getBodyA() const
        {
            return m_body0;
        }

        SmartPtr<Rigidbody> Constraint::getBodyB() const
        {
            return m_body1;
        }

        s32 Constraint::getServoHash() const
        {
            return m_servoHash;
        }

        f32 Constraint::getOriginalBreakForce() const
        {
            return m_breakForce;
        }

        f32 Constraint::getOriginalBreakTorque() const
        {
            return m_breakTorque;
        }

        s32 Constraint::getChannel() const
        {
            return m_channel;
        }

    }  // namespace scene
}  // end namespace fb
