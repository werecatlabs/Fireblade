#ifndef SaveSceneJob_h__
#define SaveSceneJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class SaveSceneJob : public Job
        {
        public:
            SaveSceneJob();
            ~SaveSceneJob() override;

            void execute() override;

            void saveScene( const String &filePath );

            String getFilePath() const;

            void setFilePath( const String &filePath );

        protected:
            String m_filePath;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // SaveSceneJob_h__
