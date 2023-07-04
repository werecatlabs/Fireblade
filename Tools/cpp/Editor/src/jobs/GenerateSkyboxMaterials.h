#ifndef GenerateSkyboxMaterials_h__
#define GenerateSkyboxMaterials_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace editor
    {
        class GenerateSkyboxMaterials : public Job
        {
        public:
            GenerateSkyboxMaterials();
            ~GenerateSkyboxMaterials() override;

            void createMaterialFromFolder( SmartPtr<IFolderExplorer> folder );
            void execute() override;
        };
    }  // namespace editor
}  // namespace fb

#endif  // GenerateSkyboxMaterials_h__
