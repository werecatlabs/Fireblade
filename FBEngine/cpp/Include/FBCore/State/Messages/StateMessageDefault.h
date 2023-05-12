#ifndef StateMessageDefault_h__
#define StateMessageDefault_h__

#include <FBCore/State/Messages/StateMessage.h>
#include "FBCore/Base/Parameter.h"
#include <FBCore/Base/HashMap.h>

namespace fb
{

    class StateMessageDefault : public StateMessage
    {
    public:
        StateMessageDefault();
        ~StateMessageDefault() override;

        s32 setProperty( hash_type hash, const String &value );
        s32 getProperty( hash_type hash, String &value ) const;

        s32 setProperty( hash_type hash, const Parameter &param );
        s32 setProperty( hash_type hash, const Parameters &params );
        s32 getProperty( hash_type hash, Parameter &param ) const;
        s32 getProperty( hash_type hash, Parameters &params ) const;

        void setSubjectId( u32 id );
        u32 getSubjectId() const;

        FB_CLASS_REGISTER_DECL;

    private:
        using StringMap = std::map<hash_type, String>;
        StringMap m_stringMap;

        using FBParamMap = std::map<hash_type, Parameter>;
        FBParamMap m_paramMap;

        using FBParamListMap = std::map<hash_type, Parameters>;
        FBParamListMap m_paramListMap;

        u32 m_subjectId;
    };
}  // end namespace fb

#endif  // StateMessageDefault_h__
