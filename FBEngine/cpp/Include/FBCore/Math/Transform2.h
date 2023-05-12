#ifndef Transform2_h__
#define Transform2_h__

#include <FBCore/Math/Vector2.h>

namespace fb
{

    /**
     * Class used to store a 2D transformation.
     */
    template <class T>
    class Transform2
    {
    public:
        /**
         * Default constructor.
         */
        Transform2() = default;

        /**
         * Constructor that takes the position and rotation of the transformation.
         * @param position The position of the transformation.
         * @param rotation The rotation of the transformation.
         */
        Transform2( const Vector2<T> &position, T rotation );

        /**
         * Destructor.
         */
        ~Transform2() = default;

        /**
         * Get the position of the transformation.
         * @return The position of the transformation.
         */
        Vector2<T> getPosition() const;

        /**
         * Set the position of the transformation.
         * @param position The new position of the transformation.
         */
        void setPosition( const Vector2<T> &position );

        /**
         * Get the rotation of the transformation.
         * @return The rotation of the transformation.
         */
        T getRotation() const;

        /**
         * Set the rotation of the transformation.
         * @param rotation The new rotation of the transformation.
         */
        void setRotation( T rotation );

    protected:
        Vector2<T> m_position = Vector2<T>::zero(); /**< The position of the transformation. */
        T m_rotation = T( 0.0 );                    /**< The rotation of the transformation. */
    };

    template <class T>
    Transform2<T>::Transform2( const Vector2<T> &position, T rotation ) :
        m_position( position ),
        m_rotation( rotation )
    {
    }

    template <class T>
    Vector2<T> Transform2<T>::getPosition() const
    {
        return m_position;
    }

    template <class T>
    void Transform2<T>::setPosition( const Vector2<T> &position )
    {
        m_position = position;
    }

    template <class T>
    T Transform2<T>::getRotation() const
    {
        return m_rotation;
    }

    template <class T>
    void Transform2<T>::setRotation( T rotation )
    {
        m_rotation = rotation;
    }

}  // end namespace fb

#endif  // Transform2_h__
