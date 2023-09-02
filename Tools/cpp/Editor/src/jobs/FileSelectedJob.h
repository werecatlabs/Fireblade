#ifndef FileSelectedJob_h__
#define FileSelectedJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class FileSelectedJob : public Job
        {
        public:
            FileSelectedJob();
            ~FileSelectedJob() override;

            void execute() override;

            String getFilePath() const;

            void setFilePath( const String &filePath );

        protected:
            mutable RecursiveMutex m_mutex;
            String m_filePath;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // FileSelectedJob_h__
