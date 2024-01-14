#ifndef LoadPluginJob_h__
#define LoadPluginJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class LoadPluginJob : public Job
        {
        public:
            LoadPluginJob();
            ~LoadPluginJob() override;

            void execute() override;

            String getPluginPath() const;

            void setPluginPath( const String &pluginPath );

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_pluginPath;
        };
    } // namespace editor
}     // namespace fb

#endif  // LoadPluginJob_h__
