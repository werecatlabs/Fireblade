#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/ObjectTransformData.h"
#include "FBCore/Base/StringUtil.h"

namespace fb
{
    ObjectTransformData MakeSaracenObjectTransform()
    {
        using namespace fb;

        ObjectTransformData objectTransform;
        objectTransform.objectReference = 0;

        objectTransform.position = getSaracenVec( Vector3<real_Num>::zero() );
        objectTransform.scale = getSaracenVec( Vector3<real_Num>::unit() );
        objectTransform.orientation = getSaracenQuat( Quaternion<real_Num>::identity() );

        // static const char* EMPTY_STR = "";
        // StringUtil::toBuffer(EMPTY_STR, objectTransform.padding, 12);

        return objectTransform;
    }
}  // namespace fb
