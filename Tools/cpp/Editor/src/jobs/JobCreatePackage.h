#ifndef JobCreatePackage_h__
#define JobCreatePackage_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class JobCreatePackage : public Job
        {
        public:
            JobCreatePackage();
            ~JobCreatePackage();

            void execute();

            StringW getDst() const;

            void setDst( const StringW &dst );

            bool getPackageTextures() const;

            void setPackageTextures( bool packageTextures );

        protected:
            StringW m_dst;
            bool m_packageTextures = true;
        };

    }  // namespace editor
}  // namespace fb

#endif  // JobCreatePackage_h__
