#ifndef __DataDeclarations_h__
#define __DataDeclarations_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/DataCoreTypes.h>

namespace fb
{
    namespace data
    {
        FB_REFLECT_DECL_STRUCT( data::render_settings_data );
        FB_REFLECT_DECL_STRUCT( data::project_data );

        FB_REFLECT_DECL_STRUCT( data::importer_data );
        FB_REFLECT_DECL_STRUCT( data::drag_drop_data );
        FB_REFLECT_DECL_STRUCT( data::mesh_importer_data );

        FB_REFLECT_DECL_STRUCT( data::resolution_entry );
        FB_REFLECT_DECL_STRUCT( data::fsaa_entry );
        FB_REFLECT_DECL_STRUCT( data::resolution_data );
        FB_REFLECT_DECL_STRUCT( data::render_system_config );

        FB_REFLECT_DECL_STRUCT( data::tree_prototype_data );
        FB_REFLECT_DECL_STRUCT( data::tree_instance_data );
        FB_REFLECT_DECL_STRUCT( data::object_bounds_data );

        FB_REFLECT_DECL_STRUCT( data::field );
        FB_REFLECT_DECL_STRUCT( data::rows );
        FB_REFLECT_DECL_STRUCT( data::query );

        FB_REFLECT_DECL_STRUCT( data::AttributeData );
        FB_REFLECT_DECL_STRUCT( data::PropertyData );
        FB_REFLECT_DECL_STRUCT( data::PropertiesData );

        FB_REFLECT_DECL_STRUCT( data::PluginEvent );
        FB_REFLECT_DECL_STRUCT( data::road );
        FB_REFLECT_DECL_STRUCT( data::road_node );
        FB_REFLECT_DECL_STRUCT( data::road_element );
        FB_REFLECT_DECL_STRUCT( data::road_section );
        FB_REFLECT_DECL_STRUCT( data::road_connection );
        FB_REFLECT_DECL_STRUCT( data::road_connection_data );
        FB_REFLECT_DECL_STRUCT( data::road_network );
        FB_REFLECT_DECL_STRUCT( data::city_data );
        FB_REFLECT_DECL_STRUCT( data::terrain_data );
        FB_REFLECT_DECL_STRUCT( data::procedural_scene );
        FB_REFLECT_DECL_STRUCT( data::procedural_world );

        FB_REFLECT_DECL_STRUCT( data::osm );
        FB_REFLECT_DECL_STRUCT( data::bounds_data );
        FB_REFLECT_DECL_STRUCT( data::node_data );
        FB_REFLECT_DECL_STRUCT( data::tag_data );
        FB_REFLECT_DECL_STRUCT( data::nd_data );
        FB_REFLECT_DECL_STRUCT( data::way_data );
        FB_REFLECT_DECL_STRUCT( data::relation );
        FB_REFLECT_DECL_STRUCT( data::member );

        FB_REFLECT_DECL_STRUCT( data::block_node );
        FB_REFLECT_DECL_STRUCT( data::block_data );

        FB_REFLECT_DECL_STRUCT( data::actor_data );
        FB_REFLECT_DECL_STRUCT( data::sub_component_data );
        FB_REFLECT_DECL_STRUCT( data::component_data );
        FB_REFLECT_DECL_STRUCT( data::fb_scene );

        FB_REFLECT_DECL_STRUCT( data::json_component );
        FB_REFLECT_DECL_STRUCT( data::json_actor );
        FB_REFLECT_DECL_STRUCT( data::json_scene );

        FB_REFLECT_DECL_STRUCT( data::renderer_data );
        FB_REFLECT_DECL_STRUCT( data::mesh_renderer_data );

