#ifndef __FBCore_JsonDataDeclarations_h__
#define __FBCore_JsonDataDeclarations_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/DataBinding.h>
#include <FBCore/Base/DataMacros.h>

namespace fb
{
    namespace data
    {
        FB_REFLECT_DECL_STRUCT( data::attribute )
        FB_REFLECT_DECL_STRUCT( data::property )
        FB_REFLECT_DECL_STRUCT( data::properties )
        FB_REFLECT_DECL_STRUCT( data::vec4 )
        FB_REFLECT_DECL_STRUCT( data::vertex )
        FB_REFLECT_DECL_STRUCT( data::transform_data )
        FB_REFLECT_DECL_STRUCT( data::sub_mesh )
        FB_REFLECT_DECL_STRUCT( data::mesh )
        FB_REFLECT_DECL_STRUCT( data::texture_map )
        FB_REFLECT_DECL_STRUCT( data::material )
        FB_REFLECT_DECL_STRUCT( data::material_data )

        FB_REFLECT_DECL_STRUCT( data::material_node )
        FB_REFLECT_DECL_STRUCT( data::material_texture )
        FB_REFLECT_DECL_STRUCT( data::material_pass )
        FB_REFLECT_DECL_STRUCT( data::material_scheme )
        FB_REFLECT_DECL_STRUCT( data::material_graph )

        FB_REFLECT_DECL_STRUCT( data::debug_draw )
    }  // end namespace data
}  // end namespace fb

#endif  // JsonDataDeclarations_h__
