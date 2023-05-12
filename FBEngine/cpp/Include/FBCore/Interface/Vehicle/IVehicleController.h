#ifndef __IVehicleController_h__
#define __IVehicleController_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    
    /** Interface for a vehicle class. */
    class IVehicleController : public ISharedObject
    {
    public:
        enum class State
        {
            AWAKE,
            DESTROYED,
            EDIT,
            PLAY,
            RESET,

            COUNT
        };

        /** Virtual destructor. */
        ~IVehicleController() override = default;

        virtual void updateTransform() = 0;

        virtual void reset() = 0;

        virtual f32 getChannel( s32 idx ) const = 0;
        virtual void setChannel( s32 idx, f32 channel ) = 0;

        virtual Vector3<real_Num> getPosition() const = 0;
        virtual void setPosition( const Vector3<real_Num> &position ) = 0;

        virtual Quaternion<real_Num> getOrientation() const = 0;
        virtual void setOrientation( const Quaternion<real_Num> &orientation ) = 0;

        virtual bool isUserControlled() const = 0;
        virtual void setUserControlled( bool val ) = 0;

        virtual real_Num getMass() const = 0;
        virtual void setMass( real_Num mass ) = 0;

        virtual SmartPtr<IVehicleBody> getBody() const = 0;
        virtual void setBody( SmartPtr<IVehicleBody> val ) = 0;

        virtual Transform3<real_Num> getWorldTransform() const = 0;
        virtual void setWorldTransform( Transform3<real_Num> val ) = 0;

        virtual Transform3<real_Num> getLocalTransform() const = 0;
        virtual void setLocalTransform( Transform3<real_Num> val ) = 0;

        virtual void drawPoint( s32 body, int id, Vector3<real_Num> positon, u32 color ) = 0;
        virtual void drawLocalPoint( s32 body, int id, Vector3<real_Num> positon, u32 color ) = 0;

        virtual void displayLocalVector( s32 bodyId, const Vector3<real_Num> &start,
                                         const Vector3<real_Num> &end, u32 colour ) = 0;
        virtual void displayVector( s32 bodyId, s32 id, const Vector3<real_Num> &start,
                                    const Vector3<real_Num> &end, u32 colour ) = 0;
        virtual void displayLocalVector( s32 bodyId, s32 id, const Vector3<real_Num> &start,
                                         const Vector3<real_Num> &end, u32 colour ) = 0;

        virtual bool getDisplayDebugData() const = 0;
        virtual void setDisplayDebugData( bool val ) = 0;

        virtual void addForce( s32 bodyIdx, Vector3<real_Num> Force, Vector3<real_Num> Loc ) = 0;
        virtual void addTorque( s32 bodyIdx, Vector3<real_Num> Torque ) = 0;

        virtual void addLocalForce( s32 bodyIdx, Vector3<real_Num> Force, Vector3<real_Num> Loc ) = 0;
        virtual void addLocalTorque( s32 bodyIdx, Vector3<real_Num> Torque ) = 0;

        virtual Vector3<real_Num> getPointVelocity( const Vector3<real_Num> &p ) = 0;

        virtual Vector3<real_Num> getAngularVelocity() = 0;
        virtual Vector3<real_Num> getLinearVelocity() = 0;

        virtual Vector3<real_Num> getLocalAngularVelocity() = 0;
        virtual Vector3<real_Num> getLocalLinearVelocity() = 0;

        virtual SmartPtr<IVehicleCallback> &getVehicleCallback() = 0;
        virtual const SmartPtr<IVehicleCallback> &getVehicleCallback() const = 0;
        virtual void setVehicleCallback( SmartPtr<IVehicleCallback> callback ) = 0;

        virtual Vector3<real_Num> getCG() const = 0;

        /** */
        virtual void setState( State state ) = 0;

        /** */
        virtual State getState() const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // __IVehicleController_h__
