#ifndef StateMessageParamVector4_h__
#define StateMessageParamVector4_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector4.h>

namespace fb
{
    class StateMessageParamVector4 : public StateMessage
    {
    public:
        StateMessageParamVector4();
        ~StateMessageParamVector4() override;

        hash32 getId() const;
        void setId( hash32 val );

        Vector4F getValue() const;
        void setValue( const Vector4F &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        hash32 m_id;
        Vector4F m_value;
    };
}  // end namespace fb

#endif  // StateMessageParamVector4_h__
