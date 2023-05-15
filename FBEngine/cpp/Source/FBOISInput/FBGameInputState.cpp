#include <FBOISInput/FBGameInputState.h>
#include <FBCore/FBCore.h>

namespace fb
{
    GameInputState::GameInputState() : m_eventType( 0 ), m_action( 0 )
    {
    }

    GameInputState::~GameInputState()
    {
    }

    hash32 GameInputState::getEventType() const
    {
        return m_eventType;
    }

    void GameInputState::setEventType( hash32 eventType )
    {
        m_eventType = eventType;
    }

    hash32 GameInputState::getAction() const
    {
        return m_action;
    }

    void GameInputState::setAction( hash32 action )
    {
        m_action = action;
    }
}  // end namespace fb
