#ifndef JobYield_h__
#define JobYield_h__

#include <FBCore/Memory/ObjectYield.h>

namespace fb
{
    namespace core
    {

        class JobYield : public SharedObject<ObjectYield>
        {
        public:
            JobYield() = default;
            JobYield( SmartPtr<IJob> job );
            JobYield( SmartPtr<IObjectYield> &jobYield );

            ~JobYield() override = default;

            void stop() override;

            SmartPtr<IJob> m_job;
        };

    }  // namespace core
}  // namespace fb

#endif  // JobYield_h__
