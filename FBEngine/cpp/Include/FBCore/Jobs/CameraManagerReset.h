#ifndef CameraManagerReset_h__
#define CameraManagerReset_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/System/Job.h>

namespace fb
{

    /** @brief Reset the camera manager.
     * This job is used to reset the camera manager.
     */
    class CameraManagerReset : public Job
    {
    public:
        /** Constructor. */
        CameraManagerReset();

        /** Destructor. */
        ~CameraManagerReset() override;

        /** @copydoc Job::execute */
        void execute() override;

        /** Get the owner of this job. */
        SmartPtr<ISharedObject> getOwner() const;

        /** Set the owner of this job. */
        void setOwner( SmartPtr<ISharedObject> owner );

        /** Get the delay time. */
        f32 getDelayTime() const;

        /** Set the delay time. */
        void setDelayTime( f32 delayTime );

    protected:
        // The owner of this job.
        SmartPtr<ISharedObject> m_owner;

        // The delay time.
        f32 m_delayTime = 0.0f;
    };
}  // namespace fb

#endif  // CameraManagerReset_h__
