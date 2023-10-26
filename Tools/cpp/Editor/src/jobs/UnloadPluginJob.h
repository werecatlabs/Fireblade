#ifndef UnloadPluginJob_h__
#define UnloadPluginJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class UnloadPluginJob : public Job
        {
        public:
            UnloadPluginJob();
            ~UnloadPluginJob();

            void execute();

            fb::SmartPtr<fb::IPlugin> getPlugin() const;

            void setPlugin( fb::SmartPtr<fb::IPlugin> plugin );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<IPlugin> m_plugin;
        };

    }  // namespace editor
}  // namespace fb

#endif  // UnloadPluginJob_h__
