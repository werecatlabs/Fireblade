#ifndef _StateChangedMessage_H_
#define _StateChangedMessage_H_

#include <FBCore/Interface/System/IStateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageStandard : public IStateMessage
    {
    public:
        StateMessageStandard();
        explicit StateMessageStandard( u32 subjectId );
        ~StateMessageStandard() override;

        void setSubjectId( u32 id );
        u32 getSubjectId() const;

    private:
        u32 m_subjectId = 0;
    };
}  // end namespace fb

#endif
