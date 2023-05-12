#include <FBCore/Base/StringTypes.h>

namespace fb
{
    namespace data
    {
        FB_STRUCT( tree_instance_data )
        {
            FB_CONSTRUCTOR( tree_instance_data() {
                heightScale = 0;
                rotation = 0;
                widthScale = 0;
                prototypeIndex = 0;
            } )

            FB_ATTR( vec4, position );
            FB_ATTR( vec4, orientation );
            FB_ATTR( vec4, scale );
            FB_ATTR( vec4, color );
            FB_ATTR( vec4, lightmapColor );
            FB_ATTR( float, heightScale );
            FB_ATTR( float, rotation );
            FB_ATTR( float, widthScale );
            FB_ATTR( int, prototypeIndex );
        };

        FB_STRUCT( tree_prototype_data )
        {
            FB_ATTR( actor_data, prefabData );
        };

        FB_STRUCT( terrain_data )
        {
            FB_CHILD( tree_prototype_data, treePrototypeData );
            FB_CHILD( tree_instance_data, treeInstanceData );
        };
    }  // end namespace data
}  // end namespace fb
