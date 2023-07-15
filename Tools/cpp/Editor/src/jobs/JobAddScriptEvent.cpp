#include <GameEditorPCH.h>
#include "jobs/JobAddScriptEvent.h"
#include "editor/EditorManager.h"
#include "editor/Project.h"
#include "jobs/JobGenerateScript.h"
#include "jobs/JobOpenScript.h"
#include "jobs/JobRestoreTree.h"
#include "jobs/JobSaveTree.h"
#include "commands/AddNewScriptCmd.h"
#include "ui/ProjectWindow.h"
#include "ui/UIManager.h"
#include <FBApplication/FBApplication.h>
#include <FBCore/FBCore.h>
#include <fstream>

using std::endl;
using std::ofstream;

namespace fb
{
    namespace editor
    {

        //--------------------------------------------
        JobAddScriptEvent::JobAddScriptEvent()
        {
        }

        //--------------------------------------------
        JobAddScriptEvent::~JobAddScriptEvent()
        {
        }

        //--------------------------------------------
        void JobAddScriptEvent::execute()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // SmartPtr<IFileSystem>& fileSystem = applicationManager->getFileSystem();

            // EditorManager* appRoot = EditorManager::getSingletonPtr();
            // SmartPtr<Project> project = appRoot->getProject();
            // SmartPtr<UIManager> guiMgr = appRoot->getUI();

            // ApplicationFrame* appFrame = guiMgr->getApplicationFrame();
            // if(!appFrame)
            //{
            //	FB_EXCEPTION("Error: App frame null! ");
            //	return;
            // }

            // ProjectWindow* projectWindow = guiMgr->getProjectWindow();
            // if(!projectWindow)
            //{
            //	FB_EXCEPTION("Error: projectWindow null! ");
            //	return;
            // }

            // SmartPtr<EventTemplate> event;// = projectWindow->getSelectedObject();

            // SmartPtr<JobSaveTree> jobSaveTree(new JobSaveTree);
            ////jobSaveTree->setPrimary(true);
            ////applicationManager->getJobQueue()->queueJob(jobSaveTree);
            //
            // SmartPtr<JobGenerateScript> jobGenerateScript(new JobGenerateScript);
            // jobGenerateScript->setPrimary(true);
            // jobGenerateScript->setEvent(event);
            // applicationManager->getJobQueue()->queueJob(jobGenerateScript);
            // m_generateScriptJob = jobGenerateScript;

            //// update project tree
            // if(!jobGenerateScript->isExistingScript())
            //{
            //	SmartPtr<JobRestoreTree> jobRestoreTree(new JobRestoreTree);
            //	//jobRestoreTree->setPrimary(true);
            //	//applicationManager->getJobQueue()->queueJob(jobRestoreTree);
            // }

            // setState(IJob::STATE_FINISHED);
        }

        //--------------------------------------------
        void JobAddScriptEvent::createOpenScriptJob()
        {
            auto applicationManager = core::IApplicationManager::instance();

            SmartPtr<JobGenerateScript> jobGenerateScript;  // = m_generateScriptJob;

            /// open the script in the editor
            SmartPtr<JobOpenScript> jobOpenScript( new JobOpenScript );
            jobOpenScript->setPrimary( true );
            //jobOpenScript->setScriptTemplate( jobGenerateScript->getScriptTemplate() );
            applicationManager->getJobQueue()->addJob( jobOpenScript );
        }

        //--------------------------------------------
        void JobAddScriptEvent::createRestoreTreeJob()
        {
            // auto applicationManager = core::IApplicationManager::instance();

            // SmartPtr<JobGenerateScript> jobGenerateScript = m_generateScriptJob;

            //// update project tree
            // if(!jobGenerateScript->isExistingScript())
            //{
            //	SmartPtr<JobRestoreTree> jobRestoreTree(new JobRestoreTree);
            //	//jobRestoreTree->setPrimary(true);
            //	//engine->getJobQueue()->queue(jobRestoreTree);
            // }
        }

        //--------------------------------------------
        SmartPtr<IJob> JobAddScriptEvent::getGenerateScriptJob() const
        {
            return m_generateScriptJob;
        }

        //--------------------------------------------
        void JobAddScriptEvent::setGenerateScriptJob( SmartPtr<IJob> val )
        {
            m_generateScriptJob = val;
        }

        //--------------------------------------------
        JobAddScriptEvent::JobListener::JobListener( JobAddScriptEvent *job ) : m_job( job )
        {
        }

        //--------------------------------------------
        JobAddScriptEvent::JobListener::~JobListener()
        {
        }

        //--------------------------------------------
        void JobAddScriptEvent::JobListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
        {
            /*if(message->isExactly(StateMessageJobStatus::TYPE_INFO))
            {
                StateMessageJobStatusPtr jobMessage = message;

                switch(jobMessage->getJobStatus())
                {
                case StateMessageJobStatus::JOB_STATUS_END:
                    {
                        if(jobMessage->getJob() == m_job->getGenerateScriptJob())
                        {

                        }
                    }
                    break;
                default:
                    {
                    }
                };
            }*/
        }

        //--------------------------------------------
        void JobAddScriptEvent::JobListener::handleStateChanged( const SmartPtr<IState> &state )
        {
        }

        //--------------------------------------------
        void JobAddScriptEvent::JobListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

    }  // end namespace editor
}  // end namespace fb
