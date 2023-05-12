#ifndef __FB_MATRIX_3_H_INCLUDED__
#define __FB_MATRIX_3_H_INCLUDED__

#include <FBCore/Math/Vector3.h>

namespace fb
{

    /**
     * A 3x3 matrix class template.
     *
     * @tparam T the data type of the matrix elements.
     */
    template <class T>
    class Matrix3
    {
    public:
        /**
         * Default constructor. Initializes the matrix to the identity matrix.
         */
        Matrix3();

        /**
         * Copy constructor.
         *
         * @param other the matrix to copy from.
         */
        Matrix3( const Matrix3 &other );

        /**
         * Constructs a matrix with the given entries.
         *
         * @param fEntry00 the (0, 0) entry of the matrix.
         * @param fEntry01 the (0, 1) entry of the matrix.
         * @param fEntry02 the (0, 2) entry of the matrix.
         * @param fEntry10 the (1, 0) entry of the matrix.
         * @param fEntry11 the (1, 1) entry of the matrix.
         * @param fEntry12 the (1, 2) entry of the matrix.
         * @param fEntry20 the (2, 0) entry of the matrix.
         * @param fEntry21 the (2, 1) entry of the matrix.
         * @param fEntry22 the (2, 2) entry of the matrix.
         */
        Matrix3( T fEntry00, T fEntry01, T fEntry02, T fEntry10, T fEntry11, T fEntry12, T fEntry20,
                 T fEntry21, T fEntry22 );

        /**
         * Allows matrix elements to be accessed using the [][ ] operator.
         *
         * @param rowIndex the row index of the element to access.
         *
         * @return a pointer to the row.
         */
        T *operator[]( u32 rowIndex ) const;

        /**
         * Assignment operator.
         *
         * @param rkMatrix the matrix to assign from.
         *
         * @return a reference to this matrix.
         */
        Matrix3 &operator=( const Matrix3 &rkMatrix );

        /**
         * Tests whether two matrices are equal.
         *
         * @param rkMatrix the matrix to compare with.
         *
         * @return true if the matrices are equal, false otherwise.
         */
        bool operator==( const Matrix3 &rkMatrix ) const;

        /**
         * Tests whether two matrices are unequal.
         *
         * @param rkMatrix the matrix to compare with.
         *
         * @return true if the matrices are unequal, false otherwise.
         */
        bool operator!=( const Matrix3 &rkMatrix ) const;

        /**
         * Adds two matrices.
         *
         * @param rkMatrix the matrix to add.
         *
         * @return the sum of the two matrices.
         */
        Matrix3 operator+( const Matrix3 &rkMatrix ) const;

        /**
         * Subtracts one matrix from another.
         *
         * @param rkMatrix the matrix to subtract.
         *
         * @return the difference between the two matrices.
         */
        Matrix3 operator-( const Matrix3 &rkMatrix ) const;

        /**
         * Multiplies two matrices.
         *
         * @param rkMatrix the matrix to multiply.
         *
         * @return the product of the two matrices.
         */
        Matrix3 operator*( const Matrix3 &rkMatrix ) const;

        /**
         * Negates the matrix.
         *
         * @return the negation of the matrix.
         */
        Matrix3 operator-() const;

        /**
         * Multiplies this matrix by a 3-D vector and returns the resulting vector.
         * @param rkVector The 3-D vector to multiply this matrix by.
         * @return The resulting 3-D vector after the multiplication.
         */
        Vector3<T> operator*( const Vector3<T> &rkVector ) const;

        /**
         * Multiplies a 3-D vector by the given matrix and returns the resulting vector.
         * @param rkVector The 3-D vector to multiply by the matrix.
         * @param rkMatrix The matrix to multiply the vector by.
         * @return The resulting 3-D vector after the multiplication.
         */
        template <class B>
        friend Vector3<B> operator*( const Vector3<B> &rkVector, const Matrix3<B> &rkMatrix );

