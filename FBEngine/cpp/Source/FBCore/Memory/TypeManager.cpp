#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Core/StringUtil.h>

#include <FBCore/Interface/IApplication.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/ITexture.h>

#include <FBCore/Interface/IO/IFolderExplorerW.h>
#include <FBCore/Interface/IO/IFolderExplorer.h>
#include <FBCore/Interface/IO/IArchive.h>
#include <FBCore/Interface/IO/IStream.h>

#include <FBCore/Interface/Memory/IData.h>

#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/ILogManager.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateQueue.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/UI/IUIText.h>
#include <FBCore/Interface/UI/IUITreeNode.h>
#include <FBCore/Interface/UI/IUIRenderWindow.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Interface/Script/IScriptManager.h>

#include "FBCore/Interface/FSM/IFSMManager.h"
#include "FBCore/Interface/IO/IFileSystem.h"
#include "FBCore/Interface/System/IStateListener.h"
#include "FBCore/Interface/UI/IUIManager.h"
//#include <FBCore/Interface/Graphics/IGraphicsSubMesh.h>

namespace fb
{

    static TypeManager *instance_ = nullptr;

    TypeManager::TypeManager()
    {
        auto size = FB_TYPE_MANAGER_NUM_TYPES;
        reserve( size );
    }

    TypeManager::~TypeManager()
    {
        m_name.clear();
        m_hash.clear();
        m_typeIndex.clear();
        m_typeGroup.clear();
        m_baseType.clear();
        m_dataTypes.clear();
        m_numInstances.clear();
    }

    String TypeManager::getName( u32 id ) const
    {
        SpinRWMutex::ScopedLock lock( m_nameMutex, false );
        FB_ASSERT( id < m_name.size() );
        return m_name[id];
    }

    hash64 TypeManager::getHash( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_hash[id];
    }

    u32 TypeManager::getBaseType( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_baseType[id];
    }

    bool TypeManager::isExactly( u32 a, u32 b ) const
    {
        FB_ASSERT( a < getSize() );
        FB_ASSERT( b < getSize() );
        return a == b;
    }

    bool TypeManager::isDerived( u32 a, u32 b ) const
    {
        FB_ASSERT( a < getSize() );
        FB_ASSERT( b < getSize() );

        auto baseType = a;
        while( baseType )
        {
            if( baseType == b )
            {
                return true;
            }

            baseType = getBaseType( baseType );
        }

        return false;
    }

    Array<String> TypeManager::getClassHierarchy( u32 id ) const
    {
        Array<String> classHierarchy;
        classHierarchy.reserve( 4 );

        auto baseType = id;
        while( baseType )
        {
            auto name = getName( baseType );
            classHierarchy.push_back( name );
            baseType = getBaseType( baseType );
        }

        return classHierarchy;
    }

    Array<u32> TypeManager::getClassHierarchyId( u32 id ) const
    {
        Array<u32> classHierarchy;
        classHierarchy.reserve( 4 );

        auto baseType = id;
        while( baseType )
        {
            classHierarchy.push_back( baseType );
            baseType = getBaseType( baseType );
        }

        return classHierarchy;
    }

    u32 TypeManager::getTypeIndex( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_typeIndex[id];
    }

    u32 TypeManager::getNumInstances( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_numInstances[id];
    }

    u32 TypeManager::getTypeByName( const String &name )
    {
        auto it = std::find( m_name.begin(), m_name.end(), name );
        if( it != m_name.end() )
        {
            return (u32)std::distance( m_name.begin(), it );
        }

        return 0;
    }

