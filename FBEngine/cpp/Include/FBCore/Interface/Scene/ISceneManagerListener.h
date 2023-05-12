#ifndef ISceneManagerListener_h__
#define ISceneManagerListener_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace scene
    {
        //-------------------------------------------------
        /** Interface for a scene manager object.
        @author	Zane Desir
        @version 1.0
        */
        class ISceneManagerListener : public ISharedObject
        {
        public:
            ~ISceneManagerListener() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    } // namespace scene
}     // namespace fb

#endif  // ISceneManagerListener_h__