        FB_REFLECT_DECL_STRUCT( data::query );
        FB_REFLECT_DECL_STRUCT( data::resource_map );
        FB_REFLECT_DECL_STRUCT( data::glow_rope_data );
        FB_REFLECT_DECL_STRUCT( data::rotor_data );
        // FB_REFLECT_DECL_STRUCT(data::transform_data);
        FB_REFLECT_DECL_STRUCT( data::sphere_collider );
        FB_REFLECT_DECL_STRUCT( data::plane_collider );
        FB_REFLECT_DECL_STRUCT( data::box_collider );
        FB_REFLECT_DECL_STRUCT( data::mesh_collider );
        FB_REFLECT_DECL_STRUCT( data::terrain_collider );
        FB_REFLECT_DECL_STRUCT( data::compond_collider );
        FB_REFLECT_DECL_STRUCT( data::collider );
        FB_REFLECT_DECL_STRUCT( data::physics_material );
        FB_REFLECT_DECL_STRUCT( data::bounds_data );
        FB_REFLECT_DECL_STRUCT( data::constraint_data );
        FB_REFLECT_DECL_STRUCT( data::rigid_body );
        FB_REFLECT_DECL_STRUCT( data::layer_data );
        FB_REFLECT_DECL_STRUCT( data::model_layer_data );
        FB_REFLECT_DECL_STRUCT( data::buoyancy_data );
        FB_REFLECT_DECL_STRUCT( data::actor_data );
        FB_REFLECT_DECL_STRUCT( data::component_event_data );
        FB_REFLECT_DECL_STRUCT( data::actor_event_data );
        // FB_REFLECT_DECL_STRUCT(data::debug_draw);
        FB_REFLECT_DECL_STRUCT( data::object_select_data );
        FB_REFLECT_DECL_STRUCT( data::pid_data );
        FB_REFLECT_DECL_STRUCT( data::pid_scaler_data );
        FB_REFLECT_DECL_STRUCT( data::stab_settings_data );
        FB_REFLECT_DECL_STRUCT( data::drone_stab_settings );
        FB_REFLECT_DECL_STRUCT( data::multi_rotor_ctrl_data );
        FB_REFLECT_DECL_STRUCT( data::component_data );
        FB_REFLECT_DECL_STRUCT( data::resolution_data );
        FB_REFLECT_DECL_STRUCT( data::setup_wizard_data );
        FB_REFLECT_DECL_STRUCT( data::system_settings_data );
        FB_REFLECT_DECL_STRUCT( data::scene_node_data );
        FB_REFLECT_DECL_STRUCT( data::water_ripples );
        FB_REFLECT_DECL_STRUCT( data::path_data );
        FB_REFLECT_DECL_STRUCT( data::model_physics_setup_flight_data );
        FB_REFLECT_DECL_STRUCT( data::model_physics_setup_data );
        FB_REFLECT_DECL_STRUCT( data::models_physics_setup_data );
        FB_REFLECT_DECL_STRUCT( data::configured_model_component_data );
        FB_REFLECT_DECL_STRUCT( data::configured_model_data );
        FB_REFLECT_DECL_STRUCT( data::switch_indicators_data );
        FB_REFLECT_DECL_STRUCT( data::four_wheeled_vehicle_data );
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

        FB_REFLECT_DECL_STRUCT( data::car_data );
        FB_REFLECT_DECL_STRUCT( data::truck_data );
        FB_REFLECT_DECL_STRUCT( data::esc_data );
        FB_REFLECT_DECL_STRUCT( data::aircraft_wheel_data );
        FB_REFLECT_DECL_STRUCT( data::cg_info_mode );
        FB_REFLECT_DECL_STRUCT( data::CMapData );
        FB_REFLECT_DECL_STRUCT( data::plugin_event );
        FB_REFLECT_DECL_STRUCT( data::plugin_events );
        FB_REFLECT_DECL_STRUCT( data::servo_hole_data );
        FB_REFLECT_DECL_STRUCT( data::servo_data );
        FB_REFLECT_DECL_STRUCT( data::swash_plate_data );
        FB_REFLECT_DECL_STRUCT( data::model_control_data );

        FB_REFLECT_DECL_STRUCT( data::collider_data );
        FB_REFLECT_DECL_STRUCT( data::director_data );
        FB_REFLECT_DECL_STRUCT( data::component_eventhandler_data );
        FB_REFLECT_DECL_STRUCT( data::component_data );
        FB_REFLECT_DECL_STRUCT( data::actor_data );

        FB_REFLECT_DECL_STRUCT( data::multi_rotor_prop_data );
        FB_REFLECT_DECL_STRUCT( data::multi_rotor_motor_data );
        FB_REFLECT_DECL_STRUCT( data::collider );

        FB_REFLECT_DECL_STRUCT( data::four_wheeled_vehicle_wheel_data );

        FB_REFLECT_DECL_STRUCT( data::model_component_data );

        FB_REFLECT_DECL_STRUCT( data::file_data );
        FB_REFLECT_DECL_STRUCT( data::directorylisting );

        FB_REFLECT_DECL_STRUCT( data::resource_map_entry );
        FB_REFLECT_DECL_STRUCT( data::movable_object_data );
        FB_REFLECT_DECL_STRUCT( data::swash_plate_position );

        FB_REFLECT_DECL_STRUCT( data::setup_wizard_data );
        FB_REFLECT_DECL_STRUCT( data::system_settings_data );
        FB_REFLECT_DECL_STRUCT( data::scenery_data );
        FB_REFLECT_DECL_STRUCT( data::model_data );
        FB_REFLECT_DECL_STRUCT( data::models_physics_setup_data );
        FB_REFLECT_DECL_STRUCT( data::fsm_data );
        FB_REFLECT_DECL_STRUCT( data::debug_text_data );
    }  // end namespace data
}  // end namespace fb

#endif  // JsonDataDeclarations_h__
