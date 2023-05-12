#ifndef __IApplication_h__
#define __IApplication_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace core
    {

        /**
         * Interface for an application class.
         *
         * This class is used as a base class for implementing a game or application.
         *
         * @author Zane Desir
         * @version 1.0
         */
        class IApplicationClient : public ISharedObject
        {
        public:
            /**
             * Virtual destructor.
             */
            ~IApplicationClient() override = default;

            /**
             * Starts the main loop of the application.
             *
             * This method should be implemented to initialize the application and start the main game loop. The loop should
             * run continuously until the application is closed or exited.
             */
            virtual void run() = 0;

            /**
             * Gets the finite state machine used by the application.
             *
             * This method returns a shared pointer to the FSM used by the application. The FSM is used to control the flow of
             * the application and manage the game states.
             *
             * @return A shared pointer to the FSM used by the application.
             */
            virtual SmartPtr<IFSM> getFSM() const = 0;

            /**
             * Sets the finite state machine used by the application.
             *
             * This method sets the FSM used by the application to the specified FSM.
             *
             * @param fsm The new FSM to use for the application.
             */
            virtual void setFSM( SmartPtr<IFSM> fsm ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace core
}  // end namespace fb

#endif  // __IApplication_h__
