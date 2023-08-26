#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageFragmentParam.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageFragmentParam, StateMessage );

    //--------------------------------------------
    StateMessageFragmentParam::StateMessageFragmentParam()
    {
    }

    //--------------------------------------------
    StateMessageFragmentParam::~StateMessageFragmentParam()
    {
    }

    String StateMessageFragmentParam::getName() const
    {
        return m_name;
    }

    void StateMessageFragmentParam::setName( const String &val )
    {
        m_name = val;
    }

    //--------------------------------------------
    f32 StateMessageFragmentParam::getFloat() const
    {
        return m_data.fData[0];
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setFloat( f32 val )
    {
        m_data.fData[0] = val;
    }

    //--------------------------------------------
    Vector2F StateMessageFragmentParam::getVector2f() const
    {
        return Vector2F( m_data.fData[0], m_data.fData[1] );
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setVector2f( const Vector2F &val )
    {
        m_data.fData[0] = val[0];
        m_data.fData[1] = val[1];
    }

    //--------------------------------------------
    Vector3F StateMessageFragmentParam::getVector3f() const
    {
        return Vector3F( m_data.fData[0], m_data.fData[1], m_data.fData[2] );
    }

    //--------------------------------------------
    void StateMessageFragmentParam::setVector3f( const Vector3F &val )
    {
        m_data.fData[0] = val[0];
        m_data.fData[1] = val[1];
        m_data.fData[2] = val[2];
    }

    //--------------------------------------------
    Vector4F StateMessageFragmentParam::getVector4f() const
    {
        return Vector4F( m_data.fData[0], m_data.fData[1], m_data.fData[2], m_data.fData[3] );
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
    ColourF StateMessageFragmentParam::getColourf() const
    {
        return ColourF( m_data.fData[0], m_data.fData[1], m_data.fData[2], m_data.fData[3] );
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
