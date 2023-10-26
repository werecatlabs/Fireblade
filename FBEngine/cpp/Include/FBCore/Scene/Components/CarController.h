#ifndef CarController_h__
#define CarController_h__

#include <FBCore/Scene/Components/VehicleController.h>
#include <FBCore/Interface/Vehicle/IVehicleCallback.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace scene
    {
        class CarController : public VehicleController
        {
        public:
            enum class Wheels
            {
                FRONT_LEFT,
                FRONT_RIGHT,
                REAR_LEFT,
                REAR_RIGHT
            };

            CarController();
            ~CarController() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::start */
            void play();

            /** @copydoc IComponent::edit */
            void edit();

            void updateTransform() override;

            void setupWheels();

            /** @copydoc VehicleController::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc VehicleController::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc VehicleController::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            SmartPtr<Rigidbody> getChassis() const;
            void setChassis( SmartPtr<Rigidbody> chassis );

            SmartPtr<IVehicleController> getVehicleController() const;
            void setVehicleController( SmartPtr<IVehicleController> vehicleController );

            f32 getThrottle() const;
            void setThrottle( f32 throttle );

            f32 getBrake() const;
            void setBrake( f32 brake );

            f32 getSteering() const;
            void setSteering( f32 steering );

            Vector3F getMOI() const;

            void setMOI( const Vector3F &moi );

            FB_CLASS_REGISTER_DECL;

        protected:
            class InputListener : public IEventListener
            {
            public:
                InputListener() = default;
                ~InputListener() override = default;

                void unload( SmartPtr<ISharedObject> data ) override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                bool inputEvent( SmartPtr<IInputEvent> event );
                bool updateEvent( const SmartPtr<IInputEvent> &event );

                void setPriority( s32 priority );
                s32 getPriority() const;

                SmartPtr<CarController> getOwner() const;
                void setOwner( SmartPtr<CarController> owner );

            protected:
                SmartPtr<CarController> m_owner;
                u32 m_priority = 1000;
            };

            class VehicleCallback : public IVehicleCallback
            {
            public:
                VehicleCallback() = default;
                VehicleCallback( CarController *controller );
                ~VehicleCallback() override;

                String getData( const String &filePath ) override;

                void addForce( s32 bodyId, const Vector3<real_Num> &force,
                               const Vector3<real_Num> &pos ) override;
                void addTorque( s32 bodyId, const Vector3<real_Num> &torque ) override;
                void addLocalForce( s32 bodyId, const Vector3<real_Num> &force,
                                    const Vector3<real_Num> &pos ) override;
                void addLocalTorque( s32 bodyId, const Vector3<real_Num> &torque ) override;

                Vector3<real_Num> getAngularVelocity() const override;
                Vector3<real_Num> getLinearVelocity() const override;

                Vector3<real_Num> getLocalAngularVelocity() const override;
                Vector3<real_Num> getLocalLinearVelocity() const override;

                Vector3<real_Num> getPosition() const override;
                Quaternion<real_Num> getOrientation() const override;

                void displayLocalVector( s32 Bdy, const Vector3<real_Num> &V,
                                         const Vector3<real_Num> &Org, s32 colour ) const override;
                void displayVector( s32 Bdy, s32 id, const Vector3<real_Num> &V,
                                    const Vector3<real_Num> &Org, s32 colour ) const override;
                void displayLocalVector( s32 Bdy, s32 id, const Vector3<real_Num> &V,
                                         const Vector3<real_Num> &Org, s32 colour ) const override;

                void *getCallbackFunction() const override;
                void setCallbackFunction( void *val ) override;

                void *getCallbackDataFunction() const override;
                void setCallbackDataFunction( void *val ) override;

                FixedArray<f32, 8> getInputData() const override;
                FixedArray<f32, 11> getControlAngles() const override;

                Vector3<real_Num> getPointVelocity( const Vector3<real_Num> &p ) override;

                bool castLocalRay( const Ray3<real_Num> &ray,
                                   SmartPtr<physics::IRaycastHit> &data ) override;
                bool castWorldRay( const Ray3<real_Num> &ray,
                                   SmartPtr<physics::IRaycastHit> &data ) override;

                SmartPtr<CarController> getOwner() const;
                void setOwner( SmartPtr<CarController> owner );

            protected:
                SmartPtr<CarController> m_owner = nullptr;
            };

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            Vector3F m_moi = Vector3F::unit();
            Vector3F m_cg = Vector3F::zero();
            f32 m_wheelBase = 2.530f;
            f32 m_length = 4.405f;
            f32 m_width = 1.810f;
            f32 m_height = 1.170f;
            f32 m_mass = 1370.0f;
            f32 m_wheelChassisOffset = 0.0f;

            // Wheel controller overrides
            f32 m_radius = 0.35f;
            f32 m_suspensionTravel = 0.3f;
            f32 m_damping = 1000.0f;
            f32 m_inertia = 2.2f;
            f32 m_grip = 1.0f;
            f32 m_brakeFrictionTorque = 4000;
            f32 m_handbrakeFrictionTorque = 0;
            f32 m_frictionTorque = 10;
            f32 m_maxSteeringAngle = 28.f;
            f32 m_massFraction = 0.25f;

            f32 m_throttle = 0.0f;
            f32 m_brake = 0.0f;
            f32 m_steering = 0.0f;

            SmartPtr<IEventListener> m_inputListener;
            SmartPtr<IVehicleController> m_vehicleController;
            SmartPtr<VehicleCallback> m_vehicleCallback;

            // SmartPtr<render::IGraphicsObject> m_mesh;
            // SmartPtr<render::ISceneNode> m_node;

            SmartPtr<Collision> m_collision;
            SmartPtr<Rigidbody> m_chassis;

            // SmartPtr<WheelController> m_leftFront;
            // SmartPtr<WheelController> m_rightFront;
            // SmartPtr<WheelController> m_leftRear;
            // SmartPtr<WheelController> m_rightRear;

            Array<SmartPtr<WheelController>> m_wheels;
            Array<SmartPtr<WheelController>> m_poweredWheels;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CarController_h__
