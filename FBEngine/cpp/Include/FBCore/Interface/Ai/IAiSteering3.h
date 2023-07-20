#ifndef IAiSteering3_h__
#define IAiSteering3_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    class IAiSteering3 : public ISharedObject
    {
    public:
        enum class summing_method
        {
            weighted_average,
            prioritized,
            dithered
        };

        enum class behavior_type
        {
            none = 0x00000,
            seek = 0x00002,
            arrive = 0x00008,
            wander = 0x00010,
            separation = 0x00040,
            wall_avoidance = 0x00200,
        };

        /** Virtual destructor. */
        ~IAiSteering3() override = default;

        // calculates and sums the steering forces from any active behaviors
        virtual Vector3F calculate() = 0;

        // calculates the component of the steering force that is parallel
        // with the entity heading
        virtual f32 forwardComponent() = 0;

        // calculates the component of the steering force that is perpendicular
        // with the entity heading
        virtual f32 sideComponent() = 0;

        virtual Vector3F getTarget() const = 0;
        virtual void setTarget( const Vector3F &target ) = 0;

        virtual Vector3F getDirection() const = 0;

        virtual Vector3F getPosition() const = 0;
        virtual void setPosition( const Vector3F &position ) = 0;

        virtual void setTargetAgent1( SmartPtr<scene::IActor> agent ) = 0;
        virtual void setTargetAgent2( SmartPtr<scene::IActor> agent ) = 0;

        virtual Vector3F getForce() const = 0;

        virtual void setSummingMethod( u32 sm ) = 0;

        virtual void seekOn() = 0;
        virtual void arriveOn() = 0;
        virtual void wanderOn() = 0;
        virtual void separationOn() = 0;
        virtual void wallAvoidanceOn() = 0;

        virtual void seekOff() = 0;
        virtual void arriveOff() = 0;
        virtual void wanderOff() = 0;
        virtual void separationOff() = 0;
        virtual void wallAvoidanceOff() = 0;

        virtual bool getSeekIsOn() = 0;
        virtual bool getArriveIsOn() = 0;
        virtual bool getWanderIsOn() = 0;
        virtual bool getSeparationIsOn() = 0;
        virtual bool getWallAvoidanceIsOn() = 0;

        virtual Array<Vector3F> getFeelers() const = 0;

        virtual f32 getWanderJitter() const = 0;
        virtual f32 getWanderDistance() const = 0;
        virtual f32 getWanderRadius() const = 0;

        virtual f32 getSeparationWeight() const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IAiSteering3_h__