        /**
         * Multiplies this matrix by a scalar and returns the resulting matrix.
         * @param fScalar The scalar to multiply this matrix by.
         * @return The resulting matrix after the multiplication.
         */
        Matrix3 operator*( T fScalar ) const;

        /**
         * Multiplies a scalar by the given matrix and returns the resulting matrix.
         * @param fScalar The scalar to multiply the matrix by.
         * @param rkMatrix The matrix to multiply by the scalar.
         * @return The resulting matrix after the multiplication.
         */
        template <class B>
        friend Matrix3<B> operator*( B fScalar, const Matrix3<B> &rkMatrix );

        /**
         * Calculates the Euler angles (yaw, pitch, roll) of a rotation represented by a 3x3 orthonormal matrix in XYZ order.
         *
         * @param rfYAngle [out] The resulting yaw angle (rotation about the Up vector).
         * @param rfPAngle [out] The resulting pitch angle (rotation about the Right axis).
         * @param rfRAngle [out] The resulting roll angle (rotation about the Direction axis).
         *
         * @return True if successful, false otherwise. The matrix must be orthonormal.
         */
        bool toEulerAnglesXYZ( T &rfYAngle, T &rfPAngle, T &rfRAngle ) const;

        /**
         * Calculates the Euler angles (yaw, pitch, roll) of a rotation represented by a 3x3 orthonormal matrix in XZY order.
         *
         * @param rfYAngle [out] The resulting yaw angle (rotation about the Up vector).
         * @param rfPAngle [out] The resulting pitch angle (rotation about the Forward axis).
         * @param rfRAngle [out] The resulting roll angle (rotation about the Right axis).
         *
         * @return True if successful, false otherwise. The matrix must be orthonormal.
         */
        bool toEulerAnglesXZY( T &rfYAngle, T &rfPAngle, T &rfRAngle ) const;

        /**
         * Calculates the Euler angles (yaw, pitch, roll) of a rotation represented by a 3x3 orthonormal matrix in YXZ order.
         *
         * @param rfYAngle [out] The resulting yaw angle (rotation about the Up vector).
         * @param rfPAngle [out] The resulting pitch angle (rotation about the Right axis).
         * @param rfRAngle [out] The resulting roll angle (rotation about the Forward axis).
         *
         * @return True if successful, false otherwise. The matrix must be orthonormal.
         */
        bool toEulerAnglesYXZ( T &rfYAngle, T &rfPAngle, T &rfRAngle ) const;

        /**
         * Calculates the Euler angles (yaw, pitch, roll) of a rotation represented by a 3x3 orthonormal matrix in YZX order.
         *
         * @param rfYAngle [out] The resulting yaw angle (rotation about the Right axis).
         * @param rfPAngle [out] The resulting pitch angle (rotation about the Forward axis).
         * @param rfRAngle [out] The resulting roll angle (rotation about the Up vector).
         *
         * @return True if successful, false otherwise. The matrix must be orthonormal.
         */
        bool toEulerAnglesYZX( T &rfYAngle, T &rfPAngle, T &rfRAngle ) const;

        /**
         * Calculates the Euler angles (yaw, pitch, roll) of a rotation represented by a 3x3 orthonormal matrix in ZXY order.
         *
         * @param rfYAngle [out] The resulting yaw angle (rotation about the Right axis).
         * @param rfPAngle [out] The resulting pitch angle (rotation about the Up vector).
         * @param rfRAngle [out] The resulting roll angle (rotation about the Forward axis).
         *
         * @return True if successful, false otherwise. The matrix must be orthonormal.
         */
        bool toEulerAnglesZXY( T &rfYAngle, T &rfPAngle, T &rfRAngle ) const;

        /**
         * Calculates the Euler angles using the ZYX convention (yaw, pitch, roll)
         * from the orthonormal Matrix3 object.
         * @param[out] rfYAngle The yaw (heading) angle in radians.
         * @param[out] rfPAngle The pitch (attitude) angle in radians.
         * @param[out] rfRAngle The roll (bank) angle in radians.
         * @return true if the operation is successful, false otherwise.
         */
        bool toEulerAnglesZYX( T &rfYAngle, T &rfPAngle, T &rfRAngle ) const;

