#ifndef SaracenTransformData_h__
#define SaracenTransformData_h__

#include <FBCore/Data/C/VectorData.h>

namespace fb
{
    class GameObjectTransformData
    {
    public:
        VectorData position;
        VectorData orientation;
        VectorData rotation;
        VectorData scale;
    };
}  // namespace fb

#endif  // SaracenTransformData_h__
