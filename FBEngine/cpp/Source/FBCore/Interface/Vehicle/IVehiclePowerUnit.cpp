#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Vehicle/IVehiclePowerUnit.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IVehiclePowerUnit, IVehicleComponent );
}  // end namespace fb