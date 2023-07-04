#include <GameEditorPCH.h>
#include "ui/ProjectTreeData.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {
        FB_CLASS_REGISTER_DERIVED( fb, ProjectTreeData, SharedObject<ISharedObject> );

        ProjectTreeData::ProjectTreeData( const String &ownerType, const String &objectType,
                                          SmartPtr<ISharedObject> ownerData,
                                          SmartPtr<ISharedObject> objectData ) :
            m_ownerType( ownerType ),
            m_objectType( objectType ),
            m_ownerData( ownerData ),
            m_objectData( objectData )
        {
        }

        void ProjectTreeData::unload( SmartPtr<ISharedObject> data )
        {
            m_ownerData = nullptr;
            m_objectData = nullptr;
        }

        SmartPtr<ISharedObject> ProjectTreeData::getOwnerData() const
        {
            return m_ownerData;
        }

        void ProjectTreeData::setOwnerData( SmartPtr<ISharedObject> ownerData )
        {
            m_ownerData = ownerData;
        }

        SmartPtr<ISharedObject> ProjectTreeData::getObjectData() const
        {
            return m_objectData;
        }

        void ProjectTreeData::setObjectData( SmartPtr<ISharedObject> objectData )
        {
            m_objectData = objectData;
        }

        String ProjectTreeData::getOwnerType() const
        {
            return m_ownerType;
        }

        void ProjectTreeData::setOwnerType( const String &ownerType )
        {
            m_ownerType = ownerType;
        }

        String ProjectTreeData::getObjectType() const
        {
            return m_objectType;
        }

        void ProjectTreeData::setObjectType( const String &objectType )
        {
            m_objectType = objectType;
        }

    }  // end namespace editor
}  // end namespace fb
