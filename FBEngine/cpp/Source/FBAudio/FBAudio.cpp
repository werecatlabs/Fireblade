#include "FBAudio/FBAudio.h"
#include <FBCore/FBCore.h>
#include "FBAudio/Sound2.h"
#include "FBAudio/Sound3.h"
#include "FBAudio/FBAudioManager.h"

namespace fb
{

    void FBAudio::load( SmartPtr<ISharedObject> data )
    {
        FactoryUtil::addFactory<FBAudioManager>();
        FactoryUtil::addFactory<Sound2>();
        FactoryUtil::addFactory<Sound3>();
    }

    void FBAudio::unload( SmartPtr<ISharedObject> data )
    {
        FB_UNUSED( data );
    }

}  // namespace fb
