#ifndef _FB_IEvent_h__
#define _FB_IEvent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for an event. */
    class IEvent : public ISharedObject
    {
    public:
        enum class Type
        {
            Loading,
            Object,
            Input,
            UI,
            Window,
            Scene,

            Count
        };

        static const hash_type loadingStateChanged;
        static const hash_type loadScene;
        static const hash_type handlePropertyButtonClick;

        static const hash_type handleTreeSelectionActivated;
        static const hash_type handleTreeNodeDoubleClicked;

        static const hash_type handlePropertyChanged;
        static const hash_type handleValueChanged;

        static const hash_type handleMouseClicked;

        static const hash_type handleSelection;

        static const hash_type handleDrop;
        static const hash_type handleDrag;

        static const hash_type inputEvent;
        static const hash_type updateEvent;

        static const hash_type handleEnterFrame;

        static const hash_type ACTIVATE_HASH;
        static const hash_type DEACTIVATE_HASH;

        static const hash_type UPDATE_HASH;
        static const hash_type HANDLE_MESSAGE_HASH;
        static const hash_type INITIALISE_START_HASH;
        static const hash_type INITIALISE_END_HASH;

        static const hash_type ADD_CHILD_HASH;
        static const hash_type REMOVE_CHILD_HASH;

        static const hash_type CHANGED_STATE_HASH;
        static const hash_type CHILD_CHANGED_STATE_HASH;

        static const hash_type TOGGLE_ENABLED_HASH;
        static const hash_type TOGGLE_HIGHLIGHT_HASH;

        static const hash_type VISIBLE_HASH;

        static const hash_type SHOW_HASH;
        static const hash_type HIDE_HASH;

        static const hash_type SELECT_HASH;
        static const hash_type DESELECT_HASH;

        static const hash_type GAIN_FOCUS_HASH;
        static const hash_type LOST_FOCUS_HASH;

        static const hash_type materialSetup;
        static const hash_type contactStart;
        static const hash_type contactEnd;
        static const hash_type contactBreak;
        static const hash_type noContact;
        static const hash_type contact;
        static const hash_type contactProcess;

        static const hash_type queued;
        static const hash_type execute;
        static const hash_type completed;

        static const hash_type stateChanged;
        static const hash_type stateMessage;

        static const hash_type transform;

        /** Virtual destructor. */
        ~IEvent() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IEvent_h__
