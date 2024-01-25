#ifndef FB_KeyFrame_h__
#define FB_KeyFrame_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class KeyFrame : public ISharedObject
    {
    public:
        KeyFrame();

        virtual ~KeyFrame();

        f32 getTime() const;

        void setTime( f32 time );

    protected:
        f32 m_time = 0.0f;
    };

}  // end namespace fb

#endif  // KeyFrame_h__
