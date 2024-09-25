#include "vectors.h"

#include <stdlib.h>
#include <math.h>

Vector vector_new(double x, double y, double z) {
    Vector v = malloc(sizeof(tVector));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

Vector vector_new_empty() {
    return vector_new(0, 0, 0);
}

void vector_free(void *vector) {
    Vector v = (Vector)vector;
    free(v);
}

void vector_normalize(Vector vector) {
    double norm = vector_norm(vector);
    vector->x /= norm;
    vector->y /= norm;
    vector->z /= norm;
}

double vector_norm(Vector vector) {
    return sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

Vector vector_scalar_multiply(Vector vector, double magnitude) {
    Vector result = vector_new_empty();
    result->x = vector->x * magnitude;
    result->y = vector->y * magnitude;
    result->z = vector->z * magnitude;
    return result;
}

Vector vector_scalar_divide(Vector vector, double value) {
    Vector result = vector_new_empty();
    result->x = vector->x / value;
    result->y = vector->y / value;
    result->z = vector->z / value;
    return result;
}

void vector_copy(Vector destination, Vector source) {
    destination->x = source->x;
    destination->y = source->y;
    destination->z = source->z;
}

double vector_distance(Vector v1, Vector v2) {
    double dx = v1->x - v2->x;
    double dy = v1->y - v2->y;
    double dz = v1->z - v2->z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

Vector vector_subtract(Vector v1, Vector v2) {
    Vector result = vector_new_empty();
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    return result;
}

Vector vector_add(Vector v1, Vector v2) {
    Vector result = vector_new_empty();
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    return result;
}
