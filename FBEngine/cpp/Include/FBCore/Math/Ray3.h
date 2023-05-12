#ifndef __FB_RAY_3D_H_
#define __FB_RAY_3D_H_

#include <FBCore/Math/Vector3.h>

namespace fb
{
    /**
     * @brief A 3D ray.
     *
     * @tparam T The type of the ray coordinates (e.g. f32, f64).
     */
    template <class T>
    class Ray3
    {
    public:
        /**
         * @brief Default constructor for the ray.
         */
        Ray3() = default;

        /**
         * @brief Constructor for the ray.
         *
         * @param origin The origin point of the ray.
         * @param direction The direction of the ray.
         */
        Ray3( const Vector3<T> &origin, const Vector3<T> &direction );

        /**
         * @brief Gets the origin point of the ray.
         *
         * @return The origin point of the ray as a 3D vector.
         */
        Vector3<T> getOrigin() const;

        /**
         * @brief Sets the origin point of the ray.
         *
         * @param origin The origin point of the ray as a 3D vector.
         */
        void setOrigin( const Vector3<T> &origin );

        /**
         * @brief Gets the direction of the ray.
         *
         * @return The direction of the ray as a 3D vector.
         */
        Vector3<T> getDirection() const;

        /**
         * @brief Sets the direction of the ray.
         *
         * @param direction The direction of the ray as a 3D vector.
         */
        void setDirection( const Vector3<T> &direction );

        /**
         * @brief Checks whether the ray is valid.
         *
         * A ray is considered invalid if its direction vector is a zero vector.
         *
         * @return true if the ray is valid, false otherwise.
         */
        bool isValid() const;

    private:
        /**
         * @brief The origin point of the ray.
         */
        Vector3<T> m_origin = Vector3<T>::zero();

        /**
         * @brief The direction of the ray.
         */
        Vector3<T> m_direction = Vector3<T>::zero();
    };

    template <class T>
    Ray3<T>::Ray3( const Vector3<T> &origin, const Vector3<T> &direction ) :
        m_origin( origin ),
        m_direction( direction )
    {
    }

    template <class T>
    Vector3<T> Ray3<T>::getOrigin() const
    {
        return m_origin;
    }

    template <class T>
    void Ray3<T>::setOrigin( const Vector3<T> &origin )
    {
        m_origin = origin;
    }

    template <class T>
    Vector3<T> Ray3<T>::getDirection() const
    {
        return m_direction;
    }

    template <class T>
    void Ray3<T>::setDirection( const Vector3<T> &direction )
    {
        m_direction = direction;
    }

    template <class T>
    bool Ray3<T>::isValid() const
    {
        return Math<T>::isFinite( m_origin.X() ) && Math<T>::isFinite( m_origin.Y() ) &&
               Math<T>::isFinite( m_origin.Z() ) && Math<T>::isFinite( m_direction.X() ) &&
               Math<T>::isFinite( m_direction.Y() ) && Math<T>::isFinite( m_direction.Z() );
    }

    /**
     * @brief Typedef for a 32-bit floating point ray.
     */
    using Ray3F = Ray3<f32>;

    /**
     * @brief Typedef for a 64-bit floating point ray.
     */
    using Ray3D = Ray3<f64>;

}  // end namespace fb

#endif
