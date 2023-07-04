#ifndef JobOpenScript_h__
#define JobOpenScript_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class JobOpenScript : public Job
        {
        public:
            JobOpenScript();
            ~JobOpenScript();

            void execute();

        protected:
            //SmartPtr<ScriptTemplate> m_scriptTemplate;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // JobOpenScript_h__
