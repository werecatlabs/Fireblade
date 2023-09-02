#include <GameEditorPCH.h>
#include <commands/PromptCmd.h>
#include <FBCore/FBCore.h>
#include <ai/PromptEvaluator.h>
#include <ai/AddPromptEvaluator.h>
#include <ai/RemovePromptEvaluator.h>
#include "AddActorCmd.h"
#include "RemoveSelectionCmd.h"

namespace fb
{
    namespace editor
    {

        PromptCmd::PromptCmd()
        {
            goalEvaluators.reserve( 32 );

            goalEvaluators.push_back( fb::make_ptr<AddPromptEvaluator>(
                AddActorCmd::ActorType::Actor, "actor",
                Array<String>( { "make", "actor", "gameobject", "game", "object" } ) ) );
            goalEvaluators.push_back(
                fb::make_ptr<AddPromptEvaluator>( AddActorCmd::ActorType::DirectionalLight, "light",
                                               Array<String>( { "make", "light" } ) ) );
            goalEvaluators.push_back( fb::make_ptr<AddPromptEvaluator>(
                AddActorCmd::ActorType::PointLight, "light",
                Array<String>( { "create", "make", "point", "light" } ) ) );
            goalEvaluators.push_back( fb::make_ptr<AddPromptEvaluator>(
                AddActorCmd::ActorType::Cube, "cube", Array<String>( { "create", "make", "cube" } ) ) );
            goalEvaluators.push_back(
                fb::make_ptr<AddPromptEvaluator>( AddActorCmd::ActorType::Cube, "sphere",
                                               Array<String>( { "create", "make", "sphere" } ) ) );
            goalEvaluators.push_back(
                fb::make_ptr<AddPromptEvaluator>( AddActorCmd::ActorType::Terrain, "terrain",
                                               Array<String>( { "create", "make", "terrain" } ) ) );

            goalEvaluators.push_back(
                fb::make_ptr<AddPromptEvaluator>( AddActorCmd::ActorType::Vehicle, "terrain",
                                               Array<String>( { "create", "make", "car" } ) ) );

            goalEvaluators.push_back( fb::make_ptr<AddPromptEvaluator>(
                AddActorCmd::ActorType::Skybox, "skybox",
                Array<String>( { "create", "make", "skybox", "sky", "skydome" } ) ) );

            goalEvaluators.push_back( fb::make_ptr<RemovePromptEvaluator>(
                "remove", Array<String>( { "destroy", "remove", "delete" } ) ) );
        }

        PromptCmd::~PromptCmd()
        {
        }

        void PromptCmd::undo()
        {
        }

        void PromptCmd::redo()
        {
        }

        void PromptCmd::execute()
        {
            auto prompt = getPrompt();
            processAIPrompt( prompt );
        }


        void PromptCmd::processAIPrompt( const String &userInput )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto taskManager = applicationManager->getTaskManager();
            auto renderLock = taskManager->lockTask( Thread::Task::Render );
            auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

            // User input
            // Tokenization
            std::vector<std::string> tokens = StringUtil::tokenize( userInput );

            // Dependency parsing (not implemented in this example)

            // Named Entity Recognition (NER)
            std::vector<std::string> namedEntities;// = StringUtil::extractNamedEntities( tokens );
            namedEntities = tokens;

            SmartPtr<PromptEvaluator> bestMatch;

            // Extract goal
            std::string goal;
            for( const std::string &entity : namedEntities )
            {
                // Check if the entity matches any keyword for goals
                if( entity == "home" )
                {
                    goal = "drive to " + entity;
                    break;
                }

                auto highestRating = 0.0f;

                for( auto goalEvaluator : goalEvaluators )
                {
                    auto promptGoalEvaluator = dynamic_cast<PromptEvaluator *>( goalEvaluator.get() );
                    promptGoalEvaluator->namedEntities = namedEntities;

                    auto rating = goalEvaluator->getRating();
                    if( rating > highestRating )
                    {
                        highestRating = rating;
                        bestMatch = goalEvaluator;
                    }
                }
            }

            if( bestMatch )
            {
                std::cout << "Extracted Goal: " << bestMatch->m_label << std::endl;
                bestMatch->activateGoal();
            }
            else
            {
                std::cout << "No goal extracted." << std::endl;
            }
        }

        String PromptCmd::getPrompt() const
        {
            return m_prompt;
        }

        void PromptCmd::setPrompt( const String &prompt )
        {
            m_prompt = prompt;
        }

    }  // namespace editor
}  // namespace fb
