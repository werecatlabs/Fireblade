namespace fb
{
    namespace data
    {
        FB_STRUCT_INHERIT( procedural_scene, public base_data )
        {
            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( city_data, cities );
            FB_ARRAY_PROPERTY( terrain_data, terrainData );
        };
    }  // namespace data
}  // namespace fb
