#ifndef StateMessageLoad_h__
#define StateMessageLoad_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageLoad : public StateMessage
    {
    public:
        static const hash_type LOAD_HASH;
        static const hash_type RELOAD_HASH;

        static const hash_type LOADED_HASH;
        static const hash_type UNLOADED_HASH;

        StateMessageLoad();
        StateMessageLoad( const StateMessageLoad &other ) = delete;
        ~StateMessageLoad() override;

        SmartPtr<ISharedObject> getObject() const;
        void setObject( SmartPtr<ISharedObject> object );

        FB_CLASS_REGISTER_DECL;

    protected:
        /// The object.
        SmartPtr<ISharedObject> m_object;
    };
}  // end namespace fb

#endif  // StateMessageLoad_h__
