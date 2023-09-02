#ifndef AddComponentCmd_h__
#define AddComponentCmd_h__

#include <commands/Command.h>

namespace fb
{
    namespace editor
    {
        class AddComponentCmd : public Command
        {
        public:
            AddComponentCmd() = default;
            ~AddComponentCmd() override = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void undo() override;
            void redo() override;
            void execute() override;

            SmartPtr<IFactory> getFactory() const;

            void setFactory( SmartPtr<IFactory> factory );

            SmartPtr<scene::IComponent> getComponent() const;

            void setComponent( SmartPtr<scene::IComponent> component );

            SmartPtr<scene::IActor> getActor() const;

            void setActor( SmartPtr<scene::IActor> actor );

        protected:
            AtomicSmartPtr<scene::IActor> m_actor;
            AtomicSmartPtr<scene::IComponent> m_component;
            AtomicSmartPtr<IFactory> m_factory;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // AddComponentCmd_h__
