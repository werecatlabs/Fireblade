#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/ObjectUpdateJob.h>

namespace fb
{

    ObjectUpdateJob::ObjectUpdateJob() = default;

    ObjectUpdateJob::~ObjectUpdateJob() = default;

    void ObjectUpdateJob::execute()
    {
        m_owner->update();
    }

    auto ObjectUpdateJob::getOwner() const -> SmartPtr<ISharedObject>
    {
        return m_owner;
    }

    void ObjectUpdateJob::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

}  // namespace fb