    u32 TypeManager::getNewTypeId( const String &name, u32 baseType )
    {
        FB_ASSERT( m_typeCount < getSize() - 1 );
        auto newId = m_typeCount++;
        if( newId >= getSize() )
        {
            auto size = getSize() + FB_TYPE_MANAGER_NUM_TYPES;
            reserve( size );
        }

        FB_ASSERT( newId < getSize() );

        if( !StringUtil::isNullOrEmpty( name ) )
        {
            SpinRWMutex::ScopedLock lock( m_nameMutex, true );
            m_name[newId] = name;
        }

        FB_ASSERT( m_baseType[newId].load() == 0 );

        m_hash[newId] = StringUtil::getHash64( name );
        m_typeIndex[newId] = newId;

        FB_ASSERT( m_baseType[newId].load() == 0 );
        m_baseType[newId] = baseType;

        return newId;
    }

    u32 TypeManager::getNewTypeId()
    {
        return m_typeCount++;
    }

    u32 TypeManager::getNewTypeIdFromName( const String &name, const String &baseName )
    {
        auto baseType = getTypeByName( baseName );
        return getNewTypeId( name, baseType );
    }

    Array<u32> TypeManager::getBaseTypes( u32 type ) const
    {
        Array<u32> types;
        types.reserve( 12 );

        type = getBaseType( type );
        while( type )
        {
            types.push_back( type );
            type = getBaseType( type );
        }

        return types;
    }

    Array<String> TypeManager::getBaseTypeNames( u32 type ) const
    {
        Array<String> types;
        types.reserve( 12 );

        type = getBaseType( type );
        while( type )
        {
            auto name = getName( type );
            types.push_back( name );
            type = getBaseType( type );
        }

        return types;
    }

    Array<u32> TypeManager::getDerivedTypes( u32 type ) const
    {
        Array<u32> types;
        types.reserve( 12 );

        for( size_t i = 0; i < m_typeCount; ++i )
        {
            auto currentType = static_cast<u32>( i );
            if( isDerived( currentType, type ) )
            {
                types.push_back( currentType );
            }
        }

        return types;
    }

    u32 TypeManager::getTypeGroup( u32 id ) const
    {
        if( id < getSize() )
        {
            if( m_typeGroup[id].load() == 0 )
            {
                m_typeGroup[id] = calculateGroupIndex( id );
            }

            return m_typeGroup[id];
        }

        return 0;
    }

    u32 TypeManager::getTotalNumTypes()
    {
        return m_typeCount;
    }

    u32 TypeManager::getDataType( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_dataTypes[id];
    }

    void TypeManager::setDataType( u32 id, u32 dataType )
    {
        FB_ASSERT( id < getSize() );
        m_dataTypes[id] = dataType;
    }

