#ifndef SaracenTransform_h__
#define SaracenTransform_h__

#include <FBCore/Data/C/VectorData.h>

namespace fb
{
    class TransformData
    {
    public:
        TransformData();
        TransformData( VectorData pos, VectorData ori, VectorData scale );

        TransformData &operator=( const TransformData &other );

        bool isFinite() const;

        bool isValid() const;

        VectorData position;
        VectorData orientation;
        VectorData scale;
    };

    extern TransformData MakeSaracenTransform();
}  // namespace fb

#endif  // SaracenTransform_h__
