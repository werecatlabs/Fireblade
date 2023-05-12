namespace fb
{
    namespace data
    {
        FB_STRUCT( base_data )
        {
            FB_ATTR( int, base_id );
        };

        FB_STRUCT_INHERIT( attribute, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, value );
        };

        FB_STRUCT_INHERIT( property, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, value );
            FB_CHILD( attribute, attributes );
        };

        FB_STRUCT_INHERIT( properties, public base_data )
        {
            FB_ARRAY_PROPERTY( property, properties_ );
        };

        FB_STRUCT_INHERIT( vec4, public base_data )
        {
            FB_CONSTRUCTOR( vec4() { x = y = z = w = 0.0f; } )

            FB_CONSTRUCTOR( vec4( float fX, float fY, float fZ, float fW = 0.0f ) {
                x = fX;
                y = fY;
                z = fZ;
                w = fW;
            } )

            FB_ATTR( float, x );
            FB_ATTR( float, y );
            FB_ATTR( float, z );
            FB_ATTR( float, w );
        };

        FB_STRUCT_INHERIT( transform_data, public base_data )
        {
            FB_CONSTRUCTOR( transform_data() {
                orientation.x = orientation.y = orientation.z = 0.0f;
                orientation.w = 1.0f;
                scale.x = scale.y = scale.z = scale.w = 1.0f;
            } )

            FB_ATTR( vec4, position );
            FB_ATTR( vec4, orientation );
            FB_ATTR( vec4, scale );
        };

        FB_STRUCT( vertex )
        {
            FB_ATTR( vec4, position );
            FB_ATTR( vec4, normal );
            FB_ATTR( vec4, tangent );
            FB_ATTR( vec4, binormal );
            FB_ARRAY_PROPERTY( vec4, texcoords );
        };

        FB_STRUCT_INHERIT( sub_mesh, public base_data )
        {
            FB_CONSTRUCTOR( sub_mesh() { useSharedVertices = false; } )

            FB_ATTR( std::string, materialName );
            FB_ATTR( bool, useSharedVertices );
            FB_ARRAY_PROPERTY( vertex, vertices );
            FB_ARRAY_PROPERTY( int, indices );
        };

        FB_STRUCT_INHERIT( mesh, public base_data )
        {
            FB_CONSTRUCTOR( mesh() { hasSharedVertexData = 0; } )

            FB_CHILD( vertex, vertices );
            FB_ATTR( bool, hasSharedVertexData );
            FB_ATTR( std::string, name );
            FB_CHILD( sub_mesh, subMeshes );
        };

        FB_STRUCT_INHERIT( texture_map, public base_data )
        {
            FB_ATTR( std::string, type );
            FB_ATTR( std::string, path );
            FB_ATTR( vec4, values );
            FB_ATTR( vec4, colour );
        };

        FB_STRUCT_INHERIT( material, public base_data )
        {
            FB_CONSTRUCTOR( material() {
                shininess = 0.0f;
                smoothness = 0.5f;
                uvSet = 0;
            } )

            FB_ATTR( std::string, name );
            FB_ATTR( std::string, uuid );
            FB_ATTR( std::string, mainTexture );
            FB_ATTR( std::string, normalTexture );
            FB_ATTR( std::string, shader );

            FB_ATTR( vec4, tiling );
            FB_ATTR( vec4, offset );

            FB_ATTR( vec4, tiling1 );
            FB_ATTR( vec4, offset1 );

            FB_CHILD( texture_map, textureMaps );

            FB_ATTR( float, shininess );
            FB_ATTR( float, smoothness );
            FB_ATTR( int, uvSet );

            FB_ARRAY_PROPERTY( data::property, properties );
        };

        FB_STRUCT_INHERIT( material_data, public base_data )
        {
            FB_ARRAY_PROPERTY( material, materials );
        };

        FB_STRUCT_INHERIT( material_node, public base_data )
        {
            FB_ATTR( std::string, name );
        };

        FB_STRUCT_INHERIT( material_texture, public material_node )
        {
            FB_ATTR( std::string, textureType );
            FB_ATTR( std::string, texturePath );
            FB_ATTR( vec4, tiling );
            FB_ATTR( vec4, offset );
        };

        FB_STRUCT_INHERIT( material_pass, public material_node )
        {
            FB_CONSTRUCTOR( material_pass() {
                metalness = 0.5f;
                roughness = 0.5f;
                uvSet = 0;
            } )

            FB_ATTR( vec4, tint );
            FB_ATTR( vec4, ambient );
            FB_ATTR( vec4, diffuse );
            FB_ATTR( vec4, specular );
            FB_ATTR( vec4, emissive );

            FB_ATTR( float, metalness );
            FB_ATTR( float, roughness );
            FB_ATTR( int, uvSet );
            FB_ARRAY_PROPERTY( material_texture, textures );
            FB_ARRAY_PROPERTY( data::property, properties );
        };

        FB_STRUCT_INHERIT( material_scheme, public material_node )
        {
            FB_ARRAY_PROPERTY( material_pass, passes );
        };

        FB_STRUCT_INHERIT( material_graph, public base_data )
        {
            FB_CONSTRUCTOR( material_graph() { materialType = 0; } )

            FB_ARRAY_PROPERTY( material_scheme, schemes );
            FB_ATTR( int, materialType );
        };

        FB_STRUCT_INHERIT( debug_draw, public base_data )
        {
            FB_CONSTRUCTOR( debug_draw() { id = 0; } )

            FB_ATTR( int, id );
            FB_ATTR( vec4, start );
            FB_ATTR( vec4, end );
        };
    }  // end namespace data
}  // end namespace fb
