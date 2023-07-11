#ifndef SceneDropJob_h__
#define SceneDropJob_h__

namespace fb
{
    namespace editor
    {

        class SceneDropJob : public Job
        {
        public:
            SceneDropJob();
            ~SceneDropJob() override;

            void execute() override;

            String getFilePath() const;

            void setFilePath( const String& filePath );

        protected:
            String m_filePath;
        };

    }  // end namespace editor
}  // end namespace fb

#endif // SceneDropJob_h__
