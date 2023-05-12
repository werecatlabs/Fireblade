#ifndef __FBParamList_H__
#define __FBParamList_H__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/FixedArray.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/AABB2.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Quaternion.h>
#include <map>

#define USE_FIXED_SIZE_PARAMS 0

namespace fb
{
    class Parameter
    {
    public:
        enum class ParameterType
        {
            PARAM_TYPE_NULL,
            PARAM_TYPE_BOOL,
            PARAM_TYPE_U8,
            PARAM_TYPE_U16,
            PARAM_TYPE_U32,
            PARAM_TYPE_S8,
            PARAM_TYPE_S16,
            PARAM_TYPE_S32,
            PARAM_TYPE_F32,
            PARAM_TYPE_S64,
            PARAM_TYPE_F64,
            PARAM_TYPE_CHAR_PTR,
            PARAM_TYPE_PTR,
            PARAM_TYPE_OBJECT,

            PARAM_TYPE_STR,
            PARAM_TYPE_VEC2I,
            PARAM_TYPE_VEC2F,
            PARAM_TYPE_VEC3I,
            PARAM_TYPE_VEC3F,

            PARAM_TYPE_ARRAY,

            PARAM_TYPE_COUNT
        };

        Parameter();

        explicit Parameter( bool data );

        explicit Parameter( const c8 *data );

        explicit Parameter( u8 data );

        explicit Parameter( u16 data );

        explicit Parameter( s32 data );

        explicit Parameter( u32 data );

        explicit Parameter( f32 data );

        explicit Parameter( s64 data );

        explicit Parameter( f64 data );

        explicit Parameter( void *data );

        explicit Parameter( const String& data );

        explicit Parameter( SmartPtr<ISharedObject> data );

        explicit Parameter( const Array<Parameter>& data );

        ~Parameter();

        inline void setBool( bool data );
        inline void setCharPtr( const c8 *data );
        inline void setU8( u8 data );
        inline void setU16( u16 data );
        inline void setS32( s32 data );
        inline void setU32( u32 data );
        inline void setF32( f32 data );
        inline void setS64( s64 data );
        inline void setF64( f64 data );
        inline void setPtr( void *data );

        void setObject( SmartPtr<ISharedObject> data );
        void setArray( const Array<Parameter>& data );
        void setStr( const String& data );

        inline bool getBool() const;
        inline c8 *getCharPtr() const;
        inline u8 getU8() const;
        inline u16 getU16() const;
        inline s32 getS32() const;
        inline u32 getU32() const;
        inline f32 getF32() const;
        inline s64 getS64() const;
        inline f64 getF64() const;
        inline void *getPtr() const;
        
        SmartPtr<ISharedObject> getObject() const;

        const Array<Parameter>& getArray() const;
        String getStr() const;

        bool operator==( const Parameter &other ) const;

        union Data
        {
            s32 bData;
            s32 iData;
            f32 fData;
            void *pData;
        };

        /// The parameter type.
        ParameterType type;

        /// The parameter data.
        Data data;

        /// The object data.
        SmartPtr<ISharedObject> object;

        /// String data
        String str;

        /** Array data */
        Array<Parameter> array;

        /// A null type.
        static const Parameter VOID_PARAM;
    };

#if USE_FIXED_SIZE_PARAMS
    static const u32 NUM_MAX_PARAMS = 4;
    using Parameters = FixedArray<Parameter, NUM_MAX_PARAMS>;
#else
    static const u32 NUM_MAX_PARAMS = 8;
    typedef Array<Parameter> Parameters;
#endif
}  // end namespace fb

#include <FBCore/Base/Parameter.inl>

#endif
