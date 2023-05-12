namespace fb
{
    namespace data
    {
        FB_STRUCT_INHERIT( PluginEvent, public base_data )
        {
            FB_ATTR( std::string, type );
            FB_ATTR( std::string, arg1 );
            FB_ATTR( std::string, arg2 );
            FB_ATTR( std::string, arg3 );
            FB_ATTR( std::string, arg4 );
        };
    }  // namespace data
}  // namespace fb
