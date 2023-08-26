#ifndef TerrainData_h__
#define TerrainData_h__

#include "FBCore/Interface/Memory/ISharedObject.h"

namespace fb
{
    class TerrainData : public ISharedObject
    {
    public:
        TerrainData();
        ~TerrainData();
    };
}  // namespace fb

#endif  // TerrainData_h__
