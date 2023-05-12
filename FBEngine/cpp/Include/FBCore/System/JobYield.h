#ifndef JobYield_h__
#define JobYield_h__

#include <FBCore/Memory/CObjectYield.h>

namespace fb
{
    namespace core
    {

        class JobYield : public CSharedObject<CObjectYield>
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
