namespace fb
{
    namespace data
    {
        FB_STRUCT_INHERIT( procedural_world, public base_data )
        {
            FB_ARRAY_PROPERTY( procedural_scene, scenes );
        };
    }  // namespace data
}  // namespace fb
