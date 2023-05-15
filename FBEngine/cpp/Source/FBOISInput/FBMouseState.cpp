#include <FBOISInput/FBMouseState.h>
#include <FBCore/FBCore.h>

namespace fb
{
    MouseState::MouseState() :
        m_wheelPosition( 0.0f ),
        m_eventType( static_cast<Event>( 0 ) ),
        m_isShiftPressed( false ),
        m_isControlPressed( false )
    {
    }

    MouseState::~MouseState()
    {
    }

    Vector2F MouseState::getRelativeMove() const
    {
        return m_movePosition;
    }

    void MouseState::setRelativeMove( const Vector2F &movePosition )
    {
        m_movePosition = movePosition;
    }

    Vector2F MouseState::getRelativePosition() const
    {
        return m_relativePosition;
    }

    void MouseState::setRelativePosition( const Vector2F &position )
    {
        m_relativePosition = position;
    }

    Vector2F MouseState::getAbsolutePosition() const
    {
        return m_absolutePosition;
    }

    void MouseState::setAbsolutePosition( const Vector2F &position )
    {
        m_absolutePosition = position;
    }

    f32 MouseState::getWheelPosition() const
    {
        return m_wheelPosition;
    }

    void MouseState::setWheelPosition( f32 wheelPosition )
    {
        m_wheelPosition = wheelPosition;
    }

    bool MouseState::isShiftPressed() const
    {
        return m_isShiftPressed;
    }

    void MouseState::setShiftPressed( bool shiftPressed )
    {
        m_isShiftPressed = shiftPressed;
    }

    bool MouseState::isControlPressed() const
    {
        return m_isControlPressed;
    }

    void MouseState::setControlPressed( bool controlPressed )
    {
        m_isControlPressed = controlPressed;
    }

    bool MouseState::isButtonPressed( u32 id ) const
    {
        auto it = m_mouseButtonStates.find( id );
        if( it != m_mouseButtonStates.end() )
        {
            return it->second;
        }

        return false;
    }

    void MouseState::setButtonPressed( u32 id, bool pressed )
    {
        m_mouseButtonStates[id] = pressed;
    }

    IMouseState::Event MouseState::getEventType() const
    {
        return m_eventType;
    }

    void MouseState::setEventType( Event eventType )
    {
        m_eventType = eventType;
    }
}  // end namespace fb
