#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/ObjectUpdateJob.h>
#include <FBCore/FBCore.h>

namespace fb
{

    ObjectUpdateJob::ObjectUpdateJob()
    {
    }

    ObjectUpdateJob::~ObjectUpdateJob()
    {
    }

    void ObjectUpdateJob::execute()
    {
        m_owner->update();
    }

    SmartPtr<ISharedObject> ObjectUpdateJob::getOwner() const
    {
        return m_owner;
    }

    void ObjectUpdateJob::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

}  // namespace fb
