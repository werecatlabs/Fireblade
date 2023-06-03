#ifndef Transformation3_h__
#define Transformation3_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Matrix4.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{

    /**
     * Class used to store a 3d transformation.
     */
    template <class T>
    class Transform3
    {
    public:
        /** Default constructor. */
        Transform3();

        /**
         * Constructor with position and orientation parameters.
         * \param position The position of the transformation.
         * \param orientation The orientation of the transformation.
         */
        Transform3( const Vector3<T> &position, const Quaternion<T> &orientation );

        /**
         * Constructor with position, orientation, and scale parameters.
         * \param position The position of the transformation.
         * \param orientation The orientation of the transformation.
         * \param scale The scale of the transformation.
         */
        Transform3( const Vector3<T> &position, const Quaternion<T> &orientation,
                    const Vector3<T> &scale );

        /**
         * Constructor with an orientation parameter.
         * \param orientation The orientation of the transformation.
         */
        Transform3( const Quaternion<T> &orientation );

        /** Copy constructor. */
        Transform3( const Transform3 &t );

        /** Destructor. */
        ~Transform3();

        /**
         * Returns the position of the transformation.
         * \return The position of the transformation.
         */
        Vector3<T> getPosition() const;

        /**
         * Sets the position of the transformation.
         * \param val The new position of the transformation.
         */
        void setPosition( const Vector3<T> &val );

        /**
         * Returns the scale of the transformation.
         * \return The scale of the transformation.
         */
        Vector3<T> getScale() const;

        /**
         * Sets the scale of the transformation.
         * \param val The new scale of the transformation.
         */
        void setScale( const Vector3<T> &val );

        /**
         * Returns the orientation of the transformation.
         * \return The orientation of the transformation.
         */
        Quaternion<T> getOrientation() const;

        /**
         * Sets the orientation of the transformation.
         * \param val The new orientation of the transformation.
         */
        void setOrientation( const Quaternion<T> &val );

        /**
         * Returns the rotation of the transformation.
         * \return The rotation of the transformation.
         */
        Vector3<T> getRotation() const;

        /**
         * Sets the rotation of the transformation.
         * \param val The new rotation of the transformation.
         */
        void setRotation( const Vector3<T> &val );

        /**
         * Returns the transformation matrix of the transformation.
         * \return The transformation matrix of the transformation.
         */
        Matrix4<T> getTransformationMatrix() const;

        /**
         * Copy assignment operator.
         * \param transform The transformation to copy.
         */
        void operator=( const Transform3<T> &transform );

        /**
         * Equality operator.
         * \param other The other transformation to compare with.
         * \return True if the transformations are equal, false otherwise.
         */
        bool operator==( const Transform3<T> &other ) const;

        /**
         * Inequality operator.
         * \param other The other transformation to compare with.
         * \return True if the transformations are not equal, false otherwise.
         */
        bool operator!=( const Transform3<T> &other ) const;

        /**
         * Transforms a point by this transformation.
         * \param p The point to transform.
         * \return The transformed point.
         */
        Vector3<T> transformPoint( const Vector3<T> &p ) const;

        /**
         * Transforms a vector by this transformation.
         * \param dir The vector to transform.
         * \return The transformed vector.
         */
        Vector3<T> transformVector( const Vector3<T> &dir ) const;

        /** Transforms a point from local space to world space. */
        Vector3<T> inverseTransformPoint( const Vector3<T> &p ) const;

        /** Transforms a vector from local space to world space. */
        Vector3<T> inverseTransformVector( const Vector3<T> &dir ) const;

        /** Rotates a point from local space to world space. */
        Vector3<T> inverseRotate( const Vector3<T> &p ) const;

        /** Returns the up vector of the transform in world space. */
        Vector3<T> up() const;

        /** Returns the forward vector of the transform in world space. */
        Vector3<T> forward() const;

        /** Returns the right vector of the transform in world space. */
        Vector3<T> right() const;

        /** Sets the transform using the given position, rotation and scale. */
        void setTransform( const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scale );

        /** Sets the transform using another Transform3 instance. */
        void setTransform( const Transform3 &t );

        /** Gets the transform values (position, rotation and scale) into the provided arguments. */
        void getTransform( Vector3<T> &pos, Quaternion<T> &rot, Vector3<T> &scale );

        /** Transforms the local transform to the world transform given the parent transform and the local transform. */
        void transformFromParent( const Transform3 &parent, const Transform3 &local );

        /** Transforms the local transform to the world transform given the parent transform and the local transform. */
        void transformFromParent( const SmartPtr<Transform3<T>> &parent,
                                  const SmartPtr<Transform3<T>> &local );

        /**
         * Transforms a point from world space to local space.
         * @param parentTransform The world transform of the parent object.
         * @param worldTransform The world transform of the object being transformed.
         */
        void fromWorldToLocal( Transform3<T> &parentTransform, Transform3<T> &worldTransform );

        /**
         * Converts a position from world space to local space.
         * @param worldPos The position in world space.
         * @return The position in local space.
         */
        Vector3<T> convertWorldToLocalPosition( const Vector3<T> &worldPos );

        /**
         * Converts an orientation from world space to local space.
         * @param worldOrientation The orientation in world space.
         * @return The orientation in local space.
         */
        Quaternion<T> convertWorldToLocalOrientation( const Quaternion<T> &worldOrientation );

        /**
         * Converts a position from local space to world space.
         * @param localPos The position in local space.
         * @return The position in world space.
         */
        Vector3<T> convertLocalToWorldPosition( const Vector3<T> &localPos );

        /**
         * Converts an orientation from local space to world space.
         * @param localOrientation The orientation in local space.
         * @return The orientation in world space.
         */
        Quaternion<T> convertLocalToWorldOrientation( const Quaternion<T> &localOrientation );

        /**
         * Transforms an AABB from local space to world space.
         * @param aabb The AABB in local space.
         * @return The AABB in world space.
         */
        AABB3<T> transformAABB( const AABB3<T> &aabb );

        /**
         * Returns true if the transform is valid.
         * @return True if the transform is valid, false otherwise.
         */
        bool isValid() const;

        /**
         * Returns true if the transform is sane (i.e. not NaN or Inf).
         * @return True if the transform is sane, false otherwise.
         */
        bool isSane() const;

        /**
         * Returns true if the transform is finite.
         * @return True if the transform is finite, false otherwise.
         */
        bool isFinite() const;

        /**
         * Returns the inverse transform.
         * @return The inverse transform.
         */
        Transform3<T> getInverse() const;

        /**
         * Returns the object properties.
         * @return The object properties.
         */
        SmartPtr<Properties> getProperties() const;

        /**
         * Sets the object properties.
         * @param properties The object properties to set.
         */
        void setProperties( SmartPtr<Properties> properties );

    private:
        /**
         * The position of the transformation in world space.
         */
        Vector3<T> m_position = Vector3<T>::zero();

        /**
         * The scale of the transformation in world space.
         */
        Vector3<T> m_scale = Vector3<T>::unit();

        /**
         * The orientation of the transformation in world space.
         */
        Quaternion<T> m_orientation = Quaternion<T>::identity();
    };

    template <class T>
    Transform3<T>::Transform3() :
        m_position( Vector3<T>::zero() ),
        m_scale( Vector3<T>::unit() ),
        m_orientation( Quaternion<T>::identity() )
    {
    }

    template <class T>
    Transform3<T>::Transform3( const Transform3 &t )
    {
        FB_ASSERT( t.isSane() );
        *this = t;
    }

    template <class T>
    Transform3<T>::Transform3( const Quaternion<T> &orientation ) :
        m_position( Vector3<T>::zero() ),
        m_scale( Vector3<T>::unit() ),
        m_orientation( orientation )
    {
        FB_ASSERT( isSane() );
    }

    template <class T>
    Transform3<T>::Transform3( const Vector3<T> &position, const Quaternion<T> &orientation ) :
        m_position( position ),
        m_scale( Vector3<T>::unit() ),
        m_orientation( orientation )
    {
        FB_ASSERT( isSane() );
    }

    template <class T>
    Transform3<T>::Transform3( const Vector3<T> &position, const Quaternion<T> &orientation,
                               const Vector3<T> &scale ) :
        m_position( position ),
        m_scale( scale ),
        m_orientation( orientation )
    {
        FB_ASSERT( isSane() );
    }

    template <class T>
    Transform3<T>::~Transform3()
    {
    }

    template <class T>
    Vector3<T> Transform3<T>::getPosition() const
    {
        FB_ASSERT( isSane() );
        return m_position;
    }

    template <class T>
    void Transform3<T>::setPosition( const Vector3<T> &val )
    {
        FB_ASSERT( isSane() );
        FB_ASSERT( val.isFinite() );
        m_position = val;
    }

    template <class T>
    Vector3<T> Transform3<T>::getScale() const
    {
        FB_ASSERT( isSane() );
        return m_scale;
    }

    template <class T>
    void Transform3<T>::setScale( const Vector3<T> &val )
    {
        FB_ASSERT( isSane() );
        FB_ASSERT( val.isFinite() );
        m_scale = val;
    }

    template <class T>
    Quaternion<T> Transform3<T>::getOrientation() const
    {
        FB_ASSERT( isSane() );
        return m_orientation;
    }

    template <class T>
    void Transform3<T>::setOrientation( const Quaternion<T> &val )
    {
        FB_ASSERT( isSane() );
        FB_ASSERT( val.isSane() );
        m_orientation = val;
    }

    template <class T>
    Vector3<T> Transform3<T>::getRotation() const
    {
        FB_ASSERT( isSane() );
        Vector3<T> degrees;
        m_orientation.toEuler( degrees );
        return degrees;
    }

    template <class T>
    void Transform3<T>::setRotation( const Vector3<T> &rotation )
    {
        FB_ASSERT( isSane() );
        m_orientation = Quaternion<T>::angleAxis( rotation.Y(), Vector3<T>::UNIT_Y ) *
                        Quaternion<T>::angleAxis( rotation.X(), Vector3<T>::UNIT_X ) *
                        Quaternion<T>::angleAxis( rotation.Z(), Vector3<T>::UNIT_Z );
    }

    template <class T>
    Matrix4<T> Transform3<T>::getTransformationMatrix() const
    {
        FB_ASSERT( isSane() );

        Matrix4<T> mat;
        mat.makeTransform( m_position, m_scale, m_orientation );
        return mat;
    }

    template <class T>
    void Transform3<T>::operator=( const Transform3<T> &transform )
    {
        FB_ASSERT( isSane() );
        FB_ASSERT( transform.isSane() );
        m_position = transform.m_position;
        m_scale = transform.m_scale;
        m_orientation = transform.m_orientation;
    }

    template <class T>
    bool Transform3<T>::operator==( const Transform3<T> &other ) const
    {
        return m_position == other.m_position && m_orientation == other.m_orientation &&
               m_scale == other.m_scale;
    }

    template <class T>
    bool Transform3<T>::operator!=( const Transform3<T> &other ) const
    {
        return !( m_position == other.m_position && m_orientation == other.m_orientation &&
                  m_scale == other.m_scale );
    }

    template <class T>
    Vector3<T> Transform3<T>::transformPoint( const Vector3<T> &p ) const
    {
        FB_ASSERT( isSane() );
        return ( ( m_orientation * p ) + m_position ) * m_scale;
    }

    template <class T>
    Vector3<T> Transform3<T>::transformVector( const Vector3<T> &dir ) const
    {
        FB_ASSERT( isSane() );
        return m_orientation * dir;
    }

    template <class T>
    Vector3<T> Transform3<T>::inverseTransformPoint( const Vector3<T> &p ) const
    {
        FB_ASSERT( isSane() );
        return m_orientation.inverse() * ( p - m_position ) / m_scale;
    }

    template <class T>
    Vector3<T> Transform3<T>::inverseTransformVector( const Vector3<T> &dir ) const
    {
        FB_ASSERT( isSane() );
        Quaternion<T> inverseOrientation = m_orientation.inverse();
        return inverseOrientation * dir;
    }

    template <class T>
    Vector3<T> Transform3<T>::inverseRotate( const Vector3<T> &p ) const
    {
        FB_ASSERT( isSane() );
        return m_orientation.rotateInv( p );
    }

    template <class T>
    Vector3<T> Transform3<T>::up() const
    {
        FB_ASSERT( isSane() );
        return m_orientation * Vector3<T>::up();
    }

    template <class T>
    Vector3<T> Transform3<T>::forward() const
    {
        FB_ASSERT( isSane() );
        return m_orientation * Vector3<T>::forward();
    }

    template <class T>
    Vector3<T> Transform3<T>::right() const
    {
        FB_ASSERT( isSane() );
        return m_orientation * Vector3<T>::right();
    }

    template <class T>
    void Transform3<T>::setTransform( const Vector3<T> &pos, const Quaternion<T> &rot,
                                      const Vector3<T> &scale )
    {
        FB_ASSERT( isSane() );

        m_position = pos;
        m_orientation = rot;
        m_scale = scale;
    }

    template <class T>
    void Transform3<T>::setTransform( const Transform3 &t )
    {
        FB_ASSERT( isSane() );

        m_position = t.m_position;
        m_scale = t.m_scale;
        m_orientation = t.m_orientation;
    }

    template <class T>
    void Transform3<T>::getTransform( Vector3<T> &pos, Quaternion<T> &rot, Vector3<T> &scale )
    {
        FB_ASSERT( isSane() );

        pos = m_position;
        rot = m_orientation;
        scale = m_scale;
    }

    template <class T>
    void Transform3<T>::transformFromParent( const Transform3 &parent, const Transform3 &local )
    {
        FB_ASSERT( isSane() );

        const auto &scale = parent.m_scale;
        m_scale = scale * local.m_scale;
        m_orientation = parent.m_orientation * local.m_orientation;
        m_position = parent.m_orientation * ( scale * local.m_position );
        m_position += parent.m_position;
    }

    template <class T>
    void Transform3<T>::transformFromParent( const SmartPtr<Transform3<T>> &parent,
                                             const SmartPtr<Transform3<T>> &local )
    {
        FB_ASSERT( local );
        FB_ASSERT( isSane() );

        if( parent )
        {
            const Vector3<T> &scale = parent->m_scale;
            m_scale = scale * local->m_scale;
            m_orientation = parent->m_orientation * local->m_orientation;
            m_position = parent->m_orientation * ( scale * local->m_position );
            m_position += parent->m_position;
        }
        else
        {
            m_scale = local->m_scale;
            m_orientation = local->m_orientation;
            m_position = local->m_position;
        }
    }

    template <class T>
    Vector3<T> Transform3<T>::convertWorldToLocalPosition( const Vector3<T> &worldPos )
    {
        auto scale = getScale();
        auto pos = getPosition();
        auto rot = getOrientation();

        auto invScale = Vector3<T>::zero();
        if( Math<T>::Abs( scale.X() ) > std::numeric_limits<T>::epsilon() )
        {
            invScale.X() = T( 1.0 ) / scale.X();
        }

        if( Math<T>::Abs( scale.Y() ) > std::numeric_limits<T>::epsilon() )
        {
            invScale.Y() = T( 1.0 ) / scale.Y();
        }

        if( Math<T>::Abs( scale.Z() ) > std::numeric_limits<T>::epsilon() )
        {
            invScale.Z() = T( 1.0 ) / scale.Z();
        }

        return rot.inverse() * ( ( worldPos - pos ) * invScale );
    }

    template <class T>
    Quaternion<T> Transform3<T>::convertWorldToLocalOrientation( const Quaternion<T> &worldOrientation )
    {
        auto rot = getOrientation();
        return rot.inverse() * worldOrientation;
    }

    template <class T>
    Vector3<T> Transform3<T>::convertLocalToWorldPosition( const Vector3<T> &localPos )
    {
        auto scale = getScale();
        auto pos = getPosition();
        auto rot = getOrientation();

        return ( ( rot * localPos ) + pos ) * scale;
    }

    template <class T>
    Quaternion<T> Transform3<T>::convertLocalToWorldOrientation( const Quaternion<T> &localOrientation )
    {
        auto rot = getOrientation();
        return rot * localOrientation;
    }

    template <class T>
    bool Transform3<T>::isValid() const
    {
        return m_position.isFinite() && m_orientation.isFinite() && m_orientation.isUnit() &&
               m_scale.isFinite();
    }

    template <class T>
    bool Transform3<T>::isSane() const
    {
        return isFinite() && m_orientation.isSane();
    }

    template <class T>
    bool Transform3<T>::isFinite() const
    {
        return m_position.isFinite() && m_orientation.isFinite() && m_scale.isFinite();
    }

    template <class T>
    Transform3<T> Transform3<T>::getInverse() const
    {
        FB_ASSERT( isFinite() );
        return Transform3<T>( m_orientation.rotateInv( -m_position ), m_orientation.getConjugate() );
    }

    /// Typedef for a f32 transform.
    using Transform3I = Transform3<s32>;

    /// Typedef for a f32 transform.
    using Transform3F = Transform3<f32>;

    /// Typedef for a f64 transform.
    using Transform3D = Transform3<f64>;

}  // end namespace fb

#endif  // Transformation3_h__
