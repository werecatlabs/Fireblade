#ifndef SaracenObjectTransform_h__
#define SaracenObjectTransform_h__

#include <FBCore/Data/C/VectorData.h>

namespace fb
{
    class ObjectTransformData
    {
    public:
        int objectReference = 0;

        VectorData position;
        VectorData orientation;
        VectorData scale;

        float reserved0 = 0.0f;
        float reserved1 = 0.0f;
        float reserved2 = 0.0f;
    };

    ObjectTransformData MakeSaracenObjectTransform();
}  // namespace fb

#endif  // SaracenObjectTransform_h__
