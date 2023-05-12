#ifndef StateMessageBuffer_h__
#define StateMessageBuffer_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    class StateMessageBuffer : public StateMessage
    {
    public:
        StateMessageBuffer();
        ~StateMessageBuffer() override;

        u8 *getBuffer() const;
        void setBuffer( u8 *val );

        FB_CLASS_REGISTER_DECL;

    protected:
        u8 *m_buffer = nullptr;
    };
}  // namespace fb

#endif  // StateMessageBuffer_h__
