#ifndef StateMessageFragmentParam_h__
#define StateMessageFragmentParam_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Core/ColourUtil.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageFragmentParam : public StateMessage
    {
    public:
        StateMessageFragmentParam();
        ~StateMessageFragmentParam() override;

        String getName() const;
        void setName( const String &val );

        f32 getFloat() const;
        void setFloat( f32 val );

        Vector2<real_Num> getVector2f() const;
        void setVector2f( const Vector2<real_Num> &val );

        Vector3<real_Num> getVector3f() const;
        void setVector3f( const Vector3<real_Num> &val );

        Vector4F getVector4f() const;
        void setVector4f( const Vector4F &val );

        ColourF getColourf() const;
        void setColourf( const ColourF &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        String m_name;

        union Data
        {
            s32 iData[4];
            f32 fData[4];
        };

        Data m_data;
    };
}  // end namespace fb

#endif  // StateMessageFragmentParam_h__
