#ifndef _FBOgreTypes_H_
#define _FBOgreTypes_H_

#include <FBCore/Core/StringUtil.h>

namespace fb
{

    enum SceneManagerType
    {
        SMT_GENERIC,
        SMT_OCTTREE,
        SMT_TERRAIN,

        SMT_COUNT
    };

    enum CoordinateSystemType
    {
        CST_LEFT_HANDED,
        CST_RIGHT_HANDED,

        CST_COUNT
    };

    static const hash_type FRAME_EVENT_PRE_RENDER = StringUtil::getHash( "pre_render" );
    static const hash_type FRAME_EVENT_RENDER_QUEUED = StringUtil::getHash( "render_queued" );
    static const hash_type FRAME_EVENT_POST_RENDER = StringUtil::getHash( "post_render" );

}  // end namespace fb

#endif
