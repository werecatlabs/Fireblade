#include <FBCore/FBCorePCH.h>
#include <FBCore/Data/DataDeclarations.h>
#include "FBCore/Base/LogManager.h"
#include "FBCore/Data/DataDeclarations.h"
#include "FBCore/Data/AeroDataDeclarations.h"
#include "FBCore/Data/VehicleDataDeclarations.h"

//#include "FBCore/Base/JsonDataDeclarations.h"
//#include "FBCore/Base/JsonData.h"
//#include "FBCore/Base/DataUtil.h"
#include "FBCore/Base/Handle.h"
#include "FBCore/Base/DataBinding.h"
#include "FBCore/Base/DataMacrosImplementation.h"
//#include "FBCore/Base/JsonDataMacros.h"
#include "FBCore/Data/DefineDataStructures.h"
#include <FBCore/Base/DataCoreTypes.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/JsonUtil.h>
#include <FBCore/Base/XmlUtil.h>
#include <FBCore/Base/PropertiesUtil.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IFactory.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <tinyxml.h>
#include <cJSON.h>

#include <vector>
#include <string>

#include "FBCore/Base/DataCoreTypes.h"

namespace fb
{
    FB_REFLECT_REGISTER_DATAUTIL( data::render_settings_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::project_data );

    FB_REFLECT_REGISTER_DATAUTIL( data::importer_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::drag_drop_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::mesh_importer_data );

    FB_REFLECT_REGISTER_DATAUTIL( data::resolution_entry );
    FB_REFLECT_REGISTER_DATAUTIL( data::fsaa_entry );
    FB_REFLECT_REGISTER_DATAUTIL( data::resolution_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::render_system_config );

    FB_REFLECT_REGISTER_DATAUTIL( data::tree_prototype_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::tree_instance_data );

    FB_REFLECT_REGISTER_DATAUTIL( data::field );
    FB_REFLECT_REGISTER_DATAUTIL( data::rows );
    FB_REFLECT_REGISTER_DATAUTIL( data::query );

    FB_REFLECT_REGISTER_DATAUTIL( data::AttributeData );
    FB_REFLECT_REGISTER_DATAUTIL( data::PropertyData );
    FB_REFLECT_REGISTER_DATAUTIL( data::PropertiesData );

