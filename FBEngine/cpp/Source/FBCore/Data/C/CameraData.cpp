#include "FBCore/FBCorePCH.h"
#include "FBCore/Data/C/CameraData.h"
#include "FBCore/Base/StringUtil.h"

namespace fb
{
    CameraData MakeSaracenCameraData()
    {
        using namespace fb;

        CameraData data;

        data.yFOV = 55.0f;

        // static const char* EMPTY_STR = "";

        // StringUtil::toBuffer(EMPTY_STR, data.padding, 12);

        return data;
    }

    CameraData::CameraData()
    {
        yFOV = 55.0f;

        constexpr int size = sizeof( CameraData );

        // static const char* EMPTY_STR = "";
        // StringUtil::toBuffer(EMPTY_STR, padding, 12);
    }
}  // namespace fb
