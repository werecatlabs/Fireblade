#ifndef __IPluginInterface_h__
#define __IPluginInterface_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace core
    {
        /** Interface for a plugin to use the engine.*/
        class IPluginInterface : public ISharedObject
        {
        public:
            ~IPluginInterface() override = default;

            virtual SmartPtr<IState> getInputState() const = 0;

            virtual String executeQuery( const String &tag, const String &queryStr ) = 0;
            virtual String executeQueryDML( const String &tag, const String &queryStr ) = 0;
            virtual String executeQueryAsyncDML( const String &tag, const String &queryStr ) = 0;
            virtual String executeQueryAsync( const String &tag, const String &queryStr ) = 0;
            virtual String executeQueryRefDB( const String &tag, const String &queryStr ) = 0;

            virtual void updateRender( const time_interval &t, const time_interval &dt ) = 0;

            virtual void clearPluginMessages() = 0;

            virtual bool getEnableDebugDisplay() const = 0;
            virtual void setEnableDebugDisplay( bool enableDebugDisplay ) = 0;

            virtual bool getEnableDirectInput() const = 0;
            virtual void setEnableDirectInput( bool externalInput ) = 0;

            virtual bool isLoadComplete() const = 0;
            virtual void setLoadComplete( bool loadComplete ) = 0;

            virtual StringW getInstallFolder() const = 0;
            virtual void setInstallFolder( const StringW &installFolder ) = 0;

            virtual StringW getUserDataFolder() const = 0;
            virtual void setUserDataFolder( const StringW &userDataFolder ) = 0;

            virtual StringW getTempDataFolder() const = 0;
            virtual void setTempDataFolder( const StringW &tempDataFolder ) = 0;

            virtual StringW getPhysicsDataFolder() const = 0;
            virtual void setPhysicsDataFolder( const StringW &physicsDataFolder ) = 0;

            virtual StringW getAppDataFolder() const = 0;
            virtual void setAppDataFolder( const StringW &appDataFolder ) = 0;

            virtual StringW getRootFolder() const = 0;
            virtual void setRootFolder( const StringW &rootFolder ) = 0;

            virtual StringW getStreamingAssetsFolder() const = 0;
            virtual void setStreamingAssetsFolder( const StringW &streamingAssetsFolder ) = 0;

            virtual String getSystemSettingsDataStr() const = 0;
            virtual void setSystemSettingsDataStr( const String &str ) = 0;

            virtual String getStartupQuality() const = 0;
            virtual void setStartupQuality( const String &startupQuality ) = 0;

            virtual void sendPluginEvent( const String &eventName, const String &eventData ) = 0;

            virtual void sendPluginEventRet( const String &eventName,
                                             SmartPtr<IPluginEvent> eventData ) = 0;

            virtual void sendEvent( const String &eventName, SmartPtr<IPluginEvent> eventData ) = 0;
            virtual void receiveEvent( const String &eventName, SmartPtr<IPluginEvent> eventData ) = 0;
        };
    }  // namespace core
}  // namespace fb

#endif  // IPluginInterface_h__
