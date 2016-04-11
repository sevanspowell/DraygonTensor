#pragma once

#include "Vector4.h"

namespace bb_math
{
class Matrix4
{
public:
    /**
     * Default constructor.
     *
     * Creates a 4x4 identity matrix.
     */
    Matrix4();
    /**
     * Construct a matrix with the given leading value.
     *
     * Constructs a 4x4 matrix filled with zeroes, other than the leading
     * diagonal, which is filled with the given parameter.
     *
     * @param  leading  scalar, value of leading diagonal numbers in matrix.
     */
    Matrix4(scalar leading);
    /**
     * Copy constructor.
     *
     * Creates a Matrix4 from another Matrix4.
     *
     * @param  other  const Matrix &, matrix to clone.
     */
    Matrix4(const Matrix4 &other);
    /**
     * Construct a matrix from four column vector.
     *
     * @param  col0  const Vector4 &, column 0 of the matrix.
     * @param  col1  const Vector4 &, column 1 of the matrix.
     * @param  col2  const Vector4 &, column 2 of the matrix.
     * @param  col3  const Vector4 &, column 3 of the matrix.
     */
    Matrix4(const Vector4 &col0,
            const Vector4 &col1,
            const Vector4 &col2,
            const Vector4 &col3);
    /**
     * Construct a matrix from the given elements.
     *
     * Note that element ordering is (row, col), so:
     *  el00 is the first column, first row of the matrix and
     *  el10 is the first column, second row of the matrix.
     *
     * @param  el00  scalar, first column, first row element.
     * @param  el01  scalar, second column, first row element.
     * @param  el02  scalar, third column, first row element.
     * @param  el03  scalar, fourth column, first row element.
     * @param  el10  scalar, first column, second row element.
     * @param  el11  scalar, second column, second row element.
     * @param  el12  scalar, third column, second row element.
     * @param  el13  scalar, fourth column, second row element.
     * @param  el20  scalar, first column, third row element.
     * @param  el21  scalar, second column, third row element.
     * @param  el22  scalar, third column, third row element.
     * @param  el23  scalar, fourth column, third row element.
     * @param  el30  scalar, first column, fourth row element.
     * @param  el31  scalar, second column, fourth row element.
     * @param  el32  scalar, third column, fourth row element.
     * @param  el33  scalar, fourth column, fourth row element.
     */
    Matrix4(scalar el00,
            scalar el01,
            scalar el02,
            scalar el03,
            scalar el10,
            scalar el11,
            scalar el12,
            scalar el13,
            scalar el20,
            scalar el21,
            scalar el22,
            scalar el23,
            scalar el30,
            scalar el31,
            scalar el32,
            scalar el33);

