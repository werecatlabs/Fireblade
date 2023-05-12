#ifndef JsonDataDeclarations_h__
#define JsonDataDeclarations_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/DataCoreTypes.h>

namespace fb
{
    namespace data
    {
        FB_REFLECT_DECL_STRUCT( data::aircraft_input_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_curve_value );
        FB_REFLECT_DECL_STRUCT( data::aircraft_curve_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_aerofoil_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_cog_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_control_surface_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_wing_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_engine_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_ground_effect_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_prop_wash_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_data );
        FB_REFLECT_DECL_STRUCT( data::esc_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_wheel_data );
    }  // end namespace data
}  // end namespace fb

#endif  // JsonDataDeclarations_h__
