#ifndef LuaObjectData_h__
#define LuaObjectData_h__

#include <FBCore/Interface/Script/IScriptData.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <luabind/luabind.hpp>

namespace fb
{

    /** The internal lua object data. */
    class LuaObjectData : public IScriptData
    {
    public:
        LuaObjectData();
        ~LuaObjectData() override;

        /** @copydoc ISharedObject::unload */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::setOwner */
        void setOwner( SmartPtr<ISharedObject> owner ) override;

        /** @copydoc ISharedObject::getOwner */
        SmartPtr<ISharedObject> getOwner() const override;

        /** @copydoc ISharedObject::getObjectData */
        void *getObjectData() const override;

        /** Gets the script class name. */
        String getClassName() const;

        /** Sets the script class name. */
        void setClassName( const String &className );

        /** @copydoc IScriptData::toString */
        String toString() const override;

        /** The luabind object */
        luabind::object &getObject();

         /** The luabind object */
        void setObject(luabind::object &object);

    protected:
        /** The owner of this data. */
        SmartPtr<ISharedObject> m_owner;

        /** The script class name. */
        FixedString<64> m_className;

        /** Object if using a single lua state. */
        luabind::object m_object;
    };

} // end namespace fb

#endif  // LuaObjectData_h__
