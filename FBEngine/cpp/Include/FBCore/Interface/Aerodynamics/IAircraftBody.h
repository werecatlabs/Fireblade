#ifndef IAircraftBody_h__
#define IAircraftBody_h__

#include <FBCore/FBCorePrerequisites.h>
#include "FBCore/Interface/Vehicle/IVehicleBody.h"

namespace fb
{

    class IAircraftBody : public IVehicleBody
    {
    public:
        ~IAircraftBody() override = default;
    };

}  // end namespace fb

#endif  // IAircraftBody_h__
