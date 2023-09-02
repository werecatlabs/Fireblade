#include <GameEditorPCH.h>
#include <ai/AddPromptEvaluator.h>
#include <FBCore/FBCore.h>
#include "commands/AddActorCmd.h"
#include "commands/RemoveSelectionCmd.h"

namespace fb
{
    namespace editor
    {

        AddPromptEvaluator::AddPromptEvaluator()
        {
        }

        AddPromptEvaluator::AddPromptEvaluator( AddActorCmd::ActorType actorType, const String &label,
                                                const Array<String> &tags ) :
            m_actorType( actorType ), PromptEvaluator( label, tags )
        {
        }

        AddPromptEvaluator::~AddPromptEvaluator()
        {
        }

        void AddPromptEvaluator::activateGoal()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();

            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( m_actorType );
            commandManager->addCommand( cmd );
        }

        f32 AddPromptEvaluator::getRating()
        {
            auto rating = 0.0f;

            for( auto entity : namedEntities )
            {
                rating += entity.find( "add" ) != String::npos ? 1.0f : 0.0f;
                rating += entity.find( "create" ) != String::npos ? 1.0f : 0.0f;
                rating += entity.find( "make" ) != String::npos ? 1.0f : 0.0f;

                // Check if the entity matches any keyword for goals
                for( auto tag : m_tags )
                {
                    if( StringUtil::numCommonSubsequence( tag, entity ) > entity.size() * 0.5f )
                    {
                        rating += 0.5f;
                    }
                }
            }

            return rating;
        }

        fb::SmartPtr<fb::ISharedObject> AddPromptEvaluator::getOwner() const
{
            return m_owner;
        }

        void AddPromptEvaluator::setOwner( SmartPtr<ISharedObject> owner )
        {
            m_owner = owner;
        }

        f32 AddPromptEvaluator::getBias() const
        {
            return m_bias;
        }

        void AddPromptEvaluator::setBias( f32 bias )
        {
            m_bias = bias;
        }

    }  // namespace editor
}  // namespace fb
