#ifndef GenerateSkyboxMaterials_h__
#define GenerateSkyboxMaterials_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace editor
    {
        class GenerateSkybox : public Job
        {
        public:
            GenerateSkybox();
            ~GenerateSkybox() override;

            
            void execute() override;

	        String getFolderPath() const;

            void setFolderPath( const String& folderPath );

        protected:
            void createMaterialFromFolder( SmartPtr<IFolderExplorer> folder );
            void setupSkyboxFromFolder( SmartPtr<IFolderExplorer> folder );

            mutable RecursiveMutex m_mutex;
            String m_folderPath;
        };
    }  // namespace editor
}  // namespace fb

#endif  // GenerateSkyboxMaterials_h__
