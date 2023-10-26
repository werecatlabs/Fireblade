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

            bool getSaveAs() const;

            void setSaveAs( bool saveAs );

        protected:
            String m_filePath;
            bool m_saveAs = false;
            mutable SpinRWMutex m_mutex;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // SaveSceneJob_h__
