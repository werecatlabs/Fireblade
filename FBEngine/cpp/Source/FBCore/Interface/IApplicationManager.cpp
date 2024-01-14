#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/DebugTrace.h>

namespace fb::core
{

    FB_CLASS_REGISTER_DERIVED( fb, IApplicationManager, ISharedObject );

    SmartPtr<IApplicationManager> IApplicationManager::m_instance;

    void IApplicationManager::setInstance( SmartPtr<IApplicationManager> instance )
    {
        m_instance = instance;
    }

}  // namespace fb::core
