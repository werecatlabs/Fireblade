#ifndef OpenSceneJob_h__
#define OpenSceneJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class OpenSceneJob : public Job
        {
        public:
            OpenSceneJob();
            ~OpenSceneJob() override;

            void execute() override;

            String getFilePath() const;

            void setFilePath( const String &filePath );

        protected:
            String m_filePath;
        };

    }  // namespace editor
}  // namespace fb

#endif  // OpenSceneJob_h__
