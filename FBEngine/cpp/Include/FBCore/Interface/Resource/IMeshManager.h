#ifndef __IMeshManager_h__
#define __IMeshManager_h__

#include <FBCore/Interface/Resource/IResourceManager.h>

namespace fb
{

    /**
     * @brief Interface for managing meshes and loading them from files.
     */
    class IMeshManager : public IResourceManager
    {
    public:
        /** Destructor */
        ~IMeshManager() override = default;

        /**
         * @brief Loads a mesh from the specified file.
         * @param fileName The name of the file to load.
         * @return A smart pointer to the loaded mesh.
         */
        virtual SmartPtr<IMesh> loadMesh( const String &fileName ) = 0;

        /**
         * @brief Loads meshes from the specified file and creates a scene node for them.
         * @param fileName The name of the file to load.
         * @return A smart pointer to the created scene node.
         */
        virtual SmartPtr<render::ISceneNode> loadMeshes( const String &fileName ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IMeshManager_h__
