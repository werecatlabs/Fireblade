#ifndef FBParamUtil_h__
#define FBParamUtil_h__

#include <FBCore/Core/Parameter.h>

namespace fb
{
    /** A utility class to make using parameter easier. */
    class FBCore_API ParameterUtil
    {
    public:
        /** Creates a parameter list from the properties of a 2d vector. */
        static Parameters setVector2( const Vector2F &vec );

        /** Creates a 2d vector from the properties in the parameter list. */
        static Vector2F getVector2( const Parameters &params );

        /** Creates a 3d vector from the properties in the parameter list. */
        static Vector3F getVector3( const Parameters &params );

        /** Creates a 2d aabb from the properties in the parameter list. */
        static AABB2F getAABB2( const Parameters &params );

        /** */
        static String getParamTypeAsString( u32 paramType );

        /** */
        static u32 getParamTypeFromString( const String &paramTypeStr );
    };
}  // end namespace fb

#endif  // FBParamUtil_h__
