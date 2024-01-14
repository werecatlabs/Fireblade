#ifndef StateMessageSetFlag_h__
#define StateMessageSetFlag_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    class StateMessageSetFlag : public StateMessage
    {
    public:
        StateMessageSetFlag();
        ~StateMessageSetFlag() override;

        u32 getFlags() const;
        void setFlags( u32 flags );

    protected:
        u32 m_flags = 0;
    };
}  // namespace fb

#endif  // StateMessageSetFlag_h__
