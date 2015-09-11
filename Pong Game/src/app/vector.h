/** @file   vector.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   22 September 2012
 *  @brief  Vector class
 */


#ifndef VECTOR_H
#define VECTOR_H


#include "system.h"


/** @brief A generic signed vector type. */
typedef struct {
    int8_t x;
    int8_t y;
} Vector;


/** @brief Vector constructor */
Vector vector_create(int8_t x, int8_t y);


/** @brief Test two vectors for equality */
bool vector_equal(Vector lhs, Vector rhs);


/** @brief Add two vectors. */
Vector vector_add(Vector lhs, Vector rhs);


#endif /* VECTOR_H */
