
#ifndef ISLA_EDA_ARRAY_OF_DOUBLES_HH_INCLUDED
#define ISLA_EDA_ARRAY_OF_DOUBLES_HH_INCLUDED

#include <stdbool.h>

struct double_statistics
{
    double average;
    double stddev;
    double median;
    double minimum;
    double maximum;
};
double *new_double_array_of(long length);
double *resize_double_array_to(double *items, long new_length);
double *read_double_array_from(const char *file_name, long *length);

void copy_double_array(long length, double copy[length],
                       const double original[length]);

bool double_arrays_equal(long length, const double first[length],
                         const double second[length]);

double double_array_average(long length, const double items[length]);
double double_array_stddev(long length, const double items[length]);
double double_array_median(long length, const double items[length]);
double double_array_minimum(long length, const double items[length]);
double double_array_maximum(long length, const double items[length]);

struct double_statistics double_array_statistics(long length,
        const double items[length]);

#endif // ISLA_EDA_ARRAY_OF_DOUBLES_HH_INCLUDED
