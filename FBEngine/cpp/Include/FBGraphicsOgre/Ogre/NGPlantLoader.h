#ifndef NGPlantLoader_h__
#define NGPlantLoader_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    class OgreNGPlantLoader
    {
    public:
        static String loadMesh( const String &meshName );
    };

}  // end namespace fb

#endif  // NGPlantLoader_h__