    /**
     * Copy assignment operator.
     *
     * Assign value of another Matrix4 to this Matrix4.
     *
     * @param   other  const Matrix4 &, Matrix4 to assign to this Matrix4.
     * @return         const Matrix4 &, resulting value of expression (i.e. the
     *  other Matrix4).
     */
    const Matrix4 &operator=(const Matrix4 &other);
    /**
     * Indexed column access operator.
     *
     * [0] = first column.
     * [1] = second column.
     * [2] = third column.
     * [3] = fourth column.
     *
     * @pre  0 <= index < 4.
     *
     * @param   index  unsigned int, index of column to access.
     * @return         const Vector4 &, column vector at the given instance.
     */
    Vector4 &operator[](unsigned int index);
    /**
     * Const indexed column access operator.
     *
     * [0] = first column.
     * [1] = second column.
     * [2] = third column.
     * [3] = fourth column.
     *
     * @pre  0 <= index < 4.
     *
     * @param   index  unsigned int, index of column to access.
     * @return         const Vector4 &, column vector at the given instance.
     */
    const Vector4 &operator[](unsigned int index) const;
    /**
     * Combo scalar multiplication-assignment operator.
     *
     * Multiply all components of the matrix by a factor.
     *
     * @param   factor  scalar, factor to multiply components of vector by.
     * @return          const Matrix4 &, resulting scaled vector.
     */
    const Matrix4 &operator*=(scalar factor);
    /**
     * Combo addition-assignment operator.
     *
     * Assign value of (other Matrix4 + this Matrix4) (component-wise addition)
     * to this Matrix4.
     *
     * @param   other  const Matrix4 &, Matrix4 to sum with this Matrix4.
     * @return         const Matrix4 &, resulting value of operation
     *  (i.e. this + other).
     */
    const Matrix4 &operator+=(const Matrix4 &other);
    /**
     * Combo subtraction-assignment operator.
     *
     * Assign value of (other Matrix4 + this Matrix4) (component-wise
     * subtraction)
     * to this Matrix4.
     *
     * @param   other  const Matrix4 &, Matrix4 to sum with this Matrix4.
     * @return         const Matrix4 &, resulting value of operation
     *  (i.e. this - other).
     */
    const Matrix4 &operator-=(const Matrix4 &other);
    /**
     * Equivalence operator.
     *
     * Compare two Matrix4s and return TRUE if they are equal or FALSE
     * otherwise. Two matrices are only equal if all their elements are equal.
     *
     * @param  other  const Matrix4 &, Matrix4 to compare to this Matrix4.
     * @return        bool, TRUE if two Matrix4s are equal, FALSE otherwise.
     */
    bool operator==(const Matrix4 &other) const;
    /**
     * Inequivalence operator.
     *
     * Compare two Matrix4s and return TRUE if they are not equal or FALSE
     * otherwise.
     *
     * @param  other  const Matrix4 &, Matrix4 to compare to this Matrix4.
     * @return        bool, TRUE if two Matrix4s are not equal, FALSE otherwise.
     */
    bool operator!=(const Matrix4 &other) const;

    /**
     * Transpose the given matrix (swap rows and columns).
     *
     * @param   mat  const Matrix4 &, Matrix4 to transpose.
     * @return       Matrix4, transposed matrix (row and column positions
     *  swapped).
     */
    static Matrix4 Transpose(const Matrix4 &mat);
    /**
     * Return the inverse of the given matrix.
     *
     * @param   mat  const Matrix4 &, matrix to find the inverse of.
     * @return       Matrix4, matrix inverse.
     */
    static Matrix4 Inverse(const Matrix4 &mat);

    /**
     * Create an orthogonal projection matrix.
     *
     * @param   width              scalar, width of the viewing volume.
     * @param   height             scalar, height of the viewing volume.
     * @param   nearPlaneDistance  scalar, distance to near clipping plane of
     *  viewing volume.
     * @param   farPlaneDistance   scalar, distance to far clipping plane of
     *  viewing volume.
     * @return                     Matrix4, orthogonal projection matrix.
     */
    static Matrix4 CreateOrthographic(scalar width,
                                      scalar height,
                                      scalar nearPlaneDistance,
                                      scalar farPlaneDistance);
    /**
     * Create a perspective projection matrix.
     *
     * @param   fieldOfView  scalar, vertical field of view of the viewing
     *  volume in radians.
     * @param   aspectRatio  scalar, aspect ratio of the viewing volume (width /
     * height).
     * @param   nearPlaneDistance  scalar, distance to near clipping plane of
     *  viewing volume.
     * @param   farPlaneDistance   scalar, distance to far clipping plane of
     *  viewing volume.
     * @return                     Matrix4, perspective projection matrix.
     */
    static Matrix4 CreatePerspectiveFieldOfView(scalar fieldOfView,
                                                scalar aspectRatio,
                                                scalar nearPlaneDistance,
                                                scalar farPlaneDistance);
    /**
     * Create a translation matrix.
     *
     * @param   translation  const Vector3 &, translation amount.
     * @return  Matrix4, translation matrix.
     */
    static Matrix4 CreateTranslationMatrix(const Vector3 &translation);
    /**
     * Create a translation matrix.
     *
     * @param   x  scalar, x translation amount.
     * @param   y  scalar, y translation amount.
     * @param   z  scalar, z translation amount.
     * @return     Matrix4, translation matrix.
     */
    static Matrix4 CreateTranslationMatrix(scalar x, scalar y, scalar z);
    /**
     * Create a scaling matrix.
     *
     * @param   scale  const Vector3 &, scale amount.
     * @return         Matrix4, scale matrix.
     */
    static Matrix4 CreateScaleMatrix(const Vector3 &scale);
    /**
     * Create a scaling matrix.
     *
     * @param   x  scalar, x scale amount.
     * @param   y  scalar, y scale amount.
     * @param   z  scalar, z scale amount.
     * @return     Matrix4, scale matrix.
     */
    static Matrix4 CreateScaleMatrix(float x, float y, float z);

