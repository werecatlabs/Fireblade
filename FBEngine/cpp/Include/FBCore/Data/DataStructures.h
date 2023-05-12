#include <FBCore/Base/StringTypes.h>

namespace fb
{
    namespace data
    {
        FB_STRUCT( render_settings_data )
        {
            FB_CONSTRUCTOR( render_settings_data() { renderPipeline = 0; } )

            FB_ATTR( int, renderPipeline );
        };

        FB_STRUCT( project_data )
        {
            FB_ATTR( std::string, projectVersion );
            FB_ATTR( std::string, uuid );
            FB_ATTR( std::string, productName );
            FB_ATTR( std::string, companyName );
            FB_ATTR( std::string, currentScenePath );
            FB_ATTR( render_settings_data, renderSettings );
        };

        FB_STRUCT( importer_data )
        {
            FB_ATTR( std::string, path );
            FB_ATTR( std::string, importerType );
        };

        FB_STRUCT( drag_drop_data )
        {
            FB_CONSTRUCTOR( drag_drop_data() { sourceId = 0; } )

            FB_ATTR( std::string, filePath );
            FB_ATTR( int, sourceId );
            FB_ATTR( int, actorId );
            FB_ATTR( std::string, actorUUID );
            FB_ATTR( std::string, sourceStr );
        };

        FB_STRUCT( mesh_importer_data )
        {
            FB_CONSTRUCTOR( mesh_importer_data() { meshScale = 1.0f; } )

            FB_ATTR( std::string, filePath );
            FB_ATTR( float, meshScale );
        };

