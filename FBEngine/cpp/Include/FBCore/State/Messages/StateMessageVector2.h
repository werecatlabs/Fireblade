#ifndef StateMessageVector2_h__
#define StateMessageVector2_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    //---------------------------------------------
    template <class T>
    class StateMessageVector2 : public StateMessage
    {
    public:
        StateMessageVector2() = default;

        ~StateMessageVector2() override = default;

        Vector2<T> getValue() const
        {
            return m_value;
        }

        void setValue( const Vector2<T> &value )
        {
            m_value = value;
        }

        FB_CLASS_REGISTER_TEMPLATE_DECL( StateMessageVector2, StateMessage );

    protected:
        Vector2<T> m_value = Vector2<T>::zero();
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, StateMessageVector2, StateMessage, StateMessage );

    using StateMessageVector2I = StateMessageVector2<s32>;
    using StateMessageVector2F = StateMessageVector2<f32>;
}  // end namespace fb

#endif  // StateMessageVector2_h__