        /**
         * Sets the rotation matrix using the ZYX convention (yaw, pitch, roll).
         * @param fYAngle The yaw (heading) angle in radians.
         * @param fPAngle The pitch (attitude) angle in radians.
         * @param fRAngle The roll (bank) angle in radians.
         */
        void fromEulerAnglesXYZ( const T &fYAngle, const T &fPAngle, const T &fRAngle );

        /**
         * Sets the rotation matrix using the XZY convention (yaw, pitch, roll).
         * @param fYAngle The yaw (heading) angle in radians.
         * @param fPAngle The pitch (attitude) angle in radians.
         * @param fRAngle The roll (bank) angle in radians.
         */
        void fromEulerAnglesXZY( const T &fYAngle, const T &fPAngle, const T &fRAngle );

        /**
         * Sets the rotation matrix using the YXZ convention (yaw, pitch, roll).
         * @param fYAngle The yaw (heading) angle in radians.
         * @param fPAngle The pitch (attitude) angle in radians.
         * @param fRAngle The roll (bank) angle in radians.
         */
        void fromEulerAnglesYXZ( const T &fYAngle, const T &fPAngle, const T &fRAngle );

        /**
         * Set this matrix to represent the orientation formed by a rotation around the Y-axis by
         * 'fYAngle' radians, followed by a rotation around the X-axis by 'fPAngle' radians, followed
         * by a rotation around the Z-axis by 'fRAngle' radians. The rotations are performed in
         * 'YXZ' order. This function assumes that the angles passed in are in radians.
         *
         * @param fYAngle The rotation around the Y-axis in radians.
         * @param fPAngle The rotation around the X-axis in radians.
         * @param fRAngle The rotation around the Z-axis in radians.
         */
        void fromEulerAnglesYZX( const T &fYAngle, const T &fPAngle, const T &fRAngle );

        void fromEulerAnglesZXY( const T &fYAngle, const T &fPAngle, const T &fRAngle );

        void fromEulerAnglesZYX( const T &fYAngle, const T &fPAngle, const T &fRAngle );

        void QDUDecomposition( Matrix3<T> &kQ, Vector3<T> &kD, Vector3<T> &kU ) const;

        Matrix3<T> transpose() const;

        static Matrix3 identity();

        union
        {
            T m[3][3];
            T m_[9];
        };
    };

    template <class T>
    Matrix3<T>::Matrix3()
    {
    }

    template <class T>
    Matrix3<T>::Matrix3( T fEntry00, T fEntry01, T fEntry02, T fEntry10, T fEntry11, T fEntry12,
                         T fEntry20, T fEntry21, T fEntry22 )
    {
        m[0][0] = fEntry00;
        m[0][1] = fEntry01;
        m[0][2] = fEntry02;
        m[1][0] = fEntry10;
        m[1][1] = fEntry11;
        m[1][2] = fEntry12;
        m[2][0] = fEntry20;
        m[2][1] = fEntry21;
        m[2][2] = fEntry22;
    }

    template <class T>
    T *Matrix3<T>::operator[]( u32 rowIndex ) const
    {
        auto p = const_cast<T *>( m[rowIndex] );
        return static_cast<T *>( p );
    }

    template <class T>
    Matrix3<T> &Matrix3<T>::operator=( const Matrix3<T> &rkMatrix )
    {
        Memory::Memcpy( m, rkMatrix.m, 9 * sizeof( T ) );
        return *this;
    }

    template <class T>
    bool Matrix3<T>::operator!=( const Matrix3<T> &rkMatrix ) const
    {
        return !operator==( rkMatrix );
    }

    template <class T>
    Matrix3<T> Matrix3<T>::identity()
    {
        static Matrix3<T> m( 1, 0, 0, 0, 1, 0, 0, 0, 1 );
        return m;
    }

    using Matrix3F = Matrix3<f32>;
    using Matrix3D = Matrix3<f64>;

}  // end namespace fb

#endif
