namespace fb
{

    inline void Parameter::setBool( bool val )
    {
        type = ParameterType::PARAM_TYPE_BOOL;
        data.bData = val;
    }

    inline void Parameter::setCharPtr( const c8 *val )
    {
        type = ParameterType::PARAM_TYPE_CHAR_PTR;
        data.pData = (void*)val;
    }

    inline void Parameter::setU8( u8 val )
    {
        type = ParameterType::PARAM_TYPE_U8;
        data.iData = val;
    }

    inline void Parameter::setU16( u16 val )
    {
        type = ParameterType::PARAM_TYPE_U16;
        data.iData = val;
    }

    inline void Parameter::setS32( s32 val )
    {
        type = ParameterType::PARAM_TYPE_S32;
        data.iData = val;
    }

    inline void Parameter::setU32( u32 val )
    {
        type = ParameterType::PARAM_TYPE_U32;
        data.iData = static_cast<s32>( val );
    }

    inline void Parameter::setF32( f32 val )
    {
        type = ParameterType::PARAM_TYPE_F32;
        data.fData = val;
    }

    inline void Parameter::setS64( s64 val )
    {
        type = ParameterType::PARAM_TYPE_S64;
        data.iData = static_cast<s32>( val );
    }

    inline void Parameter::setF64( f64 val )
    {
        type = ParameterType::PARAM_TYPE_F64;
        data.fData = static_cast<f32>( val );
    }

    inline void Parameter::setPtr( void *val )
    {
        type = ParameterType::PARAM_TYPE_PTR;
        data.pData = val;
    }

    inline bool Parameter::getBool() const
    {
        //FB_ASSERT(type == PARAM_TYPE_BOOL );
        return data.bData == 1;
    }

    inline c8 *Parameter::getCharPtr() const
    {
        return static_cast<c8 *>( data.pData );
    }

    inline u8 Parameter::getU8() const
    {
        return static_cast<u8>( data.iData );
    }

    inline u16 Parameter::getU16() const
    {
        return static_cast<u16>( data.iData );
    }

    inline s32 Parameter::getS32() const
    {
        return data.iData;
    }

    inline u32 Parameter::getU32() const
    {
        return static_cast<u32>( data.iData );
    }

    inline f32 Parameter::getF32() const
    {
        return data.fData;
    }

    inline s64 Parameter::getS64() const
    {
        return data.iData;
    }

    inline f64 Parameter::getF64() const
    {
        return data.fData;
    }

    inline void *Parameter::getPtr() const
    {
        //FB_ASSERT(type == PARAM_TYPE_PTR);
        return data.pData;
    }
}  // end namespace fb
