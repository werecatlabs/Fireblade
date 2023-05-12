#ifndef RegistryManager_h__
#define RegistryManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    class RegistrySettings : public CSharedObject<ISharedObject>
    {
    public:
        RegistrySettings();

        ~RegistrySettings() override;

        SmartPtr<Properties> getProperties() const;

        void setProperties( SmartPtr<Properties> properties );

    protected:
#ifdef FB_PLATFORM_WIN32
        static std::string getStringKeyValue( const std::string &strValueName );
        static StringW getStringKeyValue( const StringW &strValueName );

        static bool setStringKeyValue( HKEY hRootKey, const StringW &keyname, const StringW &name,
                                       const StringW &value );
        static void getStringKeyValue( HKEY hRootKey, const StringW &keyname, const StringW &name,
                                       StringW &value );

        static bool hasKeyValue( HKEY hRootKey, const StringW &keyname, const StringW &name );

        static LONG getDWORDRegKey( HKEY hKey, const StringW &strValueName, DWORD &nValue,
                                    DWORD nDefaultValue );
        static LONG getBoolRegKey( HKEY hKey, const StringW &strValueName, bool &bValue,
                                   bool bDefaultValue );
        static LONG getStringRegKey( HKEY hKey, const StringW &strValueName, StringW &strValue,
                                     const StringW &strDefaultValue );
#endif
    };
}  // namespace fb

#endif  // RegistryManager_h__
