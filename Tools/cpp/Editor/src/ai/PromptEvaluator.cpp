#include <GameEditorPCH.h>
#include <ai/PromptEvaluator.h>
#include <FBCore/FBCore.h>

#include <utility>
#include "commands/AddActorCmd.h"
#include "commands/RemoveSelectionCmd.h"

namespace fb::editor
{

    PromptEvaluator::PromptEvaluator( String label, const Array<String> &tags ) :

        m_label( std::move( label ) ),
        m_tags( tags )
    {
    }

    PromptEvaluator::PromptEvaluator() = default;

    PromptEvaluator::~PromptEvaluator() = default;

    void PromptEvaluator::activateGoal()
    {
        //auto applicationManager = core::ApplicationManager::instance();
        //auto commandManager = applicationManager->getCommandManager();

        //auto cmd = fb::make_ptr<AddActorCmd>();
        //cmd->setActorType( m_actorType );
        //commandManager->addCommand( cmd );
    }

    auto PromptEvaluator::getRating() -> f32
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

    auto PromptEvaluator::getOwner() const -> fb::SmartPtr<fb::ISharedObject>
    {
        return m_owner;
    }

    void PromptEvaluator::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    auto PromptEvaluator::getBias() const -> f32
    {
        return m_bias;
    }

    void PromptEvaluator::setBias( f32 bias )
    {
        m_bias = bias;
    }

}  // namespace fb::editor