    u32 TypeManager::calculateGroupIndex( u32 typeInfo ) const
    {
        auto index = TypeGroups::Application;

        if( isDerived( typeInfo, IJob::typeInfo() ) )
        {
            index = TypeGroups::Jobs;
        }
        else if( isDerived( typeInfo, render::ISceneNode::typeInfo() ) )
        {
            index = TypeGroups::RenderNodes;
        }
        else if( isDerived( typeInfo, render::IGraphicsObject::typeInfo() ) )
        {
            index = TypeGroups::RenderObjects;
        }
        else if( isDerived( typeInfo, render::ITexture::typeInfo() ) )
        {
            index = TypeGroups::RenderTextures;
        }
        else if( isDerived( typeInfo, render::IMaterialNode::typeInfo() ) )
        {
            index = TypeGroups::RenderMaterialNodes;
        }
        else if( isDerived( typeInfo, render::IMaterial::typeInfo() ) )
        {
            index = TypeGroups::RenderMaterials;
        }
        else if( isDerived( typeInfo, IFactory::typeInfo() ) )
        {
            index = TypeGroups::Factories;
        }
        else if( isDerived( typeInfo, IArchive::typeInfo() ) )
        {
            index = TypeGroups::IOArchive;
        }
        else if( isDerived( typeInfo, IStream::typeInfo() ) )
        {
            index = TypeGroups::IOStream;
        }
        else if( isDerived( typeInfo, IFolderExplorer::typeInfo() ) ||
                 isDerived( typeInfo, IFolderExplorerW::typeInfo() ) )
        {
            index = TypeGroups::IO;
        }
        else if( isDerived( typeInfo, core::IApplicationManager::typeInfo() ) ||
                 isDerived( typeInfo, render::IGraphicsSystem::typeInfo() ) ||
                 isDerived( typeInfo, IFactoryManager::typeInfo() ) ||
                 isDerived( typeInfo, IFSMManager::typeInfo() ) ||
                 isDerived( typeInfo, ILogManager::typeInfo() ) ||
                 isDerived( typeInfo, IStateManager::typeInfo() ) ||
                 isDerived( typeInfo, IScriptManager::typeInfo() ) ||
                 isDerived( typeInfo, scene::ISceneManager::typeInfo() ) ||
                 isDerived( typeInfo, scene::IScene::typeInfo() ) ||
                 isDerived( typeInfo, ITaskManager::typeInfo() ) ||
                 isDerived( typeInfo, IThreadPool::typeInfo() ) ||
                 isDerived( typeInfo, ITimer::typeInfo() ) ||
                 isDerived( typeInfo, ui::IUIManager::typeInfo() ) )
        {
            index = TypeGroups::System;
        }
        else if( isDerived( typeInfo, scene::IActor::typeInfo() ) )
        {
            index = TypeGroups::Actor;
        }
        else if( isDerived( typeInfo, scene::IComponent::typeInfo() ) )
        {
            index = TypeGroups::Component;
        }
        else if( isDerived( typeInfo, IData::typeInfo() ) )
        {
            index = TypeGroups::Data;
        }
        else if( isDerived( typeInfo, IFSM::typeInfo() ) )
        {
            index = TypeGroups::FSM;
        }
        else if( isDerived( typeInfo, IFSMListener::typeInfo() ) )
        {
            index = TypeGroups::FSMListener;
        }
        else if( isDerived( typeInfo, ui::IUIRenderWindow::typeInfo() ) )
        {
            index = TypeGroups::UIRenderWindows;
        }
        else if( isDerived( typeInfo, ui::IUIWindow::typeInfo() ) )
        {
            index = TypeGroups::UIWindows;
        }
        else if( isDerived( typeInfo, ui::IUITreeNode::typeInfo() ) )
        {
            index = TypeGroups::UITreeNodes;
        }
        else if( isDerived( typeInfo, ui::IUIText::typeInfo() ) )
        {
            index = TypeGroups::UIText;
        }
        else if( isDerived( typeInfo, ui::IUIElement::typeInfo() ) )
        {
            index = TypeGroups::UI;
        }
        else if( isDerived( typeInfo, IStateContext::typeInfo() ) )
        {
            index = TypeGroups::State;
        }
        else if( isDerived( typeInfo, IStateQueue::typeInfo() ) )
        {
            index = TypeGroups::StateQueues;
        }
        else if( isDerived( typeInfo, IStateMessage::typeInfo() ) )
        {
            index = TypeGroups::StateMessages;
        }
        else if( isDerived( typeInfo, IStateListener::typeInfo() ) )
        {
            index = TypeGroups::StateListeners;
        }
        else if( isDerived( typeInfo, scene::IDirector::typeInfo() ) )
        {
            index = TypeGroups::Directors;
        }
        else if( isDerived( typeInfo, IResource::typeInfo() ) )
        {
            index = TypeGroups::Resources;
        }

        return (u32)index;
    }

    u32 TypeManager::getSize() const
    {
        return m_size;
    }

    void TypeManager::reserve( u32 size )
    {
        if( m_size != size )
        {
            m_size = size;

            m_name.resize( size );
            m_hash.resize( size );
            m_typeIndex.resize( size );
            m_typeGroup.resize( size );
            m_baseType.resize( size );
            m_dataTypes.resize( size );
            m_numInstances.resize( size );
        }
    }

    void TypeManager::setInstance( TypeManager *typeManager )
    {
        instance_ = typeManager;
    }

    TypeManager *TypeManager::instance()
    {
        return instance_;
    }

}  // end namespace fb
