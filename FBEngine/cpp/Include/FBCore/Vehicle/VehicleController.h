#ifndef __VehicleController_h__
#define __VehicleController_h__

#include <FBCore/Interface/Vehicle/IVehicleController.h>

namespace fb
{

    /** Simple vehicle controller implementation. */
    class VehicleController : public IVehicleController
    {
    public:
        /** Constructor. */
        VehicleController();

        /** Destructor. */
        ~VehicleController();

        /** @copydoc IVehicleController::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IVehicleController::reset */
        void reset() override;

        /** @copydoc IVehicleController::update */
        void update() override;

        void updateTransform();

        f32 getChannel( s32 idx ) const override;
        void setChannel( s32 idx, f32 channel ) override;

        Vector3<real_Num> getScale() const;

        Vector3<real_Num> getPosition() const override;

        void setPosition( const Vector3<real_Num> &position ) override;

        Quaternion<real_Num> getOrientation() const;
        void setOrientation( const Quaternion<real_Num> &orientation );

        bool isUserControlled() const override;
        void setUserControlled( bool val ) override;

        real_Num getMass() const override;
        void setMass( real_Num mass ) override;

        SmartPtr<IVehicleBody> getBody() const override;
        void setBody( SmartPtr<IVehicleBody> val ) override;

        Transform3<real_Num> getWorldTransform() const override;
        void setWorldTransform( Transform3<real_Num> val ) override;

        Transform3<real_Num> getLocalTransform() const override;
        void setLocalTransform( Transform3<real_Num> val ) override;

        void drawPoint( s32 body, int id, Vector3<real_Num> positon, u32 color ) override;
        void drawLocalPoint( s32 body, int id, Vector3<real_Num> positon, u32 color ) override;
        void displayLocalVector( s32 bodyId, const Vector3<real_Num> &start,
                                 const Vector3<real_Num> &end, u32 colour ) override;
        void displayVector( s32 bodyId, s32 id, const Vector3<real_Num> &start,
                            const Vector3<real_Num> &end, u32 colour ) override;
        void displayLocalVector( s32 bodyId, s32 id, const Vector3<real_Num> &start,
                                 const Vector3<real_Num> &end, u32 colour ) override;

        bool getDisplayDebugData() const override;
        void setDisplayDebugData( bool displayDebugData ) override;

        void addForce( s32 bodyIdx, Vector3<real_Num> Force, Vector3<real_Num> Loc ) override;
        void addTorque( s32 bodyIdx, Vector3<real_Num> Torque ) override;
        void addLocalForce( s32 bodyIdx, Vector3<real_Num> Force, Vector3<real_Num> Loc ) override;
        void addLocalTorque( s32 bodyIdx, Vector3<real_Num> Torque ) override;

        Vector3<real_Num> getPointVelocity( const Vector3<real_Num> &p ) override;
        Vector3<real_Num> getAngularVelocity() override;
        Vector3<real_Num> getLinearVelocity() override;
        Vector3<real_Num> getLocalAngularVelocity() override;
        Vector3<real_Num> getLocalLinearVelocity() override;

        SmartPtr<IVehicleCallback> &getVehicleCallback() override;
        const SmartPtr<IVehicleCallback> &getVehicleCallback() const override;
        void setVehicleCallback( SmartPtr<IVehicleCallback> callback ) override;

        Vector3<real_Num> getCG() const override;

        /** @copydoc IVehicleController::setState */
        virtual void setState( IVehicleController::State state ) override;

        /** @copydoc IVehicleController::getState */
        virtual IVehicleController::State getState() const override;

        SmartPtr<IWheelController> getWheelController( u32 index ) const;

    protected:
        void addForce( const Vector3<real_Num> &force );

        void addTorque( const Vector3<real_Num> &torque );

        void clearForces();

        Atomic<IVehicleController::State> m_vehicleState = IVehicleController::State::AWAKE;

        Transform3<real_Num> m_bodyTransform;
        Transform3<real_Num> m_worldTransform;
        Transform3<real_Num> m_localTransform;

        AtomicSmartPtr<IVehicleBody> m_rigidbody;
        SmartPtr<IVehicleCallback> m_callback;

        Vector3<real_Num> m_force = Vector3<real_Num>::zero();
        Vector3<real_Num> m_torque = Vector3<real_Num>::zero();

        Vector3<real_Num> m_drag = Vector3<real_Num>::zero();
        Vector3<real_Num> m_cg = Vector3<real_Num>::zero();

        atomic_bool m_displayDebugData = false;

        Array<f32> m_channels;

        Array<SmartPtr<IWheelController>> m_wheels;
    };

}  // namespace fb

#endif  // VehicleController_h__
