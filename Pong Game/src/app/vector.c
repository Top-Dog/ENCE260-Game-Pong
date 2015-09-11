/** @file   vector.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   22 September 2012
 */


#include "vector.h"


Vector vector_create(int8_t x, int8_t y) {
    Vector vec = {x, y};
    return vec;
}


bool vector_equal(Vector lhs, Vector rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}


Vector vector_add(Vector lhs, Vector rhs) {
    Vector sum = {lhs.x + rhs.x, lhs.y + rhs.y};
    return sum;
}
