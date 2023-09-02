#ifndef FBGameInputState_h__
#define FBGameInputState_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IGameInputState.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class GameInputState : public IGameInputState
    {
    public:
        GameInputState();
        ~GameInputState() override;

        hash32 getEventType() const override;
        void setEventType( hash32 eventType ) override;

        hash32 getAction() const override;
        void setAction( hash32 action ) override;

    protected:
        hash32 m_eventType = 0;
        hash32 m_action = 0;
    };
}  // end namespace fb

#endif  // FBGameInputState_h__
