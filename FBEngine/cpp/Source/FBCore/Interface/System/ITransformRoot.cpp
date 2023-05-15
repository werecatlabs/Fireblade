#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/ITransformRoot.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ITransformRoot, ISharedObject );

    static SmartPtr<ITransformRoot> m_instance;

    SmartPtr<ITransformRoot> ITransformRoot::instance()
    {
        return m_instance;
    }

    void ITransformRoot::setInstance( SmartPtr<ITransformRoot> instance )
    {
        m_instance = instance;
    }
}  // end namespace fb
