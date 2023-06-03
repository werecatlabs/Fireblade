#ifndef TerrainData_h__
#define TerrainData_h__

#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    class TerrainData : public SharedObject<ISharedObject>
    {
    public:
        TerrainData();
        ~TerrainData();
    };
}  // namespace fb

#endif  // TerrainData_h__
