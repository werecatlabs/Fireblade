#include <FBCore/FBCorePCH.h>
#include <FBCore/System/CommandManagerMT.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Jobs/RunCommandJob.h>
#include <algorithm>

namespace fb
{
    CommandManagerMT::CommandManagerMT() :
        m_currentCommand( 0 ),
        m_lastUndoCommand( -1 ),
        m_lastRedoCommand( -1 ),
        m_numStoredCommands( 100 )
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "CommandManagerMT" );
        }

        m_commands.reserve( m_numStoredCommands );
        m_lastCommand = CommandType::COMMAND_MANAGER_NONE;
    }

    CommandManagerMT::~CommandManagerMT()
    {
        m_commands.clear();
    }

    void CommandManagerMT::addCommand( SmartPtr<ICommand> command )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        auto jobQueue = applicationManager->getJobQueue();

        auto primaryTask = taskManager->getTask( Thread::Task::Primary );

        s32 index = m_currentCommand;
        s32 count = ( static_cast<s32>( m_commands.size() ) - 1 ) - m_currentCommand;
        s32 used = static_cast<s32>( m_commands.size() );

        // check if we need to remove first element
        if( index == 0 && m_lastCommand == CommandType::COMMAND_MANAGER_UNDO && m_lastUndoCommand == 0 )
        {
            m_commands.clear();
            m_commands.reserve( m_numStoredCommands );
        }

        if( !( index >= used || index < 0 || count < 1 || index + count > used ) &&
            m_commands.size() > 0 )
        {
            auto startIndex = m_currentCommand + 1;
            auto endIndex = ( static_cast<s32>( m_commands.size() ) - 1 ) - m_currentCommand;

            auto startIt = m_commands.begin();
            std::advance( startIt, startIndex );

            auto endIt = m_commands.begin();
            std::advance( endIt, endIndex );

            m_commands.erase( startIt, endIt );
        }

        if( m_commands.size() < m_numStoredCommands )
        {
            m_commands.push_back( command );
        }
        else
        {
            if( m_commands.empty() )
            {
                auto startIt = m_commands.begin();
                m_commands.erase( startIt );
            }

            m_commands.push_back( command );
        }

        m_currentCommand = static_cast<s32>( m_commands.size() - 1 );

        auto numCommands = static_cast<s32>( m_commands.size() );

        auto args = Array<Parameter>();
        args.emplace_back( numCommands );
        applicationManager->triggerEvent( IEvent::Type::Application, IEvent::addCommand, args, this,
                                          nullptr, nullptr );

        m_lastCommand = CommandType::COMMAND_MANAGER_ADD;

        if( primaryTask )
        {
            auto job = fb::make_ptr<RunCommandJob>();
            job->setCommand( command );

            primaryTask->addJob( job );
        }
        else if( jobQueue )
        {
            auto job = fb::make_ptr<RunCommandJob>();
            job->setCommand( command );

            jobQueue->addJob( job );
        }
        else
        {
            command->execute();
        }
    }

    void CommandManagerMT::removeCommand( SmartPtr<ICommand> command )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        m_commands.erase( std::remove( m_commands.begin(), m_commands.end(), command ),
                          m_commands.end() );
    }

    auto CommandManagerMT::hasCommand( SmartPtr<ICommand> command ) -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto it = std::find( m_commands.begin(), m_commands.end(), command );
        if( it != m_commands.end() )
        {
            return true;
        }

        return false;
    }

    auto CommandManagerMT::isCommandQueued( SmartPtr<ICommand> command ) -> bool
    {
        auto it = std::find( m_commands.begin(), m_commands.end(), command );
        if( it != m_commands.end() )
        {
            if( it - m_commands.begin() > m_currentCommand )
            {
                return true;
            }
        }

        return false;
    }

    auto CommandManagerMT::getNextCommand() -> SmartPtr<ICommand>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        SmartPtr<ICommand> pCommand;
        s32 command = -1;

        // in case of an Add operation we don't redo
        if( m_lastCommand == CommandType::COMMAND_MANAGER_ADD )
        {
            return pCommand;
        }

        // read first the command then modify the index
        if( m_lastCommand != CommandType::COMMAND_MANAGER_UNDO || m_lastUndoCommand == 0 )
        {
            pCommand = m_commands[m_currentCommand];
            command = m_currentCommand;
        }

        ++m_currentCommand;

        auto nextCommandIdx = static_cast<s32>( m_commands.size() ) - m_currentCommand;
        auto prevCommandIdx = static_cast<s32>( m_commands.size() ) -
                              ( static_cast<s32>( m_commands.size() ) - m_currentCommand );

        auto args = Array<Parameter>();
        args.emplace_back( nextCommandIdx );
        args.emplace_back( prevCommandIdx );
        applicationManager->triggerEvent( IEvent::Type::Application, IEvent::getNextCommand, args, this,
                                          nullptr, nullptr );

        if( m_currentCommand > static_cast<s32>( m_commands.size() ) - 1 )
        {
            m_currentCommand = static_cast<s32>( m_commands.size() ) - 1;

            if( ( m_lastCommand == CommandType::COMMAND_MANAGER_REDO ||
                  m_lastCommand == CommandType::COMMAND_MANAGER_ADD ) &&
                m_lastRedoCommand == m_currentCommand )
            {
                return nullptr;
            }
        }

        // read the command after modify the index
        if( m_lastCommand == CommandType::COMMAND_MANAGER_UNDO && m_lastUndoCommand != 0 )
        {
            if( m_currentCommand < m_commands.size() )
            {
                pCommand = m_commands[m_currentCommand];
                command = m_currentCommand;
            }

            if( m_currentCommand < static_cast<s32>( m_commands.size() ) - 1 )
            {
                m_currentCommand++;
            }
        }

        m_lastCommand = CommandType::COMMAND_MANAGER_REDO;

        m_lastRedoCommand = command;

        return pCommand;
    }

    auto CommandManagerMT::getPreviousCommand() -> SmartPtr<ICommand>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        SmartPtr<ICommand> pCommand;
        s32 command = -1;

        // read first the command then modify the index
        if( m_lastCommand != CommandType::COMMAND_MANAGER_REDO ||
            m_lastRedoCommand == static_cast<s32>( m_commands.size() ) - 1 )
        {
            if( m_currentCommand < m_commands.size() )
            {
                pCommand = m_commands[m_currentCommand];
                command = m_currentCommand;
            }
        }

        auto nextCommandIdx = static_cast<s32>( m_commands.size() ) - m_currentCommand;
        auto prevCommandIdx = static_cast<s32>( m_commands.size() ) -
                              ( static_cast<s32>( m_commands.size() ) - m_currentCommand );

        auto args = Array<Parameter>();
        args.emplace_back( nextCommandIdx );
        args.emplace_back( prevCommandIdx );
        applicationManager->triggerEvent( IEvent::Type::Application, IEvent::getPreviousCommand, args,
                                          this, nullptr, nullptr );

        --m_currentCommand;

        if( m_currentCommand < 0 )
        {
            m_currentCommand = 0;
            if( m_lastCommand == CommandType::COMMAND_MANAGER_UNDO &&
                m_lastUndoCommand == m_currentCommand )
            {
                return pCommand;
            }
        }

        // read the command after modify the index
        if( m_lastCommand == CommandType::COMMAND_MANAGER_REDO &&
            m_lastRedoCommand != static_cast<s32>( m_commands.size() ) - 1 )
        {
            if( m_currentCommand < m_commands.size() )
            {
                pCommand = m_commands[m_currentCommand];
                command = m_currentCommand;
            }
        }

        m_lastCommand = CommandType::COMMAND_MANAGER_UNDO;

        m_lastUndoCommand = command;

        return pCommand;
    }

    void CommandManagerMT::clearAll()
    {
        m_commands.clear();
    }

}  // end namespace fb
