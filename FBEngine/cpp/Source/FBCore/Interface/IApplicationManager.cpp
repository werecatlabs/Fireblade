#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Base/DebugTrace.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace core
    {
        FB_CLASS_REGISTER_DERIVED( fb, IApplicationManager, ISharedObject );

        AtomicSmartPtr<IApplicationManager> IApplicationManager::m_instance;

        SmartPtr<IApplicationManager> IApplicationManager::instance()
        {
            return m_instance;
        }

        void IApplicationManager::setInstance( SmartPtr<IApplicationManager> instance )
        {
            m_instance = instance;
        }
    }  // namespace core
}  // namespace fb