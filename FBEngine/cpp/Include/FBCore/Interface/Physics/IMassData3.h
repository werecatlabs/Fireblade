#ifndef IMassData3_h__
#define IMassData3_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        /// This holds the mass data computed for a shape.
        class IMassData3 : public ISharedObject
        {
        public:
            virtual ~IMassData3() = default;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IMassData3_h__
