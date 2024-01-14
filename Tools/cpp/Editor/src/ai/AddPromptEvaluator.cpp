#include <GameEditorPCH.h>
#include <ai/AddPromptEvaluator.h>
#include <FBCore/FBCore.h>
#include "commands/AddActorCmd.h"
#include "commands/RemoveSelectionCmd.h"

namespace fb::editor
{

    AddPromptEvaluator::AddPromptEvaluator() = default;

    AddPromptEvaluator::AddPromptEvaluator( AddActorCmd::ActorType actorType, const String &label,
                                            const Array<String> &tags ) :
        m_actorType( actorType ),
        PromptEvaluator( label, tags )
    {
    }

    AddPromptEvaluator::~AddPromptEvaluator() = default;

    void AddPromptEvaluator::activateGoal()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto commandManager = applicationManager->getCommandManager();

        auto cmd = fb::make_ptr<AddActorCmd>();
        cmd->setActorType( m_actorType );
        commandManager->addCommand( cmd );
    }

    auto AddPromptEvaluator::getRating() -> f32
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

    auto AddPromptEvaluator::getOwner() const -> fb::SmartPtr<fb::ISharedObject>
    {
        return m_owner;
    }

    void AddPromptEvaluator::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    auto AddPromptEvaluator::getBias() const -> f32
    {
        return m_bias;
    }

    void AddPromptEvaluator::setBias( f32 bias )
    {
        m_bias = bias;
    }

}  // namespace fb::editor
