#ifndef DisableScreenSave_h__
#define DisableScreenSave_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class CDisableScreenSave : public ISharedObject
    {
    public:
        CDisableScreenSave();
        ~CDisableScreenSave() override;

    protected:
        int *m_pValue;
    };
}  // end namespace fb

#endif  // DisableScreenSave_h__
