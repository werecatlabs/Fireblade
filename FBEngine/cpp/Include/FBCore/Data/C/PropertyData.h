#ifndef SaracenProperty_h__
#define SaracenProperty_h__

#include <FBCore/FBCorePrerequisites.h>

#include <FBCore/Data/C/VectorData.h>
#include <FBCore/Data/C/ComponentData.h>
#include "FBCore/Data/C/CameraData.h"
#include <FBCore/Data/C/InterfaceConfigData.h>
#include "FBCore/Data/C/StateData.h"

extern "C" {

struct PropertyData
{
    char name[PLUGIN_EVENT_STR_BUFFER_SIZE];
    char value[PLUGIN_EVENT_STR_BUFFER_SIZE];
};
}

#endif  // SaracenProperty_h__
