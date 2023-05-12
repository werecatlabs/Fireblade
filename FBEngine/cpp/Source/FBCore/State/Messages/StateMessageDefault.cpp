#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageDefault.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageDefault, StateMessage );

    StateMessageDefault::StateMessageDefault()
    {
    }

    StateMessageDefault::~StateMessageDefault()
    {
    }

    s32 StateMessageDefault::setProperty( hash_type id, const String &value )
    {
        m_stringMap[id] = value;
        return 0;
    }

    s32 StateMessageDefault::getProperty( hash_type id, String &value ) const
    {
        auto it = m_stringMap.find( id );
        if( it != m_stringMap.end() )
        {
            value = it->second;
        }

        return 0;
    }

    s32 StateMessageDefault::setProperty( hash_type id, const Parameter &param )
    {
        m_paramMap[id] = param;
        return 0;
    }

    s32 StateMessageDefault::setProperty( hash_type id, const Parameters &params )
    {
        m_paramListMap[id] = params;
        return 0;
    }

    s32 StateMessageDefault::getProperty( hash_type id, Parameter &param ) const
    {
        auto it = m_paramMap.find( id );
        if( it != m_paramMap.end() )
        {
            param = it->second;
        }

        return 0;
    }

    s32 StateMessageDefault::getProperty( hash_type id, Parameters &params ) const
    {
        auto it = m_paramListMap.find( id );
        if( it != m_paramListMap.end() )
        {
            params = it->second;
        }

        return 0;
    }

    void StateMessageDefault::setSubjectId( u32 id )
    {
        m_subjectId = id;
    }

    u32 StateMessageDefault::getSubjectId() const
    {
        return m_subjectId;
    }
}  // end namespace fb
