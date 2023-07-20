#ifndef PhysicsTypes_h__
#define PhysicsTypes_h__

#include <FBCore/FBCorePrerequisites.h>

namespace fb
{
    namespace physics
    {

        class PhysicsTypes
        {
        public:
            //
            // Summary:
            //     Use ForceMode to specify how to apply a force using Rigidbody.AddForce.
            enum ForceMode
            {
                //
                // Summary:
                //     Add a continuous force to the rigidbody, using its mass.
                Force = 0,
                //
                // Summary:
                //     Add an instant force impulse to the rigidbody, using its mass.
                Impulse = 1,
                //
                // Summary:
                //     Add an instant velocity change to the rigidbody, ignoring its mass.
                VelocityChange = 2,
                //
                // Summary:
                //     Add a continuous acceleration to the rigidbody, ignoring its mass.
                Acceleration = 5
            };
        };

        struct ConstraintFlag
        {
            enum Enum
            {
                eBROKEN = 1 << 0,
                //!< whether the constraint is broken
                ePROJECT_TO_ACTOR0 = 1 << 1,
                //!< whether actor1 should get projected to actor0 for this
                //!< constraint (note: projection of a static/kinematic
                //!< actor to a dynamic actor will be ignored)
                ePROJECT_TO_ACTOR1 = 1 << 2,
                //!< whether actor0 should get projected to actor1 for this
                //!< constraint (note: projection of a static/kinematic
                //!< actor to a dynamic actor will be ignored)
                ePROJECTION = ePROJECT_TO_ACTOR0 | ePROJECT_TO_ACTOR1,
                //!< whether the actors should get projected for this constraint
                //!< (the direction will be chosen by PhysX)
                eCOLLISION_ENABLED = 1 << 3,
                //!< whether contacts should be generated between the
                //!< objects this constraint constrains
                eREPORTING = 1 << 4,
                //!< whether this constraint should generate force reports.
                //!< DEPRECATED, as constraints always generate reports
                eVISUALIZATION = 1 << 5,
                //!< whether this constraint should be visualized, if
                //!< constraint visualization is turned on
                eDRIVE_LIMITS_ARE_FORCES = 1 << 6,
                //!< limits for drive strength are forces rather than impulses
                eDEPRECATED_32_COMPATIBILITY = 1 << 7,
                //!< legacy compatibility flag for 3.3; see user guide. This flag must not
                //!< be set in order for drives to conform to an implicit spring model
                eIMPROVED_SLERP = 1 << 8
                //!< perform preprocessing for improved accuracy on D6 Slerp
                //!< Drive (this flag will be removed in a future release when
                //!< preprocessing is no longer required)
            };
        };

        struct D6Axis
        {
            enum Enum
            {
                eX = 0,
                //!< motion along the X axix
                eY = 1,
                //!< motion along the Y axis
                eZ = 2,
                //!< motion along the Z axis
                eTWIST = 3,
                //!< motion around the X axis
                eSWING1 = 4,
                //!< motion around the Y axis
                eSWING2 = 5,
                //!< motion around the Z axis
                eCOUNT = 6
            };
        };

        struct JointActorIndex
        {
            enum Enum
            {
                eACTOR0,
                eACTOR1,
                COUNT
            };
        };

        struct D6Drive
        {
            enum Enum
            {
                eX = 0,
                //!< drive along the X-axis
                eY = 1,
                //!< drive along the Y-axis
                eZ = 2,
                //!< drive along the Z-axis
                eSWING = 3,
                //!< drive of displacement from the X-axis
                eTWIST = 4,
                //!< drive of the displacement around the X-axis
                eSLERP = 5,
                //!< drive of all three angular degrees along a SLERP-path
                eCOUNT = 6
            };
        };

        struct D6Motion
        {
            enum Enum
            {
                eLOCKED,
                //!< The DOF is locked, it does not allow relative motion.
                eLIMITED,
                //!< The DOF is limited, it only allows motion within a specific range.
                eFREE  //!< The DOF is free and has its full range of motion.
            };
        };

        struct ActorFlag
        {
            enum Enum
            {
                /**
                \brief Enable debug renderer for this actor

                @see PxScene.getRenderBuffer() PxRenderBuffer PxVisualizationParameter
                */
                eVISUALIZATION = ( 1 << 0 ),

                /**
                \brief Disables scene gravity for this actor
                */
                eDISABLE_GRAVITY = ( 1 << 1 ),

