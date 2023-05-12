#ifndef IStateListener_h__
#define IStateListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @class IStateListener
     * @brief An interface for classes that listen to state messages, inheriting from ISharedObject.
     */
    class IStateListener : public ISharedObject
    {
    public:
        /** @brief Virtual destructor. */
        ~IStateListener() override = default;

        /**
         * @brief An event function used to listen for state messages.
         * @param message The message object passed to the listener.
         */
        virtual void handleStateChanged( const SmartPtr<IStateMessage> &message ) = 0;

        /**
         * @brief An event function used to listen for state changes.
         * @param state The state object passed to the listener.
         */
        virtual void handleStateChanged( SmartPtr<IState> &state ) = 0;

        /**
         * @brief An event function used to handle object queries.
         * @param query The query object passed to the listener.
         */
        virtual void handleQuery( SmartPtr<IStateQuery> &query ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IStateListener_h__
