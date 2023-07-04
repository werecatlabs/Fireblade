#ifndef NGPlantLoader_h__
#define NGPlantLoader_h__

#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    class NGPlantLoader
    {
    public:
        static SmartPtr<IMesh> loadMesh( const String &meshName );
    };
} // end namespace fb

#endif  // NGPlantLoader_h__
