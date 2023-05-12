#ifndef IMeshConverter_h__
#define IMeshConverter_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        /**
         * Interface for a mesh converter that writes a mesh.
         */
        class IMeshConverter : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IMeshConverter() override = default;

            /**
             * Writes a mesh for a given actor.
             * @param actor A smart pointer to the actor whose mesh should be written.
             */
            virtual void writeMesh( SmartPtr<scene::IActor> actor ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // namespace fb

#endif  // IMeshConverter_h__
