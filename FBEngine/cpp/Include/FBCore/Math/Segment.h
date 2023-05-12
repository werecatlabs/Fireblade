#ifndef __FB_SEGMENT_H_INCLUDED__
#define __FB_SEGMENT_H_INCLUDED__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Plane3.h>

namespace fb
{

    /**
     * @brief A line segment defined by a start point, direction and extent.
     *
     * @tparam T the type of the coordinates (e.g. float, double, int)
     */
    template <class T>
    class Segment
    {
    public:
        /**
         * @brief Creates a new segment with start point, direction, and extent.
         *
         * @param start the start point of the segment
         * @param direction the direction of the segment
         * @param extent the extent (length) of the segment
         */
        Segment( const Vector3<T> &start, const Vector3<T> &direction, T extent );

        /**
         * @brief Returns the start point of the segment.
         *
         * @return the start point of the segment
         */
        Vector3<T> getStart() const;

        /**
         * @brief Sets the start point of the segment.
         *
         * @param start the new start point of the segment
         */
        void setStart( const Vector3<T> &start );

        /**
         * @brief Returns the direction of the segment.
         *
         * @return the direction of the segment
         */
        Vector3<T> getDirection() const;

        /**
         * @brief Sets the direction of the segment.
         *
         * @param direction the new direction of the segment
         */
        void setDirection( const Vector3<T> &direction );

        /**
         * @brief Returns the extent (length) of the segment.
         *
         * @return the extent (length) of the segment
         */
        T getExtent() const;

        /**
         * @brief Sets the extent (length) of the segment.
         *
         * @param extent the new extent (length) of the segment
         */
        void setExtent( T extent );

    private:
        Vector3<T> m_start;      ///< The start point of the segment
        Vector3<T> m_direction;  ///< The direction of the segment (not necessarily normalized)
        T m_extent;              ///< The extent (length) of the segment
    };

    template <class T>
    Segment<T>::Segment( const Vector3<T> &start, const Vector3<T> &direction, T extent ) :
        m_start( start ),
        m_direction( direction ),
        m_extent( extent )
    {
    }

    template <class T>
    Vector3<T> Segment<T>::getStart() const
    {
        return m_start;
    }

    template <class T>
    void Segment<T>::setStart( const Vector3<T> &start )
    {
        m_start = start;
    }

    template <class T>
    Vector3<T> Segment<T>::getDirection() const
    {
        return m_direction;
    }

    template <class T>
    void Segment<T>::setDirection( const Vector3<T> &direction )
    {
        m_direction = direction;
    }

    template <class T>
    T Segment<T>::getExtent() const
    {
        return m_extent;
    }

    template <class T>
    void Segment<T>::setExtent( T extent )
    {
        m_extent = extent;
    }

}  // end namespace fb

#endif
