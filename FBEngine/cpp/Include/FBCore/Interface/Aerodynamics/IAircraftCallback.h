#ifndef IAircraftCallback_h__
#define IAircraftCallback_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleCallback.h>

namespace fb
{
    
    /** Interface for an aircraft callback class. */
    class IAircraftCallback : public IVehicleCallback
    {
    public:
        ~IAircraftCallback() override = default;

        String getData( const String &filePath ) override = 0;

        void addForce( s32 bodyId, const Vector3<real_Num> &force,
                       const Vector3<real_Num> &pos ) override = 0;

        /** Add a world force. */
        virtual void addForce( s32 bodyId, const Vector3<real_Num> &force ) = 0;

        /** Add a world torque. */
        void addTorque( s32 bodyId, const Vector3<real_Num> &torque ) override = 0;

        void addLocalForce( s32 bodyId, const Vector3<real_Num> &force,
                            const Vector3<real_Num> &pos ) override = 0;
        void addLocalTorque( s32 bodyId, const Vector3<real_Num> &torque ) override = 0;

        Vector3<real_Num> getAngularVelocity() const override = 0;
        Vector3<real_Num> getLinearVelocity() const override = 0;

        Vector3<real_Num> getLocalAngularVelocity() const override = 0;
        Vector3<real_Num> getLocalLinearVelocity() const override = 0;

        Vector3<real_Num> getPosition() const override = 0;
        Quaternion<real_Num> getOrientation() const override = 0;

        void *getCallbackFunction() const override = 0;
        void setCallbackFunction( void *val ) override = 0;

        void *getCallbackDataFunction() const override = 0;
        void setCallbackDataFunction( void *val ) override = 0;

        std::array<f32, 8> getInputData() const override = 0;
        std::array<f32, 11> getControlAngles() const override = 0;

        Vector3<real_Num> getPointVelocity( const Vector3<real_Num> &p ) override = 0;
    };
}  // end namespace fb

#endif  // IAircraftCallback_h__
