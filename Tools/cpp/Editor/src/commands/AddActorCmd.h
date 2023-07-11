#ifndef AddEntityCmd_h__
#define AddEntityCmd_h__

#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        class AddActorCmd : public Command
        {
        public:
            enum class ActorType
            {
                Actor,
                Button,
                Camera,
                Car,
                Canvas,
                Cube,
                CubeMesh,
                Cubemap,
                CubeGround,
                DirectionalLight,
                Panel,
                ParticleSystem,
                ParticleSystemSmoke,
                ParticleSystemSand,
                Plane,
                ProceduralScene,
                PointLight,
                PhysicsCube,
                Skybox,
                SimpleButton,
                Terrain,
                Text,
                Vehicle,

                EmptyActor
            };

            AddActorCmd() = default;
            ~AddActorCmd() override = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void undo() override;
            void redo() override;
            void execute() override;

            String getCommandId() const;

            SmartPtr<scene::IActor> getActor() const;
            void setActor( SmartPtr<scene::IActor> actor );

            SmartPtr<scene::IActor> getParent() const;
            void setParent( SmartPtr<scene::IActor> parent );

            ActorType getActorType() const;
            void setActorType( ActorType actorType );

            String getFilePath() const;
            void setFilePath( const String &filePath );

        protected:
            SmartPtr<scene::IActor> createActor();

            SmartPtr<scene::IActor> m_actor;
            SmartPtr<scene::IActor> m_parent;
            ActorType m_actorType = ActorType::EmptyActor;
            String m_filePath;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // AddEntityCmd_h__
