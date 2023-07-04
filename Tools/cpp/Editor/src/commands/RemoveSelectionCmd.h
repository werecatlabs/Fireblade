#ifndef RemoveSelectionCmd_h__
#define RemoveSelectionCmd_h__

#include <commands/Command.h>

namespace fb
{
    namespace editor
    {
        class RemoveSelectionCmd : public Command
        {
        public:
            RemoveSelectionCmd();
            ~RemoveSelectionCmd() override;

            void undo() override;
            void redo() override;
            void execute() override;

        protected:
            class ActorData : public SharedObject<ISharedObject>
            {
            public:
                ActorData();
                ~ActorData() override;

                SmartPtr<scene::IActor> getParent() const;
                void setParent( SmartPtr<scene::IActor> val );

                SmartPtr<scene::IActor> getActor() const;
                void setActor( SmartPtr<scene::IActor> val );

                SmartPtr<IData> getActorData() const;
                void setActorData( SmartPtr<IData> val );

            private:
                SmartPtr<scene::IActor> m_parent;
                SmartPtr<scene::IActor> m_actor;
                SmartPtr<IData> m_actorData;
            };

            Array<SmartPtr<ActorData>> m_actorData;
        };
    } // end namespace editor
}     // end namespace fb

#endif  // RemoveSelectionCmd_h__
