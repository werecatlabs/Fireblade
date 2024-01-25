#ifndef StateMessagePair_h__
#define StateMessagePair_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    template <class T, class U>
    class StateMessagePair : public StateMessage
    {
    public:
        StateMessagePair() = default;
        ~StateMessagePair() override = default;

        T getFirst() const
        {
            return m_first;
        }

        void setFirst( const T &value )
        {
            m_first = value;
        }

        U getSecond() const
        {
            return m_second;
        }

        void setSecond( const U &value )
        {
            m_second = value;
        }

        FB_CLASS_REGISTER_TEMPLATE_PAIR_DECL( StateMessagePair, T, U );

    protected:
        T m_first;
        U m_second;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE_PAIR_TYPEID( fb, StateMessagePair, T, U, StateMessage );

}  // end namespace fb

#endif  // StateMessagePair_h__
