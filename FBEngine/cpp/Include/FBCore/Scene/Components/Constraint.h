#ifndef Constraint_h__
#define Constraint_h__

#include <FBCore/Scene/Components/BaseComponent.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>
#include <FBCore/System/Job.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace scene
    {

        class Constraint : public BaseComponent
        {
        public:
            enum class Type
            {
                D6,
                Fixed,

                Count
            };

            class FlagSetJob : public Job
            {
            public:
                FlagSetJob();
                FlagSetJob( SmartPtr<Constraint> constraint );
                ~FlagSetJob() override;

                void execute() override;
                void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

                physics::PxConstraintFlag::Enum getFlag() const;
                void setFlag( physics::PxConstraintFlag::Enum val );

                bool getFlagValue() const;
                void setFlagValue( bool val );

                SmartPtr<Constraint> getConstraint() const;
                void setConstraint( SmartPtr<Constraint> val );

            private:
                physics::PxConstraintFlag::Enum m_flag =
                    static_cast<physics::PxConstraintFlag::Enum>( 0 );
                SmartPtr<Constraint> m_constraint = nullptr;
                bool m_flagValue = false;
            };

            class SetupLocalPose : public Job
            {
            public:
                SetupLocalPose();
                SetupLocalPose( SmartPtr<Constraint> constraint );
                ~SetupLocalPose() override;

                void execute() override;
                void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

                SmartPtr<Constraint> m_constraint = nullptr;
            };

            class MakeBreakable : public Job
            {
            public:
                MakeBreakable();
                MakeBreakable( SmartPtr<Constraint> constraint );
                ~MakeBreakable() override;

                void execute() override;
                void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

                SmartPtr<Constraint> m_constraint = nullptr;
            };

            class MakeUnbreakable : public Job
            {
            public:
                MakeUnbreakable();
                MakeUnbreakable( SmartPtr<Constraint> constraint );
                ~MakeUnbreakable() override;

                void execute() override;
                void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

                SmartPtr<Constraint> m_constraint = nullptr;
            };

            static const int STIFFNESS;
            static const u32 SET_BREAK_FORCE_HASH;

            Constraint() = default;
            ~Constraint() override;

            void initialise( const String &objectID );
            void initialise( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB );
            void setupSuspension( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB );

            void preUpdate() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void create( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB,
                         const Transform3<real_Num> &transformA,
                         const Transform3<real_Num> &transformB );

            void setDrivePosition( float position );

            void resetPositions();

            Vector3<real_Num> getRelativeAngles();

            void reset( const Vector3<real_Num> &position );
            void restore();

            SmartPtr<Rigidbody> &getBodyA();
            const SmartPtr<Rigidbody> &getBodyA() const;
            void setBodyA( SmartPtr<Rigidbody> val );

            SmartPtr<Rigidbody> &getBodyB();
            const SmartPtr<Rigidbody> &getBodyB() const;
            void setBodyB( SmartPtr<Rigidbody> val );

            SmartPtr<physics::IConstraintD6> getD6Joint();

            void setBreakForce( float force, float torque );

            void makeBroken();
            void makeBreakable( bool bNow );
            void makeUnbreakable( bool bNow );

            void setBrokenState( bool bBroken );

            void enterFlightState();
            void enterCrashState();
            void leaveCrashState();

            void updateFlightState();
            void leaveFlightState();

            void enterWorkbenchState();
            void updateWorkbenchState();
            void leaveWorkbenchState();

            void enterDestroyedState();
            void updateDestroyedState();
            void leaveDestroyedState();

            void enterResetState();
            void updateResetState();
            void leaveResetState();

            void setBreakState( bool state );

            // particle effect to fire if this join breaks
            u32 getEffectNameHash();

            float getOriginalBreakForce() const;
            float getOriginalBreakTorque() const;

            void setOriginalBreakForce( f32 breakForce );
            void setOriginalBreakTorque( f32 breakTorque );

            bool isBroken() const;

            void setDrive( physics::PxD6Drive::Enum index, SmartPtr<physics::IConstraintDrive> drive );

            void setLocalPosition( physics::PxJointActorIndex::Enum actor,
                                   const Vector3<real_Num> &pos );

            Transform3<real_Num> getLocalPose( physics::PxJointActorIndex::Enum actor ) const;
            void setLocalPose( physics::PxJointActorIndex::Enum actor,
                               const Transform3<real_Num> &localPose );

            SmartPtr<physics::IPhysicsScene3> getScene() const;

            void setMotion( physics::PxD6Axis::Enum axis, physics::PxD6Motion::Enum type );

            bool isControlComponent() const;

            void updateComponentState();

            int getServoHash() const;
            void setServoHash( int val );

            SmartPtr<physics::IConstraintFixed3> getFixedJoint() const;
            void setFixedJoint( SmartPtr<physics::IConstraintFixed3> val );

            bool isMotor() const;
            void setMotor( bool val );

            SmartPtr<ITransformNode> getAnchor0() const;
            void setAnchor0( SmartPtr<ITransformNode> val );

            SmartPtr<ITransformNode> getAnchor1() const;
            void setAnchor1( SmartPtr<ITransformNode> val );

            SmartPtr<ITransformNode> getRotatingGraphicsNode() const;
            void setRotatingGraphicsNode( SmartPtr<ITransformNode> val );

            String getRotorName() const;
            void setRotorName( const String &val );

            int getChannel() const;
            void setChannel( int val );

            bool getFlag() const;
            void setFlag( bool val );

            bool getEnableProjection() const;
            void setEnableProjection( bool val );

            void forceBreak();

            void setupUpdateRegistration() override;

            void setupDrive();

            /** @copydoc IComponent::awake */
            void awake() override;

            /** @copydoc IComponent::start */
            void play();

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            Type getType() const;
            void setType( Type val );

            FB_CLASS_REGISTER_DECL;

        protected:
            void setConstraintType( const String &constraintType, Vector3<real_Num> &tLimits,
                                    Vector3<real_Num> &aLimits );

            void setupRotor();

            int lookup( String shape );

            void updateDrive();
            void updateDriveEditor();

            void restoreFlags();

            f64 timeSinceBreakable() const;

            String getConstraintType() const;
            String getClfSenenode() const;
            String getServo() const;

            Transform3<real_Num> getLocalPose0() const;
            void setLocalPose0( Transform3<real_Num> t );

            Transform3<real_Num> getLocalPose1() const;
            void setLocalPose1( Transform3<real_Num> t );

            Transform3<real_Num> getLocalFrame1() const;
            void setLocalFrame1( Transform3<real_Num> localFrame1 );

            Transform3<real_Num> getLocalFrame0() const;
            void setLocalFrame0( Transform3<real_Num> localFrame0 );

            Vector3<real_Num> aLimits = Vector3<real_Num>( 0.0f, 0.0f, 0.0f );
            Vector3<real_Num> tLimits = Vector3<real_Num>( 0.0f, 0.0f, 0.0f );

            SmartPtr<ITransformNode> m_anchor0;
            SmartPtr<ITransformNode> m_anchor1;

            SmartPtr<MakeBreakable> m_makeBreakable;
            SmartPtr<MakeUnbreakable> m_makeUnbreakable;

            SmartPtr<ITransformNode> m_rotatingGraphicsNode;

            SmartPtr<physics::IRigidBody3> m_actor0;
            SmartPtr<physics::IRigidBody3> m_actor1;

            SmartPtr<physics::IConstraintD6> m_d6joint;
            SmartPtr<physics::IConstraintD6> m_positionJoint;
            SmartPtr<physics::IConstraintFixed3> m_fixedJoint;

            SmartPtr<Rigidbody> m_bodyA;
            SmartPtr<Rigidbody> m_bodyB;

            Transform3<real_Num> m_localPose0;
            Transform3<real_Num> m_localPose1;

            Transform3<real_Num> m_localFrame0;
            Transform3<real_Num> m_localFrame1;

            atomic_f64 m_breakableTime = 0.0;
            atomic_f32 m_motorDirection = 0.0f;
            atomic_f64 m_rotate = 0.0;
            atomic_f64 m_pos = 0.0;

            atomic_f32 m_breakForce = std::numeric_limits<f32>::max();
            atomic_f32 m_breakTorque = std::numeric_limits<f32>::max();

            atomic_f32 m_twist = 0.0f;
            atomic_f32 m_swingYAngle = 0.0f;
            atomic_f32 m_swingZAngle = 0.0f;

            atomic_f32 m_driveForce = 0.0f;
            atomic_f32 m_driveDamping = 0.0f;

            atomic_s32 m_servoNoneCompare = 0;
            atomic_s32 m_servoHash = 0;
            atomic_u32 m_effectNameHash = 0;
            atomic_u32 m_effectNameNightHash = 0;
            atomic_s32 m_channel = -1;

            atomic_s32 m_breakableCount = 0;

            atomic_bool m_isMotor = false;
            atomic_bool m_flag = false;
            atomic_bool m_linear = false;
            atomic_bool m_bServoIsNone = true;
            atomic_bool m_reportForce = false;
            atomic_bool m_enableProjection = true;

            Type m_type = Type::D6;

            String m_servo = "";
            String m_constraintType = "";
            String m_clf_scenenode = "";
            String m_rotorName = "";
            String actor0 = "";
            String actor1 = "";
            String anchor0 = "";
            String anchor1 = "";
            String anchor1_id = "";
            String drive_axis = "";
            String sceneNodeName = "";

            // lookup for constraint types types
            static const String constraints[4];
        };
    }  // namespace scene
}  // end namespace fb

#endif  // Constraint_h__
