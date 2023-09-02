#include <GameEditorPCH.h>
#include <ai/PromptEvaluator.h>
#include <FBCore/FBCore.h>
#include "commands/AddActorCmd.h"
#include "commands/RemoveSelectionCmd.h"

namespace fb
{
    namespace editor
    {

        PromptEvaluator::PromptEvaluator( const String &label,
                                          const Array<String> &tags ) :
            
            m_label( label ),
            m_tags( tags )
        {
        }

        PromptEvaluator::PromptEvaluator()
        {
        }

        PromptEvaluator::~PromptEvaluator()
        {
        }

        void PromptEvaluator::activateGoal()
        {
            //auto applicationManager = core::IApplicationManager::instance();
            //auto commandManager = applicationManager->getCommandManager();

            //auto cmd = fb::make_ptr<AddActorCmd>();
            //cmd->setActorType( m_actorType );
            //commandManager->addCommand( cmd );
        }

        f32 PromptEvaluator::getRating()
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

        fb::SmartPtr<fb::ISharedObject> PromptEvaluator::getOwner() const
{
            return m_owner;
        }

        void PromptEvaluator::setOwner( SmartPtr<ISharedObject> owner )
        {
            m_owner = owner;
        }

        f32 PromptEvaluator::getBias() const
        {
            return m_bias;
        }

        void PromptEvaluator::setBias( f32 bias )
        {
            m_bias = bias;
        }

    }  // namespace editor
}  // namespace fb
