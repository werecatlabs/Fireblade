namespace fb
{
    namespace data
    {

        FB_STRUCT_INHERIT( director_data, public base_data )
        {
            FB_ARRAY_PROPERTY( properties, properties_ );
            FB_ARRAY_PROPERTY( director_data, children );
        };

        FB_STRUCT_INHERIT( component_eventhandler_data, public base_data )
        {
            FB_ARRAY_PROPERTY( properties, listeners );
        };

        FB_STRUCT( sub_component_data )
        {
            FB_CONSTRUCTOR( sub_component_data() { componentId = 0; } )

            FB_ATTR( int, componentId );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, uuid );
            FB_ATTR( std::string, componentType );
            FB_ATTR( properties, properties_ );
        };

        FB_STRUCT( component_data )
        {
            FB_CONSTRUCTOR( component_data() { componentId = 0; } )

            FB_ATTR( int, componentId );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, uuid );
            FB_ATTR( std::string, componentType );
            FB_ATTR( properties, properties_ );
            FB_ARRAY_PROPERTY( sub_component_data, subComponentData );
            FB_ARRAY_PROPERTY( component_eventhandler_data, events );
        };

        FB_STRUCT_INHERIT( mesh_component_data, public component_data )
        {
            FB_ATTR( std::string, mesh );
        };

        FB_STRUCT_INHERIT( renderer_data, public component_data )
        {
            FB_ATTR( std::string, reserved );
        };

        FB_STRUCT_INHERIT( mesh_renderer_data, public renderer_data )
        {
            FB_ATTR( std::string, materials );
            FB_ATTR( int, castShadows );
            FB_ATTR( int, recieveShadows );
            FB_ATTR( int, reflections );
            FB_ATTR( int, occulsion );
        };

        FB_STRUCT_INHERIT( actor_data, public base_data )
        {
            FB_CONSTRUCTOR( actor_data() {
                actorId = 0;
                isStatic = false;
                isVisible = true;
                isEnabled = true;
            } )

            FB_ATTR( int, actorId );
            FB_ATTR( bool, isStatic );
            FB_ATTR( bool, isVisible );
            FB_ATTR( bool, isEnabled );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, uuid );
            FB_ATTR( std::string, label );

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ARRAY_PROPERTY( component_data, components );
            FB_ARRAY_PROPERTY( actor_data, children );
        };

        FB_STRUCT_INHERIT( fb_scene, public base_data )
        {
            FB_ARRAY_PROPERTY( actor_data, actors );
        };

        FB_STRUCT( json_component )
        {
            FB_CONSTRUCTOR( json_component() { componentId = 0; } )

            FB_ATTR( int, componentId );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, parentName );
            FB_ATTR( std::string, type );
            FB_ATTR( properties, properties_ );
        };

        FB_STRUCT_INHERIT( json_actor, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( json_actor, children );
        };

        FB_STRUCT_INHERIT( json_scene, public base_data )
        {
            FB_ARRAY_PROPERTY( json_actor, hierarchy );
        };
    }  // end namespace data
}  // end namespace fb
