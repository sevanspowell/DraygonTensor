#pragma once

#include "Precision.h"
#include "Vector3.h"

namespace ds_math
{
/**
 * @author Samuel Evans-Powell
 */
class Matrix3
{
public:
    /**
     * Default constructor.
     *
     * Creates a 3x3 identity matrix.
     */
    Matrix3();
    /**
     * Construct a matrix with the given leading value.
     *
     * Constructs a 3x3 matrix filled with zeroes, other than the leading
     * diagonal, which is filled with the given parameter.
     *
     * @param  leading  scalar, value of leading diagonal numbers in matrix.
     */
    Matrix3(scalar leading);
    /**
     * Copy constructor.
     *
     * Creates a Matrix3 from another Matrix3.
     *
     * @param  other  const Matrix3 &, matrix to clone.
     */
    Matrix3(const Matrix3 &other);
    /**
     * Construct a matrix from three column vectors.
     *
     * @param  col0  const Vector3 &, column 0 of the matrix.
     * @param  col1  const Vector3 &, column 1 of the matrix.
     * @param  col2  const Vector3 &, column 2 of the matrix.
     */
    Matrix3(const Vector3 &col0, const Vector3 &col1, const Vector3 &col2);
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
     * @param  el10  scalar, first column, second row element.
     * @param  el11  scalar, second column, second row element.
     * @param  el12  scalar, third column, second row element.
     * @param  el20  scalar, first column, third row element.
     * @param  el21  scalar, second column, third row element.
     * @param  el22  scalar, third column, third row element.
     */
    Matrix3(scalar el00,
            scalar el01,
            scalar el02,
            scalar el10,
            scalar el11,
            scalar el12,
            scalar el20,
            scalar el21,
            scalar el22);

    /**
     * Copy assignment operator.
     *
     * Assign value of another Matrix3 to this Matrix3.
     *
     * @param   other  const Matrix3 &, Matrix3 to assign to this Matrix3.
     * @return         const Matrix3 &, resulting value of expression (i.e. the
     *  other Matrix3).
     */
    const Matrix3 &operator=(const Matrix3 &other);
    /**
     * Indexed column access operator.
     *
     * [0] = first column.
     * [1] = second column.
     * [2] = third column.
     *
     * @pre  0 <= index < 3.
     *
     * @param   index  unsigned int, index of column to access.
     * @return         const Vector3 &, column vector at the given instance.
     */
    Vector3 &operator[](unsigned int index);
    /**
     * Const indexed column access operator.
     *
     * [0] = first column.
     * [1] = second column.
     * [2] = third column.
     *
     * @pre  0 <= index < 3.
     *
     * @param   index  unsigned int, index of column to access.
     * @return         const Vector3 &, column vector at the given instance.
     */
    const Vector3 &operator[](unsigned int index) const;
    /**
     * Combo scalar multiplication-assignment operator.
     *
     * Multiply all components of the matrix by a factor.
     *
     * @param   factor  scalar, factor to multiply components of vector by.
     * @return          const Matrix3 &, resulting scaled vector.
     */
    const Matrix3 &operator*=(scalar factor);
    /**
     * Combo addition-assignment operator.
     *
     * Assign value of (other Matrix3 + this Matrix3) (component-wise addition)
     * to this Matrix3.
     *
     * @param   other  const Matrix3 &, Matrix3 to sum with this Matrix3.
     * @return         const Matrix3 &, resulting value of operation
     *  (i.e. this + other).
     */
    const Matrix3 &operator+=(const Matrix3 &other);
    /**
     * Combo subtraction-assignment operator.
     *
     * Assign value of (other Matrix3 + this Matrix3) (component-wise
     * subtraction) to this Matrix3.
     *
     * @param   other  const Matrix3 &, Matrix3 to sum with this Matrix3.
     * @return         const Matrix3 &, resulting value of operation
     *  (i.e. this - other).
     */
    const Matrix3 &operator-=(const Matrix3 &other);
    /**
     * Equivalence operator.
     *
     * Compare two Matrix3s and return TRUE if they are equal or FALSE
     * otherwise. Two matrices are only equal if all their elements are equal.
     *
     * @param  other  const Matrix3 &, Matrix3 to compare to this Matrix3.
     * @return        bool, TRUE if two Matrix3s are equal, FALSE otherwise.
     */
    bool operator==(const Matrix3 &other) const;
    /**
     * Inequivalence operator.
     *
     * Compare two Matrix3s and return TRUE if they are not equal or FALSE
     * otherwise.
     *
     * @param  other  const Matrix3 &, Matrix3 to compare to this Matrix3.
     * @return        bool, TRUE if two Matrix3s are not equal, FALSE otherwise.
     */
    bool operator!=(const Matrix3 &other) const;