    FB_REFLECT_REGISTER_DATAUTIL( data::PluginEvent );
    FB_REFLECT_REGISTER_DATAUTIL( data::road );
    FB_REFLECT_REGISTER_DATAUTIL( data::road_node );
    FB_REFLECT_REGISTER_DATAUTIL( data::road_element );
    FB_REFLECT_REGISTER_DATAUTIL( data::road_section );
    FB_REFLECT_REGISTER_DATAUTIL( data::road_connection );
    FB_REFLECT_REGISTER_DATAUTIL( data::road_connection_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::road_network );
    FB_REFLECT_REGISTER_DATAUTIL( data::city_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::terrain_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::procedural_scene );
    FB_REFLECT_REGISTER_DATAUTIL( data::procedural_world );
    FB_REFLECT_REGISTER_DATAUTIL( data::block_node );
    FB_REFLECT_REGISTER_DATAUTIL( data::block_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::osm );
    FB_REFLECT_REGISTER_DATAUTIL( data::bounds_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::node_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::tag_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::nd_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::way_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::relation );
    FB_REFLECT_REGISTER_DATAUTIL( data::member );
    FB_REFLECT_REGISTER_DATAUTIL( data::actor_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::director_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::component_eventhandler_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::sub_component_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::component_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::fb_scene );
    FB_REFLECT_REGISTER_DATAUTIL( data::renderer_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::mesh_renderer_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_input_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_curve_value );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_curve_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_aerofoil_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_cog_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_control_surface_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_wing_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_engine_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_ground_effect_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_prop_wash_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::esc_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::aircraft_wheel_data );

    FB_REFLECT_REGISTER_DATAUTIL( data::vehicle_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::vehicle_wheel_data );

    // FB_REFLECT_REGISTER_DATAUTIL(data::String);
    FB_REFLECT_REGISTER_DATAUTIL( data::scenery_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::material_data);
    // FB_REFLECT_REGISTER_DATAUTIL(data::query);
    // FB_REFLECT_REGISTER_DATAUTIL(data::configured_models_data);
    FB_REFLECT_REGISTER_DATAUTIL( data::debug_text_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::dialog_manager_state);
    FB_REFLECT_REGISTER_DATAUTIL( data::resource_map );
    FB_REFLECT_REGISTER_DATAUTIL( data::glow_rope_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::rotor_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::transform_data);
    FB_REFLECT_REGISTER_DATAUTIL( data::sphere_collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::plane_collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::box_collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::mesh_collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::terrain_collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::compond_collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::collider );
    FB_REFLECT_REGISTER_DATAUTIL( data::physics_material );
    FB_REFLECT_REGISTER_DATAUTIL( data::object_bounds_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::constraint_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::rigid_body );
    FB_REFLECT_REGISTER_DATAUTIL( data::layer_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::model_layer_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::buoyancy_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::component_event_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::actor_event_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::debug_draw);
    FB_REFLECT_REGISTER_DATAUTIL( data::object_select_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::pid_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::pid_scaler_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::stab_settings_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::drone_stab_settings );
    FB_REFLECT_REGISTER_DATAUTIL( data::multi_rotor_ctrl_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::model_component_data );

    FB_REFLECT_REGISTER_DATAUTIL( data::collider_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::multi_rotor_prop_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::multi_rotor_motor_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::collider);
    FB_REFLECT_REGISTER_DATAUTIL( data::four_wheeled_vehicle_wheel_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::model_component_data);
    FB_REFLECT_REGISTER_DATAUTIL( data::file_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::directorylisting );
    FB_REFLECT_REGISTER_DATAUTIL( data::resource_map_entry );
    FB_REFLECT_REGISTER_DATAUTIL( data::movable_object_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::swash_plate_position );

    FB_REFLECT_REGISTER_DATAUTIL( data::setup_wizard_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::system_settings_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::scene_node_data );
    // FB_REFLECT_REGISTER_DATAUTIL(data::scenery_data);

    FB_REFLECT_REGISTER_DATAUTIL( data::plugin_event );
    FB_REFLECT_REGISTER_DATAUTIL( data::plugin_events );
    FB_REFLECT_REGISTER_DATAUTIL( data::configured_model_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::model_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::models_physics_setup_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::CMapData );
    FB_REFLECT_REGISTER_DATAUTIL( data::car_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::truck_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::model_control_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::fsm_data );
    FB_REFLECT_REGISTER_DATAUTIL( data::cg_info_mode );

    FB_REFLECT_REGISTER_DATAUTIL( data::path_data );

    FB_REFLECT_REGISTER_DATAUTIL( data::json_component );
    FB_REFLECT_REGISTER_DATAUTIL( data::json_actor );
    FB_REFLECT_REGISTER_DATAUTIL( data::json_scene );

    namespace data
    {
        FB_REFLECT_REGISTER_STRUCT( data::render_settings_data );
        FB_REFLECT_REGISTER_STRUCT( data::project_data );

        FB_REFLECT_REGISTER_STRUCT( data::importer_data );
        FB_REFLECT_REGISTER_STRUCT( data::drag_drop_data );
        FB_REFLECT_REGISTER_STRUCT( data::mesh_importer_data );

        FB_REFLECT_REGISTER_STRUCT( data::resolution_entry );
        FB_REFLECT_REGISTER_STRUCT( data::fsaa_entry );
        FB_REFLECT_REGISTER_STRUCT( data::resolution_data );
        FB_REFLECT_REGISTER_STRUCT( data::render_system_config );

        FB_REFLECT_REGISTER_STRUCT( data::tree_prototype_data );
        FB_REFLECT_REGISTER_STRUCT( data::tree_instance_data );

        FB_REFLECT_REGISTER_STRUCT( data::field );
        FB_REFLECT_REGISTER_STRUCT( data::rows );
        FB_REFLECT_REGISTER_STRUCT( data::query );

        FB_REFLECT_REGISTER_STRUCT( data::AttributeData );
        FB_REFLECT_REGISTER_STRUCT( data::PropertyData );
        FB_REFLECT_REGISTER_STRUCT( data::PropertiesData );

        FB_REFLECT_REGISTER_STRUCT( data::PluginEvent );
        FB_REFLECT_REGISTER_STRUCT( data::road );
        FB_REFLECT_REGISTER_STRUCT( data::road_node );
        FB_REFLECT_REGISTER_STRUCT( data::road_element );
        FB_REFLECT_REGISTER_STRUCT( data::road_section );
        FB_REFLECT_REGISTER_STRUCT( data::road_connection );
        FB_REFLECT_REGISTER_STRUCT( data::road_connection_data );
        FB_REFLECT_REGISTER_STRUCT( data::road_network );
        FB_REFLECT_REGISTER_STRUCT( data::city_data );
        FB_REFLECT_REGISTER_STRUCT( data::terrain_data );
        FB_REFLECT_REGISTER_STRUCT( data::procedural_scene );
        FB_REFLECT_REGISTER_STRUCT( data::procedural_world );
        FB_REFLECT_REGISTER_STRUCT( data::block_node );
        FB_REFLECT_REGISTER_STRUCT( data::block_data );
        FB_REFLECT_REGISTER_STRUCT( data::osm );
        FB_REFLECT_REGISTER_STRUCT( data::bounds_data );
        FB_REFLECT_REGISTER_STRUCT( data::node_data );
        FB_REFLECT_REGISTER_STRUCT( data::tag_data );
        FB_REFLECT_REGISTER_STRUCT( data::nd_data );
        FB_REFLECT_REGISTER_STRUCT( data::way_data );
        FB_REFLECT_REGISTER_STRUCT( data::relation );
        FB_REFLECT_REGISTER_STRUCT( data::member );
        FB_REFLECT_REGISTER_STRUCT( data::actor_data );
        FB_REFLECT_REGISTER_STRUCT( data::director_data );
        FB_REFLECT_REGISTER_STRUCT( data::component_eventhandler_data );
        FB_REFLECT_REGISTER_STRUCT( data::sub_component_data );
        FB_REFLECT_REGISTER_STRUCT( data::component_data );
        FB_REFLECT_REGISTER_STRUCT( data::fb_scene );
        FB_REFLECT_REGISTER_STRUCT( data::renderer_data );
        FB_REFLECT_REGISTER_STRUCT( data::mesh_renderer_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_input_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_curve_value );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_curve_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_aerofoil_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_cog_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_control_surface_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_wing_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_engine_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_ground_effect_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_prop_wash_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_data );
        FB_REFLECT_REGISTER_STRUCT( data::esc_data );
        FB_REFLECT_REGISTER_STRUCT( data::aircraft_wheel_data );

        FB_REFLECT_REGISTER_STRUCT( data::vehicle_data );
        FB_REFLECT_REGISTER_STRUCT( data::vehicle_wheel_data );

        // FB_REFLECT_REGISTER_STRUCT(data::String);
        FB_REFLECT_REGISTER_STRUCT( data::scenery_data );
        // FB_REFLECT_REGISTER_STRUCT(data::material_data);
        // FB_REFLECT_REGISTER_STRUCT(data::query);
        // FB_REFLECT_REGISTER_STRUCT(data::configured_models_data);
        FB_REFLECT_REGISTER_STRUCT( data::debug_text_data );
        // FB_REFLECT_REGISTER_STRUCT(data::dialog_manager_state);
        FB_REFLECT_REGISTER_STRUCT( data::resource_map );
        FB_REFLECT_REGISTER_STRUCT( data::glow_rope_data );
        FB_REFLECT_REGISTER_STRUCT( data::rotor_data );
        // FB_REFLECT_REGISTER_STRUCT(data::transform_data);
        FB_REFLECT_REGISTER_STRUCT( data::sphere_collider );
        FB_REFLECT_REGISTER_STRUCT( data::plane_collider );
        FB_REFLECT_REGISTER_STRUCT( data::box_collider );
        FB_REFLECT_REGISTER_STRUCT( data::mesh_collider );
        FB_REFLECT_REGISTER_STRUCT( data::terrain_collider );
        FB_REFLECT_REGISTER_STRUCT( data::compond_collider );
        FB_REFLECT_REGISTER_STRUCT( data::collider );
        FB_REFLECT_REGISTER_STRUCT( data::physics_material );
        FB_REFLECT_REGISTER_STRUCT( data::object_bounds_data );
        FB_REFLECT_REGISTER_STRUCT( data::constraint_data );
        FB_REFLECT_REGISTER_STRUCT( data::rigid_body );
        FB_REFLECT_REGISTER_STRUCT( data::layer_data );
        FB_REFLECT_REGISTER_STRUCT( data::model_layer_data );
        FB_REFLECT_REGISTER_STRUCT( data::buoyancy_data );
        FB_REFLECT_REGISTER_STRUCT( data::component_event_data );
        FB_REFLECT_REGISTER_STRUCT( data::actor_event_data );
        // FB_REFLECT_REGISTER_STRUCT(data::debug_draw);
        FB_REFLECT_REGISTER_STRUCT( data::object_select_data );
        FB_REFLECT_REGISTER_STRUCT( data::pid_data );
        FB_REFLECT_REGISTER_STRUCT( data::pid_scaler_data );
        FB_REFLECT_REGISTER_STRUCT( data::stab_settings_data );
        FB_REFLECT_REGISTER_STRUCT( data::drone_stab_settings );
        FB_REFLECT_REGISTER_STRUCT( data::multi_rotor_ctrl_data );
        FB_REFLECT_REGISTER_STRUCT( data::model_component_data );
        // FB_REFLECT_REGISTER_STRUCT(data::resolution_data);
        FB_REFLECT_REGISTER_STRUCT( data::setup_wizard_data );
        FB_REFLECT_REGISTER_STRUCT( data::system_settings_data );

        FB_REFLECT_REGISTER_STRUCT( data::scene_node_data );
        FB_REFLECT_REGISTER_STRUCT( data::water_ripples );
        FB_REFLECT_REGISTER_STRUCT( data::path_data );

        FB_REFLECT_REGISTER_STRUCT( data::model_physics_setup_flight_data );
        FB_REFLECT_REGISTER_STRUCT( data::model_physics_setup_data );
        FB_REFLECT_REGISTER_STRUCT( data::models_physics_setup_data );

        FB_REFLECT_REGISTER_STRUCT( data::configured_model_component_data );
        FB_REFLECT_REGISTER_STRUCT( data::configured_model_data );
        FB_REFLECT_REGISTER_STRUCT( data::switch_indicators_data );
        FB_REFLECT_REGISTER_STRUCT( data::four_wheeled_vehicle_data );

        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_input_data);
        // JSON_REGISTER_STRUCT(data::aircraft_curve_value);
        // JSON_REGISTER_STRUCT(data::aircraft_curve_data);
        // JSON_REGISTER_STRUCT(data::aircraft_aerofoil_data);
        // JSON_REGISTER_STRUCT(data::aircraft_cog_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_control_surface_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_wing_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_engine_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_ground_effect_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_prop_wash_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_data);
        FB_REFLECT_REGISTER_STRUCT( data::car_data );
        FB_REFLECT_REGISTER_STRUCT( data::truck_data );
        // FB_REFLECT_REGISTER_STRUCT(data::esc_data);
        // FB_REFLECT_REGISTER_STRUCT(data::aircraft_wheel_data);

        FB_REFLECT_REGISTER_STRUCT( data::cg_info_mode );
        FB_REFLECT_REGISTER_STRUCT( data::CMapData );

        FB_REFLECT_REGISTER_STRUCT( data::plugin_event );
        FB_REFLECT_REGISTER_STRUCT( data::plugin_events );

        FB_REFLECT_REGISTER_STRUCT( data::servo_hole_data );
        FB_REFLECT_REGISTER_STRUCT( data::servo_data );
        FB_REFLECT_REGISTER_STRUCT( data::swash_plate_data );
        FB_REFLECT_REGISTER_STRUCT( data::model_control_data );

        FB_REFLECT_REGISTER_STRUCT( data::collider_data );
        FB_REFLECT_REGISTER_STRUCT( data::multi_rotor_prop_data );
        FB_REFLECT_REGISTER_STRUCT( data::multi_rotor_motor_data );
        FB_REFLECT_REGISTER_STRUCT( data::four_wheeled_vehicle_wheel_data );
        FB_REFLECT_REGISTER_STRUCT( data::file_data );
        FB_REFLECT_REGISTER_STRUCT( data::directorylisting );
        FB_REFLECT_REGISTER_STRUCT( data::resource_map_entry );
        FB_REFLECT_REGISTER_STRUCT( data::movable_object_data );
        FB_REFLECT_REGISTER_STRUCT( data::swash_plate_position );
        FB_REFLECT_REGISTER_STRUCT( data::model_data );
        FB_REFLECT_REGISTER_STRUCT( data::fsm_data );

        FB_REFLECT_REGISTER_STRUCT( data::json_component );
        FB_REFLECT_REGISTER_STRUCT( data::json_actor );
        FB_REFLECT_REGISTER_STRUCT( data::json_scene );
    }  // end namespace data
}  // end namespace fb
