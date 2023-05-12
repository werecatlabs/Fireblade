#ifndef StateMessageFloatValue_h__
#define StateMessageFloatValue_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //--------------------------------------------
    class StateMessageFloatValue : public StateMessage
    {
    public:
        static const hash_type LEFT_HASH;
        static const hash_type TOP_HASH;
        static const hash_type WIDTH_HASH;
        static const hash_type HEIGHT_HASH;

        StateMessageFloatValue() = default;
        ~StateMessageFloatValue() override = default;

        f32 getValue() const;
        void setValue( f32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        f32 m_value = 0.0f;
    };
}  // end namespace fb

#endif  // StateMessageFloatValue_h__
