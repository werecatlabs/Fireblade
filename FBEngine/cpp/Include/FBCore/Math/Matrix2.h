#ifndef __FB_MATRIX_2_H_INCLUDED__
#define __FB_MATRIX_2_H_INCLUDED__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    template <class T>
    class Matrix2
    {
    public:
        Matrix2();
        Matrix2( const Matrix2 &other );
        Matrix2( T angle );

        // matrix times vector M * v
        Vector2<T> operator*( const Vector2<T> &rkV ) const;

        /**
         * Set this matrix to be a rotation of the specified angle, in radians, around the z-axis.
         *
         * @param fAngle Angle to rotate by, in radians.
         */
        void fromAngle( T fAngle );

        /**
         * Extract the rotation angle from this matrix.
         *
         * @param rfAngle On return, contains the rotation angle, in radians.
         */
        void toAngle( T &rfAngle ) const;

        /**
         * Get an identity matrix.
         *
         * @return An identity matrix.
         */
        static Matrix2 identity();

    private:
        //! Matrix data, stored in row-major order
        T M[4];
    };

    template <class T>
    Matrix2<T>::Matrix2()
    {
    }

    template <class T>
    Matrix2<T>::Matrix2( const Matrix2 &other )
    {
        *this = other;
    }

    template <class T>
    Vector2<T> Matrix2<T>::operator*( const Vector2<T> &rkV ) const
    {
        return Vector2<T>( M[0] * rkV[0] + M[1] * rkV[1], M[2] * rkV[0] + M[3] * rkV[1] );
    }

    template <class T>
    void Matrix2<T>::fromAngle( T fAngle )
    {
        M[0] = Math<T>::Cos( fAngle );
        M[2] = Math<T>::Sin( fAngle );
        M[1] = -M[2];
        M[3] = M[0];
    }

    template <class T>
    void Matrix2<T>::toAngle( T &rfAngle ) const
    {
        // assert:  matrix is a rotation
        rfAngle = Math<T>::ATan2( M[2], M[0] );
    }

    template <class T>
    Matrix2<T> Matrix2<T>::identity()
    {
        static Matrix2 mat;
        return mat;
    }

    using Matrix2F = Matrix2<f32>;
    using Matrix2D = Matrix2<f64>;

}  // end namespace fb

#endif