    /**
     * Transpose the given matrix (swap rows and columns).
     *
     * @param   mat  const Matrix3 &, Matrix3 to transpose.
     * @return       Matrix3, transposed matrix (row and column positions
     *  swapped).
     */
    static Matrix3 Transpose(const Matrix3 &mat);
    /**
     * Return the inverse of the given matrix.
     *
     * @param   mat  const Matrix3 &, matrix to find the inverse of.
     * @return       Matrix3, matrix inverse.
     */
    static Matrix3 Inverse(const Matrix3 &mat);
    /**
     * Transform the given vector by the given matrix.
     *
     * @param  mat  const Matrix3 &, matrix to transform vector by.
     * @param  vec  const Vector3 &, vector to transform.
     * @return      Vector3, transformed vector.
     */
    static Vector3 Transform(const Matrix3 &mat, const Vector3 &vec);
    /**
     * Transform the given vector by the inverse of the given matrix.
     *
     * @param  mat  const Matrix3 &, matrix to invert and transform vector by.
     * @param  vec  const Vector3 &, vector to transform.
     * @return      Vector3, transformed vector.
     */
    static Vector3 TransformInverse(const Matrix3 &mat, const Vector3 &vec);

    Vector3 data[3];
};

// Binary operators
/**
 * Binary addition operator.
 *
 * Perform component-wise addition on two matrices.
 *
 * @param   m1  const Matrix3 &, Matrix3 to sum.
 * @param   m2  const Matrix3 &, Matrix3 to sum.
 * @return      Matrix3, component-wise sum of m1 and m2.
 */
Matrix3 operator+(const Matrix3 &m1, const Matrix3 &m2);
/**
 * Binary subtraction operator.
 *
 * Perform component-wise subtraction on two matrices.
 *
 * @param   m1  const Matrix3 &, Matrix3 to sum.
 * @param   m2  const Matrix3 &, Matrix3 to sum.
 * @return      Matrix3, component-wise subtraction of m1 and m2.
 */
Matrix3 operator-(const Matrix3 &m1, const Matrix3 &m2);
/**
 * Matrix multiplication.
 *
 * Multiply two matrices.
 *
 * @param   m1  const Matrix3 &, Matrix3 to multiply.
 * @param   m2  const Matrix3 &, Matrix3 to multiply.
 * @return      Matrix3, result of matrix multiplication between m1 and m2.
 */
Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2);
/**
 * Matrix-vector multiplication.
 *
 * Multiply the column vector by the given matrix.
 *
 * @param   mat     const Matrix3 &, matrix to multiply by.
 * @param   column  const Vector3 &, column vector to multiply.
 * @return          Vector3, resulting column vector.
 */
Vector3 operator*(const Matrix3 &mat, const Vector3 &column);
/**
 * Matrix-vector multiplication.
 *
 * Multiply the matrix by the given row vector.
 *
 * @param   row  const Vector3 &, row vector to multiply by.
 * @param   mat  const Matrix3 &, matrix to multiply.
 * @return       Vector3, resulting row vector.
 */
Vector3 operator*(const Vector3 &row, const Matrix3 &mat);
/**
 * Multiply each element in the given matrix by a given factor.
 *
 * @param   mat     const Matrix3 &, matrix to peform multiplication on.
 * @param   factor  scalar, factor to multiply each element of the matrix with.
 * @return          Matrix3, each element of the input matrix scaled by the
 * given matrix.
 */
Matrix3 operator*(const Matrix3 &mat, scalar factor);
/**
 * Multiply each element in the given matrix by a given factor.
 *
 * @param   factor  scalar, factor to multiply each element of the matrix with.
 * @param   mat     const Matrix3 &, matrix to peform multiplication on.
 * @return          Matrix3, each element of the input matrix scaled by the
 * given matrix.
 */
Matrix3 operator*(scalar factor, const Matrix3 &mat);

/**
 * Output stream operator.
 *
 * Ouput matrix contents to a stream.
 *
 * @param   out  std::ostream &, output stream to output to.
 * @param   vec  const Matrix3 &, vector to output.
 * @return       std::ostream &, output stream outputed to.
 */
std::ostream &operator<<(std::ostream &out, const Matrix3 &mat);
}
