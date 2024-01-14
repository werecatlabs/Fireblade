#ifndef StateMessageAnimationEnable_h__
#define StateMessageAnimationEnable_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{

    class StateMessageAnimationEnable : public StateMessage
    {
    public:
        StateMessageAnimationEnable();
        ~StateMessageAnimationEnable() override;

        String getName() const;
        void setName( const String &name );

        f32 getTime() const;
        void setTime( f32 time );

        bool getEnabled() const;
        void setEnabled( bool enabled );

        FB_CLASS_REGISTER_DECL;

    protected:
        String m_name;
        f32 m_time;
        bool m_isEnabled;
    };
}  // end namespace fb

#endif  // StateMessageAnimationEnable_h__
