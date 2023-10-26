#ifndef CompileProjectJob_h__
#define CompileProjectJob_h__

#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class CompileProjectJob : public Job
        {
        public:
            CompileProjectJob();
            ~CompileProjectJob() override;

            void execute() override;
        };
    } // namespace editor
}     // namespace fb

#endif // CompileProjectJob_h__
