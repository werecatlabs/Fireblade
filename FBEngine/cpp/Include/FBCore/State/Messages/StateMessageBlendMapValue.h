#ifndef StateMessageBlendMapValue_h__
#define StateMessageBlendMapValue_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    //--------------------------------------------
    class StateMessageBlendMapValue : public StateMessage
    {
    public:
        StateMessageBlendMapValue();
        ~StateMessageBlendMapValue() override;

        Vector2I getCoordinates() const;
        void setCoordinates( const Vector2I &val );

        f32 getBlendValue() const;
        void setBlendValue( f32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector2I m_coordinates;
        f32 m_blendValue;
    };
}  // end namespace fb

#endif  // StateMessageBlendMapValue_h__
