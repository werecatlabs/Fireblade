#ifndef ResourceInfo_h__
#define ResourceInfo_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    struct ResourceInfo
    {
        /// The resource name.
        FixedString<256> name;

        /// The id of the file.
        hash64 fileId = 0;
    };
}  // end namespace fb

#endif  // ResourceInfo_h__
