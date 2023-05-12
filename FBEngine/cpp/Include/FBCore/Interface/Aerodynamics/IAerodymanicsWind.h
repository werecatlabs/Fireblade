#ifndef IAerodymanicsWind_h__
#define IAerodymanicsWind_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    
    class IAerodymanicsWind : public IVehicleComponent
    {
    public:
        /** Virtual destructor. */
        ~IAerodymanicsWind() override = default;

        virtual void setWind( real_Num Speed, real_Num Direction, real_Num Turb, real_Num GndHt,
                              real_Num DirOff, real_Num Rough ) = 0;

        virtual Vector3<real_Num> getWind( real_Num Height, real_Num Time ) = 0;
    };

}  // end namespace fb

#endif  // IAerodymanicsWind_h__
