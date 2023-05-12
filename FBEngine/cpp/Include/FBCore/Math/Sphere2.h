#ifndef Sphere2_h__
#define Sphere2_h__

#include <FBCore/Math/Vector2.h>

namespace fb
{
    /**
     * @brief A 2D sphere.
     *
     * @tparam T The numeric type used for the sphere's position and radius.
     */
    template <class T>
    class Sphere2
    {
    public:
        // Constructors

        /**
         * @brief Constructs a sphere with a center at the origin and a radius of 0.
         */
        Sphere2() = default;

        /**
         * @brief Constructs a sphere with the given center and radius.
         *
         * @param center The center of the sphere.
         * @param radius The radius of the sphere.
         */
        Sphere2( const Vector2<T> &center, f32 radius );

        /**
         * @brief Copy constructor.
         *
         * @param other The sphere to copy from.
         */
        Sphere2( const Sphere2<T> &other );

        // Operators

        /**
         * @brief Copy assignment operator.
         *
         * @param other The sphere to copy from.
         *
         * @return A reference to the assigned sphere.
         */
        Sphere2<T> &operator=( const Sphere2<T> &other );

        /**
         * @brief Checks if two spheres are equal.
         *
         * @param other The sphere to compare with.
         *
         * @return `true` if the two spheres are equal, `false` otherwise.
         */
        bool operator==( const Sphere2<T> &other ) const;

        /**
         * @brief Checks if two spheres are not equal.
         *
         * @param other The sphere to compare with.
         *
         * @return `true` if the two spheres are not equal, `false` otherwise.
         */
        bool operator!=( const Sphere2<T> &other ) const;

        // Functions

        /**
         * @brief Returns the center of the sphere.
         *
         * @return The center of the sphere.
         */
        Vector2<T> getCenter() const;

        /**
         * @brief Sets the center of the sphere.
         *
         * @param center The new center of the sphere.
         */
        void setCenter( Vector2<T> center );

        /**
         * @brief Returns the radius of the sphere.
         *
         * @return The radius of the sphere.
         */
        T getRadius() const;

        /**
         * @brief Sets the radius of the sphere.
         *
         * @param radius The new radius of the sphere.
         */
        void setRadius( T radius );

        /**
         * @brief Tests if this sphere intersects with another sphere.
         *
         * @param other The sphere to test intersection with.
         *
         * @return `true` if the spheres intersect, `false` otherwise.
         */
        bool intersects( const Sphere2<T> &other ) const;

        /**
         * @brief Tests if this sphere intersects with another sphere.
         *
         * @param other The sphere to test intersection with.
         *
         * @return `true` if the spheres intersect, `false` otherwise.
         *
         * This uses the squared distance of centers and squared sum of radii for speed.
         */
        bool intersectsSQ( const Sphere2<T> &other ) const;

    private:
        Vector2<T> m_center = Vector2<T>::zero(); /**< The center of the sphere. */
        T m_radius = T( 0.0 );                    /**< The radius of the sphere. */
    };

    template <class T>
    Sphere2<T>::Sphere2( const Vector2<T> &center, const f32 radius ) :
        m_center( center ),
        m_radius( radius )
    {
    }

    template <class T>
    Sphere2<T>::Sphere2( const Sphere2<T> &other ) :
        m_center( other.getCenter() ),
        m_radius( other.getRadius() )
    {
    }

    template <class T>
    Sphere2<T> &Sphere2<T>::operator=( const Sphere2<T> &other )
    {
        m_center = other.getCenter();
        m_radius = other.getRadius();
        return *this;
    }

    template <class T>
    bool Sphere2<T>::operator==( const Sphere2<T> &other ) const
    {
        return ( m_center == other.getCenter() && m_radius == other.getRadius() );
    }

    template <class T>
    bool Sphere2<T>::operator!=( const Sphere2<T> &other ) const
    {
        return !( m_center == other.getCenter() && m_radius == other.getRadius() );
    }

    template <class T>
    Vector2<T> Sphere2<T>::getCenter() const
    {
        return m_center;
    }

    template <class T>
    void Sphere2<T>::setCenter( Vector2<T> center )
    {
        m_center = center;
    }

    template <class T>
    T Sphere2<T>::getRadius() const
    {
        return m_radius;
    }

    template <class T>
    void Sphere2<T>::setRadius( T radius )
    {
        m_radius = radius;
    }

    template <class T>
    bool Sphere2<T>::intersects( const Sphere2<T> &other ) const
    {
        Vector2<T> vec = m_center - other.getCenter();
        T radii = m_radius + other.getRadius();
        if( vec.length() > radii )
        {
            return false;
        }

        return true;
    }

    template <class T>
    bool Sphere2<T>::intersectsSQ( const Sphere2<T> &other ) const
    {
        Vector2<T> vec = m_center - other.getCenter();
        T radii = m_radius + other.getRadius();
        if( vec.squaredLength() > ( radii * radii ) )
        {
            return false;
        }

        return true;
    }

    using Sphere2F = Sphere2<f32>;
    using Sphere2D = Sphere2<f64>;

}  // end namespace fb

#endif  // Sphere2_h__
