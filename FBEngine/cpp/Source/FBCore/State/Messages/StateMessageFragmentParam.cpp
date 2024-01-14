#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageFragmentParam.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageFragmentParam, StateMessage );

    //--------------------------------------------
    StateMessageFragmentParam::StateMessageFragmentParam() = default;

    //--------------------------------------------
    StateMessageFragmentParam::~StateMessageFragmentParam() = default;

    auto StateMessageFragmentParam::getName() const -> String
    {
        return m_name;
    }

    void StateMessageFragmentParam::setName( const String &val )
    {
        m_name = val;
    }

    //--------------------------------------------
    auto StateMessageFragmentParam::getFloat() const -> f32
    {
        return m_data.fData[0];
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setFloat( f32 val )
    {
        m_data.fData[0] = val;
    }

    //--------------------------------------------
    auto StateMessageFragmentParam::getVector2f() const -> Vector2F
    {
        return { m_data.fData[0], m_data.fData[1] };
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setVector2f( const Vector2F &val )
    {
        m_data.fData[0] = val[0];
        m_data.fData[1] = val[1];
    }

    //--------------------------------------------
    auto StateMessageFragmentParam::getVector3f() const -> Vector3F
    {
        return { m_data.fData[0], m_data.fData[1], m_data.fData[2] };
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setVector3f( const Vector3F &val )
    {
        m_data.fData[0] = val[0];
        m_data.fData[1] = val[1];
        m_data.fData[2] = val[2];
    }

    //--------------------------------------------
    auto StateMessageFragmentParam::getVector4f() const -> Vector4F
    {
        return { m_data.fData[0], m_data.fData[1], m_data.fData[2], m_data.fData[3] };
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setVector4f( const Vector4F &val )
    {
        m_data.fData[0] = val[0];
        m_data.fData[1] = val[1];
        m_data.fData[2] = val[2];
        m_data.fData[3] = val[3];
    }

    //--------------------------------------------
    auto StateMessageFragmentParam::getColourf() const -> ColourF
    {
        return { m_data.fData[0], m_data.fData[1], m_data.fData[2], m_data.fData[3] };
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setColourf( const ColourF &val )
    {
        m_data.fData[0] = val.r;
        m_data.fData[1] = val.g;
        m_data.fData[2] = val.b;
        m_data.fData[3] = val.a;
    }
}  // end namespace fb
