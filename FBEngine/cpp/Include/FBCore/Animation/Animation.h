#ifndef CAnimation_h__
#define CAnimation_h__

#include <FBCore/Interface/Animation/IAnimation.h>

namespace fb
{

    class Animation : public IAnimation
    {
    public:
        Animation();
        ~Animation();

        String getName() const;
        void setName( const String &name );

        f32 getLength() const;
        void setLength( f32 len );
    };

}  // end namespace fb

#endif  // CAnimation_h__
