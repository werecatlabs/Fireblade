#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/DataCoreTypes.h>
#include <FBCore/Base/Data.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/DataMacrosImplementation.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/JsonUtil.h>
#include <FBCore/Base/XmlUtil.h>
#include <FBCore/Base/PropertiesUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IFactory.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Base/DataBinding.h>
#include <cJSON.h>
#include <tinyxml.h>

namespace fb
{
    FB_REFLECT_REGISTER_DATAUTIL( data::attribute )

    FB_REFLECT_REGISTER_DATAUTIL( data::property )

    FB_REFLECT_REGISTER_DATAUTIL( data::properties )

    FB_REFLECT_REGISTER_DATAUTIL( data::vec4 )

    FB_REFLECT_REGISTER_DATAUTIL( data::vertex )

    FB_REFLECT_REGISTER_DATAUTIL( data::transform_data )

    FB_REFLECT_REGISTER_DATAUTIL( data::sub_mesh )

    FB_REFLECT_REGISTER_DATAUTIL( data::mesh )

    FB_REFLECT_REGISTER_DATAUTIL( data::texture_map )

    FB_REFLECT_REGISTER_DATAUTIL( data::material )

    FB_REFLECT_REGISTER_DATAUTIL( data::material_data )

    FB_REFLECT_REGISTER_DATAUTIL( data::material_node )

    FB_REFLECT_REGISTER_DATAUTIL( data::material_texture )

    FB_REFLECT_REGISTER_DATAUTIL( data::material_pass )

    FB_REFLECT_REGISTER_DATAUTIL( data::material_scheme )

    FB_REFLECT_REGISTER_DATAUTIL( data::material_graph )

    FB_REFLECT_REGISTER_DATAUTIL( data::debug_draw )

    namespace data
    {
        FB_REFLECT_REGISTER_STRUCT( data::attribute )

        FB_REFLECT_REGISTER_STRUCT( data::property )

        FB_REFLECT_REGISTER_STRUCT( data::properties )

        FB_REFLECT_REGISTER_STRUCT( data::vec4 )

        FB_REFLECT_REGISTER_STRUCT( data::vertex )

        FB_REFLECT_REGISTER_STRUCT( data::transform_data )

        FB_REFLECT_REGISTER_STRUCT( data::sub_mesh )

        FB_REFLECT_REGISTER_STRUCT( data::mesh )

        FB_REFLECT_REGISTER_STRUCT( data::texture_map )

        FB_REFLECT_REGISTER_STRUCT( data::material )

        FB_REFLECT_REGISTER_STRUCT( data::material_data )

        FB_REFLECT_REGISTER_STRUCT( data::material_node )

        FB_REFLECT_REGISTER_STRUCT( data::material_texture )

        FB_REFLECT_REGISTER_STRUCT( data::material_pass )

        FB_REFLECT_REGISTER_STRUCT( data::material_scheme )

        FB_REFLECT_REGISTER_STRUCT( data::material_graph )

        FB_REFLECT_REGISTER_STRUCT( data::debug_draw )
    }  // end namespace data
}  // end namespace fb
