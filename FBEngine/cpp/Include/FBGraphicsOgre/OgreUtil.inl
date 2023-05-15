namespace fb
{
    namespace render
    {
        template <class T>
        Ogre::Vector2 OgreUtil::convertToOgreVector( const Vector2<T> &vector )
        {
            return Ogre::Vector2( vector.X(), vector.Y() );
        }

        template <class T>
        Ogre::Vector3 OgreUtil::convertToOgreVector( const Vector3<T> &vector )
        {
            return Ogre::Vector3( vector.X(), vector.Y(), vector.Z() );
        }

        inline Ogre::Quaternion convertToOgreVector( const QuaternionF &orienation )
        {
            return Ogre::Quaternion( orienation.W(), orienation.X(), orienation.Y(), orienation.Z() );
        }
    } // namespace render
}     // namespace fb
