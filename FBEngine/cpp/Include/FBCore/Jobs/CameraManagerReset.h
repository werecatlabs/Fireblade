#ifndef CameraManagerReset_h__
#define CameraManagerReset_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/System/Job.h>

namespace fb
{

    class CameraManagerReset : public Job
    {
    public:
        CameraManagerReset();
        ~CameraManagerReset() override;

        void execute() override;

        SmartPtr<ISharedObject> getOwner() const;
        void setOwner( SmartPtr<ISharedObject> owner );

    protected:
        SmartPtr<ISharedObject> m_owner;
    };

}  // namespace fb

#endif  // CameraManagerReset_h__
