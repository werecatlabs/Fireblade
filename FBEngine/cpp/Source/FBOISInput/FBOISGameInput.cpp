#include <FBOISInput/FBOISGameInput.h>
#include <FBOISInput/FBOISInputManager.h>
#include <FBOISInput/FBJoyStickData.h>
#include <FBOISInput/FBOISGameInputMap.h>
#include <FBOISInput/FBOISKeyConverter.h>
#include <FBCore/FBCore.h>
#include <OIS.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, OISGameInput, IGameInput );

    OISGameInput::OISGameInput( OISInputManager *inputManager ) :
        // m_inputMap(SmartPtr<IGameInputMap>::NULL_PTR),
        m_inputManager( inputManager ),
        m_joyStick( nullptr ),

        m_userData( nullptr ),
        m_id( 0 ),
        m_pov( IJoystickState::POV_Centered ),

        m_playerIndex( 0 ),
        m_joystickId( 0 ),
        m_poll( false ),
        m_handleEvents( true )
    {
        m_inputMap = SmartPtr<IGameInputMap>( new OISGameInputMap() );
    }

    OISGameInput::~OISGameInput()
    {
        m_joyStick = nullptr;
        m_userData = nullptr;
        m_inputManager = nullptr;
    }

    void OISGameInput::update()
    {
    }

    bool OISGameInput::getHandleEvents() const
    {
        return m_handleEvents;
    }

    void OISGameInput::setHandleEvents( bool handleEvents )
    {
        m_handleEvents = handleEvents;
    }

    bool OISGameInput::getPollHardware() const
    {
        return m_poll;
    }

    void OISGameInput::setPollHardware( bool poll )
    {
        m_poll = poll;
    }

    u32 OISGameInput::getPOVDirection() const
    {
        return m_pov;
    }

    void OISGameInput::setPOVDirection( u32 pov )
    {
        m_pov = pov;
    }

    void OISGameInput::setJoyStick( OIS::JoyStick *val )
    {
        m_joyStick = val;
    }

    OIS::JoyStick *OISGameInput::getJoyStick() const
    {
        return m_joyStick;
    }

    bool OISGameInput::isAssigned() const
    {
        return m_joyStick != nullptr;
    }

    void OISGameInput::setUserData( void *val )
    {
        m_userData = val;
    }

    void *OISGameInput::getUserData() const
    {
        return m_userData;
    }

    u32 OISGameInput::getJoystickId() const
    {
        return m_joystickId;
    }

    void OISGameInput::setJoystickId( u32 joystickId )
    {
        // m_joystickId = val;

        // Engine* engine = Engine::getSingletonPtr();
        // SmartPtr<OISInputManager> inputManager = engine->getInputManager();

        // Array< WeakPtr<OIS::JoyStick> > joySticks = m_inputManager->getJoySticks();
        // if(!joySticks.empty() && m_joystickId < joySticks.size())
        //{
        //	m_joyStick = joySticks[m_joystickId];

        //	JoyStickData* joyStickData = (JoyStickData*)inputManager->getUserData(m_joyStick.get());
        //	joyStickData->m_gameInputId = this->getId();
        //}
    }

    void OISGameInput::setPlayerIndex( u32 val )
    {
        m_playerIndex = val;
    }

    u32 OISGameInput::getPlayerIndex() const
    {
        return m_playerIndex;
    }

    SmartPtr<IGameInputMap> OISGameInput::getGameInputMap() const
    {
        return m_inputMap;
    }

    void OISGameInput::setId( hash32 id )
    {
        m_id = id;
    }

    hash32 OISGameInput::getId() const
    {
        return m_id;
    }
}  // end namespace fb
