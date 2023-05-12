namespace fb
{
    namespace data
    {
        FB_STRUCT_INHERIT( tag_data, public base_data )
        {
            FB_ATTR( std::string, k );
            FB_ATTR( std::string, v );
        };

        FB_STRUCT_INHERIT( member, public base_data )
        {
            FB_ATTR( std::string, type );
            FB_ATTR( std::string, ref );
            FB_ATTR( std::string, role );
        };

        FB_STRUCT_INHERIT( nd_data, public base_data )
        {
            FB_ATTR( std::string, ref );
        };

        FB_STRUCT_INHERIT( relation, public base_data )
        {
            FB_ATTR( std::string, id );
            FB_ATTR( int, uid );
            FB_ATTR( int, version );
            FB_ATTR( bool, visible );
            FB_ARRAY_PROPERTY( data::member, member );
        };

        FB_STRUCT_INHERIT( way_data, public base_data )
        {
            FB_ATTR( std::string, id );
            FB_ATTR( bool, visible );
            FB_ARRAY_PROPERTY( nd_data, nd );
            FB_ARRAY_PROPERTY( tag_data, tag );
        };

        FB_STRUCT_INHERIT( node_data, public base_data )
        {
            FB_ATTR( std::string, id );
            FB_ATTR( int, uid );
            FB_ATTR( int, version );
            FB_ATTR( bool, visible );
            FB_ATTR( float, lat );
            FB_ATTR( float, lon );
            FB_ARRAY_PROPERTY( tag_data, tag );
        };

        FB_STRUCT_INHERIT( bounds_data, public base_data )
        {
            FB_ATTR( float, minlat );
            FB_ATTR( float, minlon );
            FB_ATTR( float, maxlat );
            FB_ATTR( float, maxlon );
        };

        FB_STRUCT_INHERIT( osm, public base_data )
        {
            FB_ATTR( int, version );
            FB_ATTR( std::string, generator );
            FB_ARRAY_PROPERTY( bounds_data, bounds );
            FB_ARRAY_PROPERTY( node_data, node );
            FB_ARRAY_PROPERTY( way_data, way );
            FB_ARRAY_PROPERTY( data::relation, relation );
        };
    }  // end namespace data
}  // end namespace fb