                /**
                \brief Enables the sending of PxSimulationEventCallback::onWake() and
                PxSimulationEventCallback::onSleep() notify events

                @see PxSimulationEventCallback::onWake() PxSimulationEventCallback::onSleep()
                */
                eSEND_SLEEP_NOTIFIES = ( 1 << 2 ),

                /**
                \brief Disables simulation for the actor.

                \note This is only supported by PxRigidStatic and PxRigidDynamic actors and can be used
                to reduce the memory footprint when rigid actors are used for scene queries only.

                \note Setting this flag will remove all constraints attached to the actor from the scene.

                \note If this flag is set, the following calls are forbidden:
                \li PxRigidBody: setLinearVelocity(), setAngularVelocity(), addForce(), addTorque(),
                clearForce(), clearTorque() \li PxRigidDynamic: setKinematicTarget(), setWakeCounter(),
                wakeUp(), putToSleep()

                \par <b>Sleeping:</b>
                Raising this flag will set all velocities and the wake counter to 0, clear all forces,
                clear the kinematic target, put the actor to sleep and wake up all touching actors from
                the previous frame.
                */
                eDISABLE_SIMULATION = ( 1 << 3 )
            };
        };

        struct RigidBodyFlag
        {
            enum Enum
            {
                /**
                \brief Enables kinematic mode for the actor.

                Kinematic actors are special dynamic actors that are not
                influenced by forces (such as gravity), and have no momentum. They are considered to have
                infinite mass and can be moved around the world using the setKinematicTarget() method.
                They will push regular dynamic actors out of the way. Kinematics will not collide with
                static or other kinematic objects.

                Kinematic actors are great for moving platforms or characters, where direct motion
                control is desired.

                You can not connect Reduced joints to kinematic actors. Lagrange joints work ok if the
                platform is moving with a relatively low, uniform velocity.

                <b>Sleeping:</b>
                \li Setting this flag on a dynamic actor will put the actor to sleep and set the
                velocities to 0. \li If this flag gets cleared, the current sleep state of the actor will
                be kept.

                \note kinematic actors are incompatible with CCD so raising this flag will automatically
                clear eENABLE_CCD

                @see PxRigidDynamic.setKinematicTarget()
                */
                eKINEMATIC = ( 1 << 0 ),
                //!< Enable kinematic mode for the body.

                /**
                \brief Use the kinematic target transform for scene queries.

                If this flag is raised, then scene queries will treat the kinematic target transform as
                the current pose of the body (instead of using the actual pose). Without this flag, the
                kinematic target will only take effect with respect to scene queries after a simulation
                step.

                @see PxRigidDynamic.setKinematicTarget()
                */
                eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES = ( 1 << 1 ),

                /**
                \brief Enables swept integration for the actor.

                If this flag is raised and CCD is enabled on the scene, then this body will be simulated
                by the CCD system to ensure that collisions are not missed due to high-speed motion. Note
                individual shape pairs still need to enable PxPairFlag::eDETECT_CCD_CONTACT in the
                collision filtering to enable the CCD to respond to individual interactions.

                \note kinematic actors are incompatible with CCD so this flag will be cleared
                automatically when raised on a kinematic actor

                */
                eENABLE_CCD = ( 1 << 2 ),
                //!< Enable CCD for the body.

                /**
                \brief Enabled CCD in swept integration for the actor.

                If this flag is raised and CCD is enabled, CCD interactions will simulate friction. By
                default, friction is disabled in CCD interactions because CCD friction has been observed
                to introduce some simulation artifacts. CCD friction was enabled in previous versions of
                the SDK. Raising this flag will result in behavior that is a closer match for previous
                versions of the SDK.

                \note This flag requires PxRigidBodyFlag::eENABLE_CCD to be raised to have any effect.
                */
                eENABLE_CCD_FRICTION = ( 1 << 3 )
            };
        };

        struct FilterData
        {
            /**
            \brief Default constructor.
            */
            FilterData()
            {
                word0 = word1 = word2 = word3 = 0;
            }

            /**
            \brief Constructor to set filter data initially.
            */
            FilterData( u32 w0, u32 w1, u32 w2, u32 w3 ) :
                word0( w0 ),
                word1( w1 ),
                word2( w2 ),
                word3( w3 )
            {
            }

            /**
            \brief (re)sets the structure to the default.
            */
            void setToDefault()
            {
                *this = FilterData();
            }

            u32 word0;
            u32 word1;
            u32 word2;
            u32 word3;
        };

        struct D6JointDriveFlag
        {
            enum Enum
            {
                eACCELERATION =
                    1  //!< drive spring is for the acceleration at the joint (rather than the force)
            };
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // PhysicsTypes_h__
