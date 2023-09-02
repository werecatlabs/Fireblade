#ifndef __FB__Component_FiniteStateMachine_h__
#define __FB__Component_FiniteStateMachine_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        
        class FiniteStateMachine : public Component
        {
        public:
            FiniteStateMachine();
            ~FiniteStateMachine() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<IFSM> m_fsm;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // FiniteStateMachine_h__
