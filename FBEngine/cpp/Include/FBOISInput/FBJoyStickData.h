#ifndef FBJoyStickData_h__
#define FBJoyStickData_h__

#include <FBOISInput/FBOISInputPrerequisites.h>

namespace fb
{
    struct JoyStickData
    {
        JoyStickData();

        hash32 m_gameInputId;
        u32 m_joyStickIdx;
    };
}  // end namespace fb

#endif  // FBJoyStickData_h__
