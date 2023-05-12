#ifndef CollisionTerrain_h__
#define CollisionTerrain_h__

#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {

        class CollisionTerrain : public Collision
        {
        public:
            CollisionTerrain();
            ~CollisionTerrain() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Handles component events within the context of the finite state machine (FSM).
             *
             * @param state The current state of the FSM.
             * @param eventType The type of event to handle.
             * @return The return type of the FSM after handling the event.
             */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionTerrain_h__
