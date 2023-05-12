#ifndef StateMessageAttributes_h__
#define StateMessageAttributes_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    class StateMessageAttributes : public StateMessage
    {
    public:
    protected:
        int m_iValue;
        f32 m_fValue;
        Vector3F m_vector3Value;
        Vector4F m_vector4Value;
        String m_sValue;
    };
}  // namespace fb

#endif  // StateMessageAttributes_h__
