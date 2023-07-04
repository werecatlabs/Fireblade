#ifndef JobGenerateScript_h__
#define JobGenerateScript_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class JobGenerateScript : public Job
        {
        public:
            JobGenerateScript();
            ~JobGenerateScript();

            void execute();

            bool isExistingScript() const;
            void setExistingScript( bool val );

        protected:
            //SmartPtr<ScriptTemplate> m_scriptTemplate;
            //SmartPtr<EventTemplate> m_event;

            bool m_isExistingScript = false;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // JobGenerateScript_h__
