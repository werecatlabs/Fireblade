#include <FBCore/FBCorePCH.h>
#include "FBCore/Base/DataUtil.h"
#include "FBCore/Base/Properties.h"
#include "FBCore/Base/StringUtil.h"
#include "FBCore/Memory/PointerUtil.h"
#include <cJSON.h>

#include <FBCore/Base/DataCoreTypes.h>
#include <FBCore/Base/Data.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/DataUtil.h>
//#include <FBCore/Data/DataDeclarations.h>
#include "FBCore/Base/LogManager.h"
//#include "FBCore/Data/DataDeclarations.h"
//#include "FBCore/Data/AeroDataDeclarations.h"

//#include "FBCore/Base/JsonDataDeclarations.h"
//#include "FBCore/Base/JsonData.h"
//#include "FBCore/Base/DataUtil.h"
//#include "FBCore/Base/JsonBindings.h"
//#include "FBCore/Base/JsonDataMacros.h"
//#include "FBCore/Data/DefineDataStructures.h"
#include <FBCore/Base/DataCoreTypes.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Memory/RawPtr.h>

#include <tinyxml.h>
#include <cJSON.h>

#include <vector>
#include <string>

#include "FBCore/Base/DataCoreTypes.h"

#include <cJSON.h>
#include <tinyxml.h>

#include <FBCore/Base/DataBinding.h>

using namespace fb;
#include <FBCore/Reflection/ReflectionXML.h>
#include <FBCore/Base/DataStructures.h>
#include <FBCore/Reflection/ReflectionJSON.h>
#include <FBCore/Base/DataStructures.h>
#include <FBCore/Reflection/ReflectionProperties.h>
#include <FBCore/Base/DataStructures.h>
#include <FBCore/Reflection/ReflectionClean.h>

namespace fb
{
    // SmartPtr<Properties> DataUtil::jsonToProperties(const std::string& jsonData)
    //{
    //	FB_ASSERT(!StringUtil::isNullOrEmpty(jsonData));

    //	auto properties = fb::make_ptr<Properties>();

    //	auto json = cJSON_Parse(jsonData.c_str());
    //	auto data = json->child;

    //	while (data)
    //	{
    //		auto name = String(data->string ? data->string : "");
    //		auto value = String("");

    //		if (data->type == cJSON_String)
    //		{
    //			value = data->valuestring ? data->valuestring : "";
    //		}

    //		properties->setProperty(name, value);
    //		data = data->next;
    //	}

    //	return properties;
    //}
}  // end namespace fb
