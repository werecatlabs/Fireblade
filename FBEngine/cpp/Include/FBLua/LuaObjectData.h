#ifndef LuaObjectData_h__
#define LuaObjectData_h__

#include <FBCore/Interface/Script/IScriptData.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <luabind/luabind.hpp>

namespace fb
{

    class LuaObjectData : public CSharedObject<IScriptData>
    {
    public:
        LuaObjectData();
        ~LuaObjectData() override;

        /** @copydoc ISharedObject::unload */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void setOwner( SmartPtr<ISharedObject> owner ) override;
        SmartPtr<ISharedObject> getOwner() const override;

        void *getObjectData() const override;

        String getClassName() const;

        void setClassName( const String &className );

        String toString() const override;

        luabind::object &getObject();
        void setObject(luabind::object &object);

    protected:
        SmartPtr<ISharedObject> m_owner;
        String m_className;

        /// Object if using a single lua state.
        luabind::object m_object;
    };

} // end namespace fb

#endif  // LuaObjectData_h__
