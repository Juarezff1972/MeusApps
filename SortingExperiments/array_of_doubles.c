#include "array_of_doubles.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

double *new_double_array_of(const long length)
{
    assert(length >= 0L);
    return malloc(length * sizeof(double));
}

double *resize_double_array_to(double *const items, const long new_length)
{
    assert(new_length >= 0L);
    return realloc(items, new_length * sizeof(double));
}

double *read_double_array_from(const char *const file_name, long *const length)
{
    assert(file_name != NULL);
    assert(length != NULL);
    FILE *file = fopen(file_name, "r");
    if(file == NULL)
        return NULL;
    long capacity = 32L;
    double *items = new_double_array_of(capacity);
    if(items == NULL)
    {
        fclose(file);
        return NULL;
    }
    long i = 0L;
    double item;
    while(fscanf(file, "%lg", &item) == 1)
    {
        if(i == capacity)
        {
            double *new_items =
                resize_double_array_to(items, capacity *= 2L);
            if(new_items == NULL)
            {
                fclose(file);
                free(items);
                return NULL;
            }
            items = new_items;
        }
        items[i++] = item;
    }
    *length = i;
    if(*length != capacity)
        items = resize_double_array_to(items, *length);
    fclose(file);
    return items;
}

void copy_double_array(const long length,
                       double copy[length], const double original[length])
{
    assert(length >= 0L);
    assert(length == 0L || copy != NULL);
    assert(length == 0L || original != NULL);
    for(long i = 0L; i != length; i++)
        copy[i] = original[i];
}

bool double_arrays_equal(const long length,
                         const double first[length], const double second[length])
{
    assert(length >= 0L);
    assert(length == 0L || first != NULL);
    assert(length == 0L || second != NULL);
    long i = 0L;
    while(i != length && first[i] == second[i])
        i++;
    return i == length;
}

static int compare(const void *first_generic, const void *second_generic)
{
    const double *first = first_generic;
    const double *second = second_generic;
    if(first < second)
        return -1;
    else
        if(first > second)
            return 1;
        else
            return 0;
}

double double_array_average(const long length, const double items[length])
{
    assert(length >= 0L);
    assert(length == 0L || items != NULL);
    double sum = 0.0;
    for(long i = 0L; i != length; i++)
        sum += items[i];
    return sum / length;
}

static double square_of(const double value)
{
    return value * value;
}

double double_array_stddev(const long length, const double items[length])
{
    assert(length >= 0L);
    assert(length == 0L || items != NULL);
    const double average = double_array_average(length, items);
    double sum = 0.0;
    for(long i = 0L; i != length; i++)
        sum += square_of(items[i] - average);
    return sqrt(sum / length);
}

double double_array_median(const long length, const double items[length])
{
    assert(length >= 0L);
    assert(length == 0L || items != NULL);
    if(length == 0L)
        return NAN;
    double work_items[length];
    copy_double_array(length, work_items, items);
    qsort(work_items, length, sizeof(double), compare);
    if(length % 2 == 0L)
        return (work_items[length / 2 - 1] + work_items[length / 2]) / 2;
    else
        return work_items[length / 2];
}

double double_array_minimum(const long length, const double items[length])
{
    assert(length >= 0L);
    assert(length == 0L || items != NULL);
    double minimum = INFINITY;
    for(long i = 0L; i != length; i++)
        if(items[i] < minimum)
            minimum = items[i];
    return minimum;
}

double double_array_maximum(const long length, const double items[length])
{
    assert(length > 0L);
    assert(length == 0L || items != NULL);
    double maximum = -INFINITY;
    for(long i = 0L; i != length; i++)
        if(items[i] > maximum)
            maximum = items[i];
    return maximum;
}

struct double_statistics double_array_statistics(const long length,
        const double items[length])
{
    assert(length >= 0L);
    assert(length == 0L || items != NULL);
    struct double_statistics statistics =
    {
        .average = NAN,
        .stddev = NAN,
        .median = NAN,
        .minimum = INFINITY,
        .maximum = -INFINITY
    };
    if(length == 0L)
        return statistics;
    double work_items[length];
    double sum = 0.0;
    double sum_of_squares = 0.0;
    for(long i = 0L; i != length; i++)
    {
        work_items[i] = items[i];
        sum += items[i];
        sum_of_squares += square_of(items[i]);
    }
    statistics.average = sum / length;
    statistics.stddev =
        sqrt(sum_of_squares / length - square_of(statistics.average));
    qsort(work_items, length, sizeof(double), compare);
    statistics.minimum = work_items[0];
    if(length % 2 == 0L)
        statistics.median =
            (work_items[length / 2 - 1] + work_items[length / 2]) / 2;
    else
        statistics.median = work_items[length / 2];
    statistics.maximum = work_items[length - 1];
    return statistics;
}

