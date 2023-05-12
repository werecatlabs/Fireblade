#ifndef OSMUtil_h__
#define OSMUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{
    class OSMUtil
    {
    public:
        static bool isBuilding( SmartPtr<IData> data, const String &id );
        static bool isRoad( SmartPtr<IData> data, const String &id );
        static String getTagValue( SmartPtr<IData> data, const String &id, const String &key );
        static data::node_data getNodeFromDataById( SmartPtr<IData> data, const String &id );
        static bool getProperties( SmartPtr<IData> data, SmartPtr<Properties> properties,
                                   const String &id );
    };
}  // namespace fb

#endif  // OSMUtil_h__
