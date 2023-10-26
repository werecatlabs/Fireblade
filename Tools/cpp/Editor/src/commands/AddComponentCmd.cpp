#include <GameEditorPCH.h>
#include "commands/AddComponentCmd.h"
#include <commands/AddActorCmd.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/ActorWindow.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        void AddComponentCmd::undo()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( auto actor = getActor() )
            {
                if( auto component = getComponent() )
                {
                    actor->removeComponentInstance( component );
                }
            }

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            if( auto actorWindow = ui->getActorWindow() )
            {
                actorWindow->buildTree();
            }
        }

        void AddComponentCmd::redo()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( auto actor = getActor() )
            {
                if( auto component = getComponent() )
                {
                    actor->addComponentInstance( component );
                }
            }

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            if( auto actorWindow = ui->getActorWindow() )
            {
                actorWindow->buildTree();
            }
        }

        void AddComponentCmd::execute()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected )
                {
                    if( selected->isDerived<scene::IActor>() )
                    {
                        auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                        if( actor )
                        {
                            setActor( actor );

                            auto factory = getFactory();

                            auto component =
                                static_cast<scene::IComponent *>( factory->createObjectFromPool() );
                            actor->addComponentInstance( component );
                            setComponent( component );

                            if( component )
                            {
                                if( applicationManager->isEditor() )
                                {
                                    component->setState( scene::IComponent::State::Edit );
                                }
                                else
                                {
                                    component->setState( scene::IComponent::State::Play );
                                }
                            }
                        }
                    }
                }
            }

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            if( auto actorWindow = ui->getActorWindow() )
            {
                actorWindow->buildTree();
            }
        }

        SmartPtr<IFactory> AddComponentCmd::getFactory() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_factory;
        }

        void AddComponentCmd::setFactory( SmartPtr<IFactory> factory )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_factory = factory;
        }

        SmartPtr<scene::IComponent> AddComponentCmd::getComponent() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_component;
        }

        void AddComponentCmd::setComponent( SmartPtr<scene::IComponent> component )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_component = component;
        }

        SmartPtr<scene::IActor> AddComponentCmd::getActor() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_actor;
        }

        void AddComponentCmd::setActor( SmartPtr<scene::IActor> actor )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_actor = actor;
        }
    }  // namespace editor
}  // namespace fb
