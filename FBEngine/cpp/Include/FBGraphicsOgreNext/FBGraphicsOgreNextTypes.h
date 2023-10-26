#ifndef _FBGraphicsOgreNextTypes_H_
#define _FBGraphicsOgreNextTypes_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>

namespace fb
{

    class GraphicsOgreNextTypes
    {
    public:
        enum class SceneManagerType
        {
            SMT_GENERIC,
            SMT_OCTTREE,
            SMT_TERRAIN,

            SMT_COUNT
        };

        enum class CoordinateSystemType
        {
            CST_LEFT_HANDED,
            CST_RIGHT_HANDED,

            CST_COUNT
        };

        static const hash32 FRAME_EVENT_PRE_RENDER;
        static const hash32 FRAME_EVENT_RENDER_QUEUED;
        static const hash32 FRAME_EVENT_POST_RENDER;

        static const hash32 VISIBILITY_MASK_HASH;
        static const hash32 STATE_MESSAGE_CAST_SHADOWS;
        static const hash32 STATE_MESSAGE_RENDER_QUEUE;

        static const hash32 STATE_MESSAGE_ATTENUATION;
        static const hash32 STATE_MESSAGE_DIRECTION;
    };

}  // end namespace fb

#endif
