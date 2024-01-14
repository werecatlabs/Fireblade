#ifndef IDrone_h__
#define IDrone_h__

#include <FBCore/Interface/Vehicle/IVehicleController.h>

namespace fb
{

    /**
     *        * @brief An interface for a drone.
     *
     */
    class IDrone : public IVehicleController
    {
    public:
        ~IDrone() override = default;

        virtual f32 getGroundEffectMultiplier() const = 0;
        virtual void setGroundEffectMultiplier( f32 val ) = 0;

        virtual Vector3<real_Num> getDrag() const = 0;
        virtual void setDrag( const Vector3<real_Num> &val ) = 0;
    };

}  // end namespace fb

#endif  // IDrone_h__