    Vector4 data[4];
};

// Binary operators
/**
 * Binary addition operator.
 *
 * Perform component-wise addition on two matrices.
 *
 * @param   m1  const Matrix4 &, Matrix4 to sum.
 * @param   m2  const Matrix4 &, Matrix4 to sum.
 * @return      Matrix4, component-wise sum of m1 and m2.
 */
Matrix4 operator+(const Matrix4 &m1, const Matrix4 &m2);
/**
 * Binary subtraction operator.
 *
 * Perform component-wise subtraction on two matrices.
 *
 * @param   m1  const Matrix4 &, Matrix4 to sum.
 * @param   m2  const Matrix4 &, Matrix4 to sum.
 * @return      Matrix4, component-wise subtraction of m1 and m2.
 */
Matrix4 operator-(const Matrix4 &m1, const Matrix4 &m2);
/**
 * Matrix multiplication.
 *
 * Multiply two matrices.
 *
 * @param   m1  const Matrix4 &, Matrix4 to multiply.
 * @param   m2  const Matrix4 &, Matrix4 to multiply.
 * @return      Matrix4, result of matrix multiplication between m1 and m2.
 */
Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2);
/**
 * Matrix-vector multiplication.
 *
 * Multiply the column vector by the given matrix.
 *
 * @param   mat     const Matrix4 &, matrix to multiply by.
 * @param   column  const Vector4 &, column vector to multiply.
 * @return          Vector4, resulting column vector.
 */
Vector4 operator*(const Matrix4 &mat, const Vector4 &column);
/**
 * Matrix-vector multiplication.
 *
 * Multiply the matrix by the given row vector.
 *
 * @param   row  const Vector4 &, row vector to multiply by.
 * @param   mat  const Matrix4 &, matrix to multiply.
 * @return       Vector4, resulting row vector.
 */
Vector4 operator*(const Vector4 &row, const Matrix4 &mat);
/**
 * Multiply each element in the given matrix by a given factor.
 *
 * @param   mat     const Matrix4 &, matrix to peform multiplication on.
 * @param   factor  scalar, factor to multiply each element of the matrix with.
 * @return          Matrix4, each element of the input matrix scaled by the
 * given matrix.
 */
Matrix4 operator*(const Matrix4 &mat, scalar factor);
/**
 * Multiply each element in the given matrix by a given factor.
 *
 * @param   factor  scalar, factor to multiply each element of the matrix with.
 * @param   mat     const Matrix4 &, matrix to peform multiplication on.
 * @return          Matrix4, each element of the input matrix scaled by the
 * given matrix.
 */
Matrix4 operator*(scalar factor, const Matrix4 &mat);

/**
 * Output stream operator.
 *
 * Ouput matrix contents to a stream.
 *
 * @param   out  std::ostream &, output stream to output to.
 * @param   vec  const Matrix4 &, vector to output.
 * @return       std::ostream &, output stream outputted to.
 */
std::ostream &operator<<(std::ostream &out, const Matrix4 &mat);
}
