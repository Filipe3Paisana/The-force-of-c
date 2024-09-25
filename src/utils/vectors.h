#ifndef VECTORS_H
#define VECTORS_H

typedef struct {
    double x;
    double y;
    double z;
} tVector, *Vector;

Vector vector_new(double x, double y, double z);

Vector vector_new_empty();

void vector_free(void *vector);

void vector_normalize(Vector vector);

double vector_norm(Vector vector);

Vector vector_scalar_multiply(Vector vector, double value);

Vector vector_scalar_divide(Vector vector, double value);

void vector_copy(Vector destination, Vector source);

double vector_distance(Vector v1, Vector v2);

Vector vector_subtract(Vector v1, Vector v2);

Vector vector_add(Vector v1, Vector v2);

//Vector vector_null(Vector vector);

#endif