#include "FBLua/LuaObjectData.h"

namespace fb
{
    LuaObjectData::LuaObjectData()
    {
    }

    LuaObjectData::~LuaObjectData()
    {
        unload( nullptr );
    }

    void LuaObjectData::load( SmartPtr<ISharedObject> data )
    {
    }

    void LuaObjectData::unload( SmartPtr<ISharedObject> data )
    {
        m_object = luabind::object();
        m_owner = nullptr;
    }

    void LuaObjectData::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    SmartPtr<ISharedObject> LuaObjectData::getOwner() const
    {
        return m_owner;
    }

    void *LuaObjectData::getObjectData() const
    {
        return (void *)&m_object;
    }

    String LuaObjectData::getClassName() const
    {
        return String( m_className.c_str() );
    }

    void LuaObjectData::setClassName( const String &className )
    {
        m_className = className;
    }

    String LuaObjectData::toString() const
    {
        std::stringstream stream;
        stream << "m_className: " << m_className.c_str() << std::endl;
        return stream.str();
    }

    luabind::object &LuaObjectData::getObject()
    {
        return m_object;
    }

    void LuaObjectData::setObject( luabind::object &object )
    {
        m_object = object;
    }

}  // end namespace fb
