#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/StringUtil.h>
#include <sstream>

namespace fb
{
    u32 Handle::m_nameExt = 1;

    Handle::Handle() : m_hash( 0 )
    {
        constexpr auto size = sizeof( Handle );

        m_id = m_nameExt++;
        m_hash = m_id;
    }

    Handle::Handle( const String &name )
    {
        setName( name );
        m_hash = StringUtil::getHash( name );
    }

    Handle::Handle( const Handle &other )
    {
        *this = other;
    }

    Handle::~Handle()
    {
    }

    hash64 Handle::getHash() const
    {
        return m_hash;
    }

    void Handle::setHash( hash64 hash )
    {
        m_hash = hash;
    }

    String Handle::getName() const
    {
        return m_name;
    }

    void Handle::setName( const String &name )
    {
        m_name = name;
        m_hash = StringUtil::getHash( name );
    }

    String Handle::getUUID() const
    {
        return m_uuid;
    }

    void Handle::setUUID( const String &uuid )
    {
        FB_ASSERT( !StringUtil::isNullOrEmpty( uuid ) );
        m_uuid = uuid;
    }

    bool Handle::operator==( const Handle &b )
    {
        return m_hash == b.m_hash;
    }

    u32 Handle::getId() const
    {
        return m_id;
    }

    void Handle::setId( u32 id )
    {
        m_id = id;
    }

    void Handle::setInstanceId( u32 instanceId )
    {
        m_instanceId = instanceId;
    }

    Handle &Handle::operator=( const Handle &other )
    {
        m_id = other.m_id;
        m_instanceId = other.m_instanceId;
        m_hash = other.m_hash;
        m_name = other.m_name;
        m_uuid = other.m_uuid;
        return *this;
    }

    String Handle::toString() const
    {
        std::stringstream stream;
        stream << "m_hash: " << m_hash << "\n";
        stream << "m_name: " << m_name.c_str() << "\n";
        return stream.str().c_str();
    }

    String Handle::getClassId() const
    {
        return m_classId;
    }

    void Handle::setClassId( const String &classId )
    {
        m_classId = classId;
    }

    String Handle::getFileId() const
    {
        return m_fileId;
    }

    void Handle::setFileId( const String &fileId )
    {
        m_fileId = fileId;
    }

}  // end namespace fb
