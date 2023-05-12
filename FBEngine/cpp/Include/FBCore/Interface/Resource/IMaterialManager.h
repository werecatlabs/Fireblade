//
// Created by Zane Desir on 11/11/2021.
//

#ifndef FB_IMATERIALMANAGER_H
#define FB_IMATERIALMANAGER_H

#include <FBCore/Interface/Resource/IResourceManager.h>

namespace fb
{

    /**
     * An interface for a material manager.
     */
    class IMaterialManager : public IResourceManager
    {
    public:
        /**
         * Destructor.
         */
        ~IMaterialManager() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // FB_IMATERIALMANAGER_H
