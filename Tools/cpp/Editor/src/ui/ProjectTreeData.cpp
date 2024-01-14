#include <GameEditorPCH.h>
#include "ui/ProjectTreeData.h"
#include <FBCore/FBCore.h>

#include <utility>

namespace fb::editor
{
    FB_CLASS_REGISTER_DERIVED( fb, ProjectTreeData, ISharedObject );

    ProjectTreeData::ProjectTreeData( String ownerType, String objectType,
                                      SmartPtr<ISharedObject> ownerData,
                                      SmartPtr<ISharedObject> objectData ) :
        m_ownerType( std::move( ownerType ) ),
        m_objectType( std::move( objectType ) ),
        m_ownerData( std::move( ownerData ) ),
        m_objectData( std::move( objectData ) )
    {
    }

    void ProjectTreeData::unload( SmartPtr<ISharedObject> data )
    {
        m_ownerData = nullptr;
        m_objectData = nullptr;
    }

    auto ProjectTreeData::getOwnerData() const -> SmartPtr<ISharedObject>
    {
        return m_ownerData;
    }

    void ProjectTreeData::setOwnerData( SmartPtr<ISharedObject> ownerData )
    {
        m_ownerData = ownerData;
    }

    auto ProjectTreeData::getObjectData() const -> SmartPtr<ISharedObject>
    {
        return m_objectData;
    }

    void ProjectTreeData::setObjectData( SmartPtr<ISharedObject> objectData )
    {
        m_objectData = objectData;
    }

    auto ProjectTreeData::getOwnerType() const -> String
    {
        return m_ownerType;
    }

    void ProjectTreeData::setOwnerType( const String &ownerType )
    {
        m_ownerType = ownerType;
    }

    auto ProjectTreeData::getObjectType() const -> String
    {
        return m_objectType;
    }

    void ProjectTreeData::setObjectType( const String &objectType )
    {
        m_objectType = objectType;
    }

}  // namespace fb::editor
