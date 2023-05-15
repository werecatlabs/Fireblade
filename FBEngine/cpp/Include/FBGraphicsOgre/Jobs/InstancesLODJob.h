#ifndef InstancesLODJob_h__
#define InstancesLODJob_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include "FBCore/Interface/System/IJob.h"
#include "FBGraphicsOgre/InstanceObject.h"

namespace fb
{
    namespace render
    {

        class InstancesLODJob : public IJob
        {
        public:
            InstancesLODJob( Array<CInstanceObjectOld *> &instances, Ogre::Viewport *viewport );
            ~InstancesLODJob();

            void update();

            void processInstance( CInstanceObjectOld *instanceObject );

            u32 getProgress() const;
            void setProgress( u32 val );

            s32 getPriority() const;
            void setPriority( s32 val );

        protected:
            u32 numInstancesLess150;
            u32 numInstancesLess300;
            u32 numInstancesLess500;
            u32 numInstancesGreater500;

            atomic_u32 m_progress;
            atomic_u32 m_priority;

            u32 m_index;

            Ogre::Viewport *m_viewport;
            Array<CInstanceObjectOld *> m_instances;
        };

    }  // namespace render
}  // end namespace fb

#endif  // InstancesLODJob_h__
