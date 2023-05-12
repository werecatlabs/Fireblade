#ifndef IAsyncOperation_h__
#define IAsyncOperation_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Function.h>

namespace fb
{

    /**
     * @brief Interface for an asynchronous operation, used to manage completion events.
     * Inherits from ISharedObject.
     */
    class IAsyncOperation : public ISharedObject
    {
    public:
        /**
         * @brief Destroy the IAsyncOperation object.
         */
        ~IAsyncOperation() override = default;

        /**
         * @brief Remove a completion event function from the async operation.
         *
         * @param func A function<void()> to be removed from the completion event list.
         */
        virtual void removeCompleteEvent( Function<void()> func ) = 0;

        /**
         * @brief Add a completion event function to the async operation.
         *
         * @param func A function<void()> to be added to the completion event list.
         */
        virtual void addCompleteEvent( Function<void()> func ) = 0;
    };

}  // end namespace fb

#endif  // IAsyncOperation_h__
