#ifndef _OgreUtil_H
#define _OgreUtil_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Core/StringTypes.h>
#include <OgreVector.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreAxisAlignedBox.h>

namespace fb
{
    namespace render
    {
        /** Provides functions to help deal with ogre. */
        class OgreUtil
        {
        public:
            static Ogre::String toString( const String &str );
            static Ogre::String toString( const StringW &str );

            static bool equals( const Ogre::Vector3 &a, const Ogre::Vector3 &b );
            static bool equals( const Ogre::Quaternion &a, const Ogre::Quaternion &b );

            static bool equals( const Ogre::ColourValue &a, const Ogre::ColourValue &b );

            static Ogre::ColourValue convertToOgre( const ColourF &c );
            static ColourF convert( const Ogre::ColourValue &c );

            /** Converts this engine's vector to an ogre vector. */
            template <class T>
            static Ogre::Vector2 convertToOgreVector( const Vector2<T> &vector );

            /** Converts this engine's vector to an ogre vector. */
            template <class T>
            static Ogre::Vector3 convertToOgreVector( const Vector3<T> &vector );

            static Vector3F convert( const Ogre::Vector3 &vector );

            /** Converts this engine's Quaternion to an ogre Quaternion. */
            static Ogre::Quaternion convertToOgreQuaternion( const QuaternionF &orienation );

            static QuaternionF convert( const Ogre::Quaternion &orienation );

            static AABB3F convert( const Ogre::AxisAlignedBox &box );

            static Ogre::Vector3 getDerivedPosition( Ogre::SceneNode *sceneNode );
            static Ogre::AxisAlignedBox getDerivedBoundingBox( Ogre::SceneNode *node );

            static void getVertexMinMax( Ogre::Entity *entity, Ogre::Vector3 &minPoint,
                                         Ogre::Vector3 &maxPoint );

            static int CalculateNearest2Pow( int input );
        };
    }  // namespace render
}  // end namespace fb

#include "OgreUtil.inl"

#endif
