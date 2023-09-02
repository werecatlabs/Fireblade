#ifndef FBOISGameInputMap_h__
#define FBOISGameInputMap_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include "FBCore/Interface/Input/IGameInputMap.h"
#include "FBCore/Interface/Input/IInputAction.h"
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/HashMap.h>
#include <map>
#include <vector>

namespace fb
{
    using ActionKeyMap = std::map<u32, SmartPtr<IInputAction>>;

    class OISGameInputMap : public IGameInputMap
    {
    public:
        OISGameInputMap();
        ~OISGameInputMap() override;

        u32 getActionFromKey( u32 key ) const override;

        void setKeyboardAction( u32 id, const String &key0, const String &key1 ) override;
        void getKeyboardAction( u32 id, String &key0, String &key1 ) override;

        u32 getActionFromButton( u32 button ) const override;

        void getJoystickAction( u32 id, u32 &button0, u32 &button1 ) override;
        void setJoystickAction( u32 id, u32 button0, u32 button1 ) override;

        const ActionKeyMap &getKeyboardMap() const;
        const ActionKeyMap &getJoystickMap() const;

        bool getInputActionData( u32 button, SmartPtr<IInputAction> &data );

    protected:
        /** */
        void setKeyboardAction( u32 id, const SmartPtr<IInputAction> &actionData );

        /** */
        void setJoystickAction( u32 id, const SmartPtr<IInputAction> &actionData );

        ///
        ActionKeyMap m_keyboardMap;

        ///
        ActionKeyMap m_joystickMap;
    };
}  // namespace fb

#endif  // FBOISGameInputMap_h__
