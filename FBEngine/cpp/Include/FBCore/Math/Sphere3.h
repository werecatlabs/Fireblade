#ifndef Sphere3_h__
#define Sphere3_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    /**
     * A 3D sphere class.
     */
    template <class T>
    class Sphere3
    {
    public:
        /**
         * Default constructor.
         */
        Sphere3() = default;

        /**
         * Constructs a new sphere with the given center and radius.
         *
         * @param center The center of the sphere.
         * @param radius The radius of the sphere.
         */
        Sphere3( const Vector3<T> &center, T radius );

        /**
         * Copy constructor.
         *
         * @param other The sphere to copy.
         */
        Sphere3( const Sphere3<T> &other );

        /**
         * Assignment operator.
         *
         * @param other The sphere to copy.
         * @return A reference to this sphere.
         */
        Sphere3<T> &operator=( const Sphere3<T> &other );

        /**
         * Equality operator.
         *
         * @param other The sphere to compare to.
         * @return `true` if the spheres are equal, `false` otherwise.
         */
        bool operator==( const Sphere3<T> &other ) const;

        /**
         * Inequality operator.
         *
         * @param other The sphere to compare to.
         * @return `true` if the spheres are not equal, `false` otherwise.
         */
        bool operator!=( const Sphere3<T> &other ) const;

        /**
         * Returns the center of the sphere.
         *
         * @return The center of the sphere.
         */
        Vector3<T> getCenter() const;

        /**
         * Sets the center of the sphere.
         *
         * @param center The new center of the sphere.
         */
        void setCenter( Vector3<T> center );

        /**
         * Returns the radius of the sphere.
         *
         * @return The radius of the sphere.
         */
        T getRadius() const;

        /**
         * Sets the radius of the sphere.
         *
         * @param radius The new radius of the sphere.
         */
        void setRadius( T radius );

        /**
         * Tests if this sphere intersects with another sphere.
         *
         * @param other The sphere to test intersection with.
         * @return `true` if the spheres intersect, `false` otherwise.
         */
        bool intersects( const Sphere3<T> &other ) const;

        /**
         * Tests if this sphere intersects with another sphere.
         *
         * This method is faster than `intersects` since it avoids computing square roots.
         *
         * @param other The sphere to test intersection with.
         * @return `true` if the spheres intersect, `false` otherwise.
         */
        bool intersectsSQ( const Sphere3<T> &other ) const;

        /**
         * Tests if this sphere intersects with an axis-aligned bounding box.
         *
         * @param box The box to test intersection with.
         * @return `true` if the sphere and box intersect, `false` otherwise.
         */
        bool intersects( const AABB3<T> &box ) const;

    private:
        Vector3<T> m_center = Vector3<T>::zero();  //!< The center of the sphere.
        T m_radius = T( 0.0 );                     //!< The radius of the sphere.
    };

    template <class T>
    Sphere3<T>::Sphere3( const Vector3<T> &center, T radius ) : m_center( center ), m_radius( radius )
    {
    }

    template <class T>
    Sphere3<T>::Sphere3( const Sphere3<T> &other ) :
        m_center( other.getCenter() ),
        m_radius( other.getRadius() )
    {
    }

    template <class T>
    Sphere3<T> &Sphere3<T>::operator=( const Sphere3<T> &other )
    {
        m_center = other.getCenter();
        m_radius = other.getRadius();
        return *this;
    }

    template <class T>
    bool Sphere3<T>::operator==( const Sphere3<T> &other ) const
    {
        return ( m_center == other.getCenter() && m_radius == other.getRadius() );
    }

    template <class T>
    bool Sphere3<T>::operator!=( const Sphere3<T> &other ) const
    {
        return !( m_center == other.getCenter() && m_radius == other.getRadius() );
    }

    template <class T>
    Vector3<T> Sphere3<T>::getCenter() const
    {
        return m_center;
    }

    template <class T>
    void Sphere3<T>::setCenter( Vector3<T> center )
    {
        m_center = center;
    }

    template <class T>
    T Sphere3<T>::getRadius() const
    {
        return m_radius;
    }

    template <class T>
    void Sphere3<T>::setRadius( T radius )
    {
        m_radius = radius;
    }

    template <class T>
    bool Sphere3<T>::intersects( const Sphere3<T> &other ) const
    {
        Vector3<T> vec = m_center - other.getCenter();
        T radii = m_radius + other.getRadius();
        if( vec.length() > radii )
        {
            return false;
        }

        return true;
    }

    template <class T>
    bool Sphere3<T>::intersectsSQ( const Sphere3<T> &other ) const
    {
        Vector3<T> vec = m_center - other.getCenter();
        T radii = m_radius + other.getRadius();
        if( vec.squaredLength() > ( radii * radii ) )
        {
            return false;
        }
        return true;
    }

    /*
    template <class T>
    bool Sphere3<T>::intersects(const SCone& cone) const
    {
        f32 invSin = 1.0f / cone.sinAngle;
        f32 cosSQ = cone.cosAngle * cone.cosAngle;

        vector3<T> centerVertex = center - cone.vertex;
        vector3<T> kd = centerVertex + (cone.axis * (Radius * invSin));
        f64 kdSQ = kd.squaredLength();

        f64 E = kd.dotProduct(cone.axis);
        if(E > 0.0f && E * E >= kdSQ * cosSQ)
        {
            f64 sinSQ = cone.sinAngle * cone.sinAngle;
            kdSQ = centerVertex.squaredLength();
            E = -centerVertex.dotProduct(cone.axis);
            if( E > 0.0f && E * E >= kdSQ * sinSQ)
            {
                f32 rSQ = Radius * Radius;
                return kdSQ <= rSQ;
            }
            return true;
        }
        return false;
    }
    */

    template <class T>
    bool Sphere3<T>::intersects( const AABB3<T> &box ) const
    {
        Vector3<T> center = box.getCenter();
        Vector3<T> halfSize = box.getExtent() * 0.5f;

        Vector3<T> dir = center - m_center;
        T distanceToCenter = dir.normaliseLength();

        T distanceBetween = dir.dotProduct( halfSize ) + m_radius;

        if( fabs( distanceToCenter ) > fabs( distanceBetween ) )
        {
            return false;
        }

        return true;
    }

    using Sphere3F = Sphere3<f32>;
    using Sphere3D = Sphere3<f64>;

}  // end namespace fb

#endif  // Sphere3_h__