        FB_STRUCT( field )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, value );
        };

        FB_STRUCT( rows )
        {
            FB_ATTR( string_pairs, fields );
            FB_ARRAY_PROPERTY( fb::data::field, fieldPairs );
        };

        FB_STRUCT( query )
        {
            FB_ARRAY_PROPERTY( fb::data::rows, rows );
        };

        FB_STRUCT( physics_material )
        {
            FB_CONSTRUCTOR( physics_material() {
                staticFriction = 1.0f;
                dynamicFriction = 1.0f;
                restitution = 1.0f;
                restitution = 0.0f;
            } )

            FB_ATTR( float, staticFriction );
            FB_ATTR( float, dynamicFriction );
            FB_ATTR( float, restitution );
            FB_ATTR( float, roughness );
        };

        FB_STRUCT( object_bounds_data )
        {
            FB_ATTR( vec4, minExtents );
            FB_ATTR( vec4, maxExtents );
        };

        FB_STRUCT( collider )
        {
            FB_CONSTRUCTOR( collider() { isTrigger = false; } )

            FB_ATTR( std::string, colliderType );
            FB_ATTR( transform_data, transform );
            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ATTR( physics_material, physicsMaterial );
            FB_ATTR( bool, isTrigger );
        };

        FB_STRUCT_INHERIT( sphere_collider, collider )
        {
            FB_CONSTRUCTOR( sphere_collider() { radius = 0; } )

            FB_ATTR( vec4, center );
            FB_ATTR( float, radius );
            FB_INHERIT( collider );
        };

        FB_STRUCT_INHERIT( plane_collider, collider )
        {
            FB_CONSTRUCTOR( plane_collider() { distance = 0; } )

            FB_ATTR( vec4, normal );
            FB_ATTR( float, distance );
            FB_INHERIT( collider );
        };

        FB_STRUCT_INHERIT( box_collider, collider )
        {
            FB_ATTR( vec4, center );
            FB_ATTR( vec4, size );
            FB_INHERIT( collider );
        };

        FB_STRUCT_INHERIT( mesh_collider, collider )
        {
            FB_CONSTRUCTOR( mesh_collider() { isConvex = false; } )

            FB_ATTR( std::string, filePath );
            FB_ATTR( bool, isConvex );
            FB_ARRAY_PROPERTY( vec4, colliders );
            FB_ARRAY_PROPERTY( int, indices );
            FB_INHERIT( collider );
        };

        FB_STRUCT_INHERIT( terrain_collider, collider )
        {
            FB_ATTR( std::string, filePath );
            FB_ATTR( vec4, heightScale );
            FB_ATTR( vec4, terrainSize );
            FB_ATTR( vec4, size );
            // FB_ARRAY_PROPERTY(float, heightData);
            FB_INHERIT( collider );
        };

        FB_STRUCT_INHERIT( compond_collider, collider )
        {
            FB_ARRAY_PROPERTY( sphere_collider, sphereColliders );
            FB_ARRAY_PROPERTY( plane_collider, planeColliders );
            FB_ARRAY_PROPERTY( box_collider, boxColliders );
            FB_ARRAY_PROPERTY( mesh_collider, meshColliders );
            FB_ARRAY_PROPERTY( terrain_collider, terrainColliders );
            FB_ARRAY_PROPERTY( compond_collider, compondColliders );
            FB_INHERIT( collider );
        };

        FB_STRUCT( collider_data )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, type );
            FB_ATTR( std::string, meshName );
            FB_ATTR( vec4, size );
            FB_ATTR( vec4, minExtents );
            FB_ATTR( vec4, maxExtents );
        };

        FB_STRUCT_INHERIT( road_connection_data, public base_data )
        {
            FB_ATTR( std::string, roadName );
            FB_ATTR( int, connectionIndex );
            FB_ATTR( int, roadConnectionIndex );
            FB_ATTR( transform_data, localTransform );
        };

        FB_STRUCT_INHERIT( road_connection, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, resourceName );
            FB_ATTR( std::string, connectionType );
            FB_ATTR( transform_data, localTransform );
            FB_ARRAY_PROPERTY( road_connection_data, roadConnectionData );
        };

        FB_STRUCT_INHERIT( road_node, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( transform_data, localTransform );
            FB_ARRAY_PROPERTY( std::string, roads );
            FB_ARRAY_PROPERTY( std::string, connectedNodes );
            FB_ARRAY_PROPERTY( std::string, mergedNodes );
            FB_ATTR( std::string, mergedNode );
            FB_ATTR( bool, isConnection );
        };

        FB_STRUCT_INHERIT( block_node, public base_data )
        {
            FB_ATTR( transform_data, localTransform );
        };

        FB_STRUCT_INHERIT( block_data, public base_data )
        {
            FB_ATTR( transform_data, localTransform );
            FB_ARRAY_PROPERTY( vec4, points );
            FB_ARRAY_PROPERTY( block_node, nodes );
            FB_ATTR( properties, keyvalues );
        };

        FB_STRUCT_INHERIT( road_element, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( vec4, points );
            FB_ARRAY_PROPERTY( road_node, nodes );
        };

        FB_STRUCT_INHERIT( road_section, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( vec4, points );
            FB_ARRAY_PROPERTY( road_node, nodes );
            FB_ARRAY_PROPERTY( road_element, elements );
        };

        FB_STRUCT_INHERIT( road, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( vec4, points );
            FB_ARRAY_PROPERTY( road_node, nodes );
            FB_ARRAY_PROPERTY( road_section, sections );
        };

        FB_STRUCT_INHERIT( road_network, public base_data )
        {
            FB_ARRAY_PROPERTY( road, roads );
            FB_ARRAY_PROPERTY( road_connection, roadConnections );
            FB_ARRAY_PROPERTY( road_node, nodes );
            FB_ARRAY_PROPERTY( road_node, mergedNodes );
        };

        FB_STRUCT_INHERIT( city_data, public base_data )
        {
            FB_ARRAY_PROPERTY( block_data, blocks );
            FB_ATTR( road_network, roadNetwork );
        };

        FB_STRUCT( resolution_entry )
        {
            FB_CONSTRUCTOR( resolution_entry() {
                width = 0;
                height = 0;
                option = 0;
            } )

            FB_ATTR( std::string, label );
            FB_ATTR( int, width );
            FB_ATTR( int, height );
            FB_ATTR( int, option );
        };

        FB_STRUCT( fsaa_entry )
        {
            FB_CONSTRUCTOR( fsaa_entry() { value = 0; } )

            FB_ATTR( std::string, label );
            FB_ATTR( int, value );
        };

        FB_STRUCT( resolution_data )
        {
            FB_ARRAY_PROPERTY( resolution_entry, resolutions );
            FB_ARRAY_PROPERTY( fsaa_entry, availableFSAA );
        };

        FB_STRUCT( render_system_config )
        {
            FB_CONSTRUCTOR( render_system_config() {
                fsaa = 0;
                vsync = false;
                fullscreen = false;
            } )

            FB_ATTR( vec4, resolution );
            FB_ATTR( int, fsaa );
            FB_ATTR( bool, vsync );
            FB_ATTR( bool, fullscreen );
        };
    }  // end namespace data
}  // end namespace fb

#include "FBCore/Data/Actor.h"
#include <FBCore/Data/SceneStructures.h>
#include "FBCore/Data/Component.h"
#include "FBCore/Data/PluginEvent.h"
#include "FBCore/Data/ProceduralScene.h"
#include "FBCore/Data/ProceduralWorld.h"
#include "FBCore/Data/OSM.h"
#include <FBCore/Data/AeroDataStructures.h>
#include <FBCore/Data/VehicleDataStructures.h>
#include <FBCore/Data/EngineStructures.h>
