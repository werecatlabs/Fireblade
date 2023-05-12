#include <FBCore/FBCorePCH.h>
#include <FBCore/System/CommandManager.h>
#include <FBCore/FBCore.h>
#include <algorithm>

namespace fb
{

    CommandManager::CommandManager() :
        m_currentCommand( 0 ),
        m_lastUndoCommand( -1 ),
        m_lastRedoCommand( -1 ),
        m_numStoredCommands( 100 )
    {
        m_commands.reserve( m_numStoredCommands );
        m_lastCommand = CommandType::COMMAND_MANAGER_NONE;
    }

    CommandManager::~CommandManager()
    {
        m_commands.clear();
        m_listeners.clear();
    }

    void CommandManager::addCommand( SmartPtr<ICommand> command )
    {
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
        for( auto &listener : m_listeners )
        {
            listener->OnAddCommand( numCommands );
        }

        m_lastCommand = CommandType::COMMAND_MANAGER_ADD;

        command->execute();
    }

    void CommandManager::removeCommand( SmartPtr<ICommand> command )
    {
        m_commands.erase( std::remove( m_commands.begin(), m_commands.end(), command ),
                          m_commands.end() );
    }

    bool CommandManager::hasCommand( SmartPtr<ICommand> command )
    {
        auto it = std::find( m_commands.begin(), m_commands.end(), command );
        if( it != m_commands.end() )
        {
            return true;
        }

        return false;
    }

    bool CommandManager::isCommandQueued( SmartPtr<ICommand> command )
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

    SmartPtr<ICommand> CommandManager::getNextCommand()
    {
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

        for( auto listener : m_listeners )
        {
            auto nextCommandIdx = static_cast<s32>( m_commands.size() ) - m_currentCommand;
            auto prevCommandIdx = static_cast<s32>( m_commands.size() ) -
                                  ( static_cast<s32>( m_commands.size() ) - m_currentCommand );

            listener->OnGetNextCommand( nextCommandIdx, prevCommandIdx );
        }

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

    SmartPtr<ICommand> CommandManager::getPreviousCommand()
    {
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

        for( auto listener : m_listeners )
        {
            auto nextCommandIdx = static_cast<s32>( m_commands.size() ) - m_currentCommand;
            auto prevCommandIdx = static_cast<s32>( m_commands.size() ) -
                                  ( static_cast<s32>( m_commands.size() ) - m_currentCommand );

            listener->OnGetPreviousCommand( nextCommandIdx, prevCommandIdx );
        }

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

    void CommandManager::clearAll()
    {
        m_commands.clear();
    }

    void CommandManager::addListener( SmartPtr<ICommandManagerListener> listener )
    {
        m_listeners.push_back( listener );
    }

    bool CommandManager::removeListener( SmartPtr<ICommandManagerListener> listener )
    {
        auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
        if( it != m_listeners.end() )
        {
            m_listeners.erase( it );
            return true;
        }

        return false;
    }
}  // end namespace fb
