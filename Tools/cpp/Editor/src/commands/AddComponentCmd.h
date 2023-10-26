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
            SmartPtr<scene::IActor> m_actor;
            SmartPtr<scene::IComponent> m_component;
            SmartPtr<IFactory> m_factory;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // AddComponentCmd_h__
