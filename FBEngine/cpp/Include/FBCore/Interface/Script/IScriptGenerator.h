#ifndef IScriptGenerator_h__
#define IScriptGenerator_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IScriptGenerator : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IScriptGenerator() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IScriptGenerator_h__
