#ifndef SaracenVec_h__
#define SaracenVec_h__

#include <FBCore/FBCorePrerequisites.h>
#include "FBCore/Math/Vector3.h"
#include "FBCore/Math/Quaternion.h"
#include <xmmintrin.h>

namespace fb
{
    class VectorData
    {
    public:
        VectorData();

        VectorData( const VectorData &other );

        VectorData( f32 fX, f32 fY, f32 fZ, f32 fW );

        VectorData &operator=( const VectorData &other );

        Vector3<real_Num> toVector();

        Quaternion<real_Num> toRotation();

        Vector3<real_Num> toCoordsVector();

        Quaternion<real_Num> toCoordsRotation();

        bool isValid() const;

        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 z = 0.0f;
        f32 w = 0.0f;
    };

    extern VectorData MakeSaracenVec();
    extern VectorData MakeSaracenVec( f32 fX, f32 fY, f32 fZ, f32 fW = 0.0f );
    extern VectorData getSaracenVec( const Vector3<real_Num> &vec );
    extern VectorData getSaracenQuat( const Quaternion<real_Num> &vec );
    extern void getSaracenVec4( const Vector3<real_Num> &vec, data::vec4 &saracenVec );
    extern void getSaracenQuatVec4( const Quaternion<real_Num> &vec, data::vec4 &saracenVec );

    //	void operator = (const saracen::Vector3<real_Num>& rkVector)
    //	{
    //#if FB_USE_SSE
    //		m_vec128 = rkVector.m_vec128;
    //#else
    //		x = rkVector.x;
    //		y = rkVector.y;
    //		z = rkVector.z;
    //		w = rkVector.w;
    //#endif
    //	}
    //
    //	void operator = (const saracen::Quaternion<real_Num>& rkVector)
    //	{
    //#if FB_USE_SSE
    //		m_vec128 = rkVector.m_vec128;
    //#else
    //		x = rkVector.x;
    //		y = rkVector.y;
    //		z = rkVector.z;
    //		w = rkVector.w;
    //#endif
    //	}
}  // namespace fb

#endif  // SaracenVec_h__
