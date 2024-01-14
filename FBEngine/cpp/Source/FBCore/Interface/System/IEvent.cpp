#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IEvent, ISharedObject );

    const hash_type IEvent::loadingStateChanged = StringUtil::getHash( "loadingStateChanged" );
    const hash_type IEvent::loadScene = StringUtil::getHash( "loadScene" );
    const hash_type IEvent::handlePropertyButtonClick =
        StringUtil::getHash( "handlePropertyButtonClick" );

    const hash_type IEvent::handleTreeSelectionActivated =
        StringUtil::getHash( "handleTreeSelectionActivated" );
    const hash_type IEvent::handleTreeSelectionRelease =
        StringUtil::getHash( "handleTreeSelectionRelease" );
    const hash_type IEvent::handleTreeNodeDoubleClicked =
        StringUtil::getHash( "handleTreeNodeDoubleClicked" );

    const hash_type IEvent::addActor = StringUtil::getHash( "addActor" );
    const hash_type IEvent::removeActor = StringUtil::getHash( "removeActor" );

    const hash_type IEvent::handlePropertyChanged = StringUtil::getHash( "handlePropertyChanged" );
    const hash_type IEvent::handleValueChanged = StringUtil::getHash( "handleValueChanged" );

    const hash_type IEvent::handleMouseClicked = StringUtil::getHash( "handleMouseClicked" );
    const hash_type IEvent::handleMouseReleased = StringUtil::getHash( "handleMouseReleased" );

    const hash_type IEvent::handleSelection = StringUtil::getHash( "handleSelection" );

    const hash_type IEvent::handleDrop = StringUtil::getHash( "handleDrop" );
    const hash_type IEvent::handleDrag = StringUtil::getHash( "handleDrag" );

    const hash_type IEvent::inputEvent = StringUtil::getHash( "inputEvent" );
    const hash_type IEvent::updateEvent = StringUtil::getHash( "updateEvent" );

    const hash_type IEvent::handleEnterFrame = StringUtil::getHash( "handleEnterFrame" );

    const hash_type IEvent::CLICK_HASH = StringUtil::getHash( "click" );
    const hash_type IEvent::ACTIVATE_HASH = StringUtil::getHash( "activate" );
    const hash_type IEvent::UPDATE_HASH = StringUtil::getHash( "update" );
    const hash_type IEvent::HANDLE_MESSAGE_HASH = StringUtil::getHash( "handleMessage" );
    const hash_type IEvent::INITIALISE_START_HASH = StringUtil::getHash( "initialiseStart" );
    const hash_type IEvent::INITIALISE_END_HASH = StringUtil::getHash( "initialiseEnd" );

    const hash_type IEvent::ADD_CHILD_HASH = StringUtil::getHash( "addChild" );
    const hash_type IEvent::REMOVE_CHILD_HASH = StringUtil::getHash( "removeChild" );

    const hash_type IEvent::CHANGED_STATE_HASH = StringUtil::getHash( "changedState" );
    const hash_type IEvent::CHILD_CHANGED_STATE_HASH = StringUtil::getHash( "childChangedState" );

    const hash_type IEvent::TOGGLE_ENABLED_HASH = StringUtil::getHash( "toggleEnabled" );

    const hash_type IEvent::TOGGLE_HIGHLIGHT_HASH = StringUtil::getHash( "toggleHighlight" );

    const hash_type IEvent::VISIBLE_HASH = StringUtil::getHash( "visible" );

    const hash_type IEvent::SHOW_HASH = StringUtil::getHash( "show" );
    const hash_type IEvent::HIDE_HASH = StringUtil::getHash( "hide" );

    const hash_type IEvent::DEACTIVATE_HASH = StringUtil::getHash( "deactivate" );

    const hash_type IEvent::SELECT_HASH = StringUtil::getHash( "select" );
    const hash_type IEvent::DESELECT_HASH = StringUtil::getHash( "deselect" );

    const hash_type IEvent::GAIN_FOCUS_HASH = StringUtil::getHash( "gainFocus" );
    const hash_type IEvent::LOST_FOCUS_HASH = StringUtil::getHash( "lostFocus" );
    const hash_type IEvent::childChangedState = StringUtil::getHash( "childChangedState" );

    const hash_type IEvent::materialSetup = StringUtil::getHash( "materialSetup" );
    const hash_type IEvent::contactStart = StringUtil::getHash( "contactStart" );
    const hash_type IEvent::contactEnd = StringUtil::getHash( "contactEnd" );
    const hash_type IEvent::contactBreak = StringUtil::getHash( "contactBreak" );
    const hash_type IEvent::noContact = StringUtil::getHash( "noContact" );
    const hash_type IEvent::contact = StringUtil::getHash( "contact" );
    const hash_type IEvent::contactProcess = StringUtil::getHash( "contactProcess" );

    const hash_type IEvent::queued = StringUtil::getHash( "queued" );
    const hash_type IEvent::execute = StringUtil::getHash( "execute" );
    const hash_type IEvent::completed = StringUtil::getHash( "completed" );

    const hash_type IEvent::stateChanged = StringUtil::getHash( "stateChanged" );
    const hash_type IEvent::stateMessage = StringUtil::getHash( "stateMessage" );

    const hash_type IEvent::transform = StringUtil::getHash( "transform" );

    const hash_type IEvent::addSelectedObject = StringUtil::getHash( "addSelectedObject" );
    const hash_type IEvent::addSelectedObjects = StringUtil::getHash( "addSelectedObjects" );
    const hash_type IEvent::deselectObjects = StringUtil::getHash( "deselectObjects" );
    const hash_type IEvent::deselectAll = StringUtil::getHash( "deselectAll" );

    const hash_type IEvent::addCommand = StringUtil::getHash( "addCommand" );
    const hash_type IEvent::getNextCommand = StringUtil::getHash( "getNextCommand" );
    const hash_type IEvent::getPreviousCommand = StringUtil::getHash( "getPreviousCommand" );

}  // end namespace fb
