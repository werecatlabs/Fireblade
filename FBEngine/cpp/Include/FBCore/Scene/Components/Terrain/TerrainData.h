#ifndef TerrainData_h__
#define TerrainData_h__

#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    class TerrainData : public CSharedObject<ISharedObject>
    {
    public:
        TerrainData();
        ~TerrainData();
    };
}  // namespace fb

#endif  // TerrainData_h__
