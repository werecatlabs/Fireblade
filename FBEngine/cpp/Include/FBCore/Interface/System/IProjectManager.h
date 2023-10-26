#ifndef IProjectManager_h__
#define IProjectManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** Interface for a project manager. */
    class IProjectManager : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IProjectManager() override = default;

        /*
        virtual void addProject( const String &projectPath ) = 0;
        virtual void removeProject( const String &projectPath ) = 0;
        virtual void removeProject( const IProject *project ) = 0;
        virtual void removeProject( size_t index ) = 0;
        virtual void clearProjects() = 0;

        virtual IProject *getProject( const String &projectPath ) const = 0;
        virtual IProject *getProject( size_t index ) const = 0;
        virtual size_t getProjectCount() const = 0;

        virtual Array<IProject *> getProjects() const = 0;
        */

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IProjectManager_h__
