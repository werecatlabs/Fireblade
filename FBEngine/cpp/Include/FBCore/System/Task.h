#ifndef Task_h__
#define Task_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITaskLock.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Thread/SpinRWMutex.h>

namespace fb
{
    /** Default task implementation.  */
    class Task : public SharedObject<ITask>
    {
    public:
        class Flags
        {
        public:
            Flags( SmartPtr<Task> task );

            ~Flags();

            SmartPtr<Task> getTask() const;

            void setTask( SmartPtr<Task> task );

        private:
            AtomicSmartPtr<Task> m_task;
        };

        /** Constructor */
        Task() = default;

        /** Copy constructor */
        Task( const Task &other ) = delete;

        /** Destructor */
        ~Task() override = default;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::reload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ITask::update */
        void update() override;

        /** @copydoc ITask::reset */
        void reset() override;

        /** @copydoc ITask::queueJob */
        void queueJob( SmartPtr<IJob> job ) override;

        /** @copydoc ITask::setPrimary */
        void setPrimary( bool usePrimary ) override;

        /** @copydoc ITask::isPrimary */
        bool isPrimary() const override;

        /** @copydoc ITask::setRecycle */
        void setRecycle( bool recycle ) override;

        /** @copydoc ITask::getRecycle */
        bool getRecycle() const override;

        /** @copydoc ITask::setAffinity */
        void setAffinity( s32 id ) override;

        /** @copydoc ITask::getAffinity */
        s32 getAffinity() const override;

        /** @copydoc ITask::isParallel */
        bool isParallel() const override;

        /** @copydoc ITask::setParallel */
        void setParallel( bool parallel ) override;

        /** @copydoc ITask::isEnabled */
        bool isEnabled() const override;

        /** @copydoc ITask::setEnabled */
        void setEnabled( bool val ) override;

        /** @copydoc ITask::removeListener */
        Thread::Task getTask() const override;

        /** @copydoc ITask::removeListener */
        void setTask( Thread::Task task ) override;

        /** @copydoc ITask::removeListener */
        void stop() override;

        /** @copydoc ITask::removeListener */
        void start() override;

        /** @copydoc ITask::isStopped */
        bool isStopped() const override;

        bool isUpdating() const override;

        void setUpdating( bool updating ) override;

        /** @copydoc ITask::isExecuting */
        bool isExecuting() const override;

        /** @copydoc ITask::getTargetFPS */
        f64 getTargetFPS() const override;

        /** @copydoc ITask::setTargetFPS */
        void setTargetFPS( f64 targetFPS ) override;

        /** @copydoc ITask::getUseFixedTime */
        bool getUseFixedTime() const override;

        /** @copydoc ITask::setUseFixedTime */
        void setUseFixedTime( bool useFixedTime ) override;

        /** @copydoc ITask::getNextUpdateTime */
        time_interval getNextUpdateTime() const override;

        /** @copydoc ITask::setNextUpdateTime */
        void setNextUpdateTime( time_interval nextUpdateTime ) override;

        /** @copydoc ITask::setState */
        void setState( State state ) override;

        /** @copydoc ITask::getState */
        State getState() const override;

        /** @copydoc ITask::getOwner */
        SmartPtr<ISharedObject> getOwner() const override;

        /** @copydoc ITask::setOwner */
        void setOwner( SmartPtr<ISharedObject> owner ) override;

        /** Calculates the automatic framerate. */
        void calculateAutoFPS();

        /** Gets if the frame rate should be automatically calculated. */
        f64 getAutoFPS() const;

        /** Sets if the frame rate should be automatically calculated. */
        void setAutoFPS( f64 val );

        u32 getTicks() const override;

        SmartPtr<IProfile> getProfile() const;

        void setProfile( SmartPtr<IProfile> profile );

        SmartPtr<IFSM> getFSM() const;

        FB_CLASS_REGISTER_DECL;

    protected:
        AtomicSmartPtr<IFSM> m_fsm;

        AtomicSmartPtr<IProfile> m_profile;

        Thread::Task m_taskId = Thread::Task::Primary;
        ConcurrentQueue<SmartPtr<IJob>> m_jobs;
        atomic_f64 m_autoFPS = 0.0;
        atomic_u32 m_tickCount = 0;
        atomic_s32 m_stopped = 0;
        atomic_s32 m_affinity = 0;
        atomic_u32* m_taskFlags = nullptr;

        void *m_managerData = nullptr;

        mutable SpinRWMutex m_mutex;
    };
} // end namespace fb

#endif  // TaskMinimal_h__
