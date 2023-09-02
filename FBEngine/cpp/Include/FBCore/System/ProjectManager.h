#ifndef __FB_ProjectManager_h__
#define __FB_ProjectManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class ProjectManager : public ISharedObject
    {
    public:
        ProjectManager() = default;
        ~ProjectManager() override = default;

        void generateCMakeProject();

        bool isSharedLibrary() const;
        void setSharedLibrary( bool val );

    protected:
        bool m_isSharedLibrary = true;
    };
}  // end namespace fb

#endif  // ProjectManager_h__
