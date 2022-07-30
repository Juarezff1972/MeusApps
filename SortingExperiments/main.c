#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>


#include "array_of_doubles.h"
#include "sorting_algorithms.h"


static const clock_t clock_precision = 1000000U; // clocks (tics)
static const double threshold_repetition_time = 300.0; // seconds
static const long maximum_number_of_repetions = 1001L;
static const double threshold_time_per_sort = 300.0; // seconds
static const long maximum_file_size = 1L << 24;

const char *const file_types[] =
{
    "sorted",
    "partially_sorted",
    "shuffled"
};
const int number_of_file_types = sizeof(file_types) / sizeof(file_types[0]);

struct algorithm_statistics
{
    struct algorithm_counts counts;
    long accumulated_runs;
    long repetitions;
    struct double_statistics times;
};

static const struct algorithm_statistics initial_statistics =
{
    .counts = {
        .comparisons = 0,
        .swaps = 0,
        .copies = 0
    },
    .accumulated_runs = 0,
    .repetitions = 0,
    .times = {
        .average = NAN,
        .stddev = NAN,
        .median = NAN,
        .minimum = INFINITY,
        .maximum = -INFINITY
    }
};


static bool valid_file_type(const char* file_type)
{
    assert(file_type != NULL);
    int i = 0;
    while(i != number_of_file_types &&
            strcmp(file_type, file_types[i]) != 0)
        i++;
    return i != number_of_file_types;
}


static double copy_time_estimate(const long length, double work_items[length],
                                 const double items[length])
{
    assert(length > 0L);
    assert(work_items != NULL);
    assert(items != NULL);
    printf("\tEstimating copy time...\n");
    long runs = 0L;
    clock_t start = clock();
    do
    {
        copy_double_array(length, work_items, items);
        runs++;
    }
    while(clock() - start < clock_precision);
    start = clock();
    for(long i = 0L; i != runs; i++)
        copy_double_array(length, work_items, items);
    double copy_time = (double)(clock() - start) / CLOCKS_PER_SEC / runs;
    printf("\tCopy time estimated to be %g seconds (based on %ld runs).\n",
           copy_time, runs);
    return copy_time;
}


static double sort_time_estimate(struct sorting_algorithm algorithm,
                                 const long length, double work_items[length],
                                 const double items[length], const long runs,
                                 const double copy_time)
{
    assert(length > 0L);
    assert(work_items != NULL);
    assert(items != NULL);
    assert(runs > 0L);
    assert(copy_time > 0.0);
    clock_t start = clock();
    for(long i = 0; i != runs; i++)
    {
        copy_double_array(length, work_items, items);
        if(algorithm.sort(length, work_items))
        {
            fprintf(stderr, "Error: could not run sorting "
                    "algorithm '%s'.\n", algorithm.name);
            return -1.0;
        }
    }
    return (double)(clock() - start) / CLOCKS_PER_SEC / runs - copy_time;
}

static long number_of_runs(const struct sorting_algorithm algorithm,
                           const long length, double work_items[length],
                           const double items[length], const double copy_time)
{
    assert(length > 0L);
    assert(work_items != NULL);
    assert(items != NULL);
    assert(copy_time > 0.0);
    long runs = 0L;
    clock_t start = clock();
    do
    {
        copy_double_array(length, work_items, items);
        if(algorithm.sort(length, work_items))
        {
            fprintf(stderr, "Error: could not run sorting "
                    "algorithm '%s'.\n", algorithm.name);
            return -1;
        }
        runs++;
    }
    while(clock() - start - runs * copy_time * CLOCKS_PER_SEC <
            clock_precision);
    return runs;
}

static bool experiment_algorithm(const struct sorting_algorithm algorithm,
                                 const long length, double work_items[length],
                                 const double items[length],
                                 const double sorted_items[length],
                                 const double copy_time,
                                 struct algorithm_statistics *statistics)
{
    assert(length > 0L);
    assert(work_items != NULL);
    assert(items != NULL);
    assert(sorted_items != NULL);
    assert(copy_time > 0.0);
    assert(statistics != NULL);
    copy_double_array(length, work_items, items);
    printf("\t\tRunning counting algorithm version.\n");
    if(algorithm.sort_and_count(length, work_items, &statistics->counts))
    {
        fprintf(stderr, "Error: could not run sorting algorithm '%s' "
                "(counting version).\n", algorithm.name);
        return true;
    }
    printf("\t\tChecking correctness of counting algorithm version.\n");
    if(!double_arrays_equal(length, sorted_items, work_items))
    {
        fprintf(stderr, "Error: sorting algorithm '%s' (counting "
                "version) did not sort.\n", algorithm.name);
        return true;
    }
    copy_double_array(length, work_items, items);
    printf("\t\tRunning non-counting algorithm version.\n");
    if(algorithm.sort(length, work_items))
    {
        fprintf(stderr, "Error: could not run sorting algorithm '%s'.\n",
                algorithm.name);
        return true;
    }
    printf("\t\tChecking correctness of non-counting algorithm version.\n");
    if(!double_arrays_equal(length, sorted_items, work_items))
    {
        fprintf(stderr, "Error: sorting algorithm '%s' did not sort.\n",
                algorithm.name);
        return true;
    }
    printf("\t\tTime measurements:\n");
    long runs =
        number_of_runs(algorithm, length, work_items, items, copy_time);
    if(runs < 0L)
        return true;
    printf("\t\t\tEach time will be estimated using %ld runs.\n", runs);
    statistics->accumulated_runs = runs;
    double times[maximum_number_of_repetions];
    printf("\t\t\tStarting repeated estimations:\n");
    long repetitions = 0L;
    clock_t start = clock();
    double accumulated_time;
    do
    {
        times[repetitions] =
            sort_time_estimate(algorithm, length, work_items,
                               items, runs, copy_time);
        if(times[repetitions] < 0.0)
            return true;
        repetitions++;
        accumulated_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    while(repetitions != maximum_number_of_repetions &&
            accumulated_time < threshold_repetition_time);
    printf("\t\t\t%ld repetitions in %g seconds.\n", repetitions,
           accumulated_time);
    statistics->repetitions = repetitions;
    statistics->times = double_array_statistics(repetitions, times);
    return false;
}


static void write_statistics_headers(FILE *const output,
                                     const char *algorithm_name)
{
    assert(output != NULL);
    assert(algorithm_name != NULL);
    fprintf(output, ";Comparisons (%s)", algorithm_name);
    fprintf(output, ";Swaps (%s)", algorithm_name);
    fprintf(output, ";Copies (%s)", algorithm_name);
    fprintf(output, ";Accumulated runs (%s)", algorithm_name);
    fprintf(output, ";Repetitions (%s)", algorithm_name);
    fprintf(output, ";Time Average [seconds] (%s)", algorithm_name);
    fprintf(output, ";Time Stddev [seconds] (%s)", algorithm_name);
    fprintf(output, ";Time Median [seconds] (%s)", algorithm_name);
    fprintf(output, ";Time Minimum [seconds] (%s)", algorithm_name);
    fprintf(output, ";Time Maximum [seconds] (%s)", algorithm_name);
}


static void write_statistics(FILE *const output,
                             const struct algorithm_statistics statistics)
{
    assert(output != NULL);
    fprintf(output, ";%ld", statistics.counts.comparisons);
    fprintf(output, ";%ld", statistics.counts.swaps);
    fprintf(output, ";%ld", statistics.counts.copies);
    fprintf(output, ";%ld", statistics.accumulated_runs);
    fprintf(output, ";%ld", statistics.repetitions);
    fprintf(output, ";%g", statistics.times.average);
    fprintf(output, ";%g", statistics.times.stddev);
    fprintf(output, ";%g", statistics.times.median);
    fprintf(output, ";%g", statistics.times.minimum);
    fprintf(output, ";%g", statistics.times.maximum);
}


bool experiment_size(FILE *const output, const char *const path,
                     const char *const file_type, const long size,
                     bool excessive_time_per_sort[number_of_sorting_algorithms])
{
    char file_name[FILENAME_MAX];
    snprintf(file_name, FILENAME_MAX, "%s%s_%ld.txt", path, file_type, size);
    char sorted_file_name[FILENAME_MAX];
    snprintf(sorted_file_name, FILENAME_MAX, "%ssorted_%ld.txt", path, size);
    double *items = NULL;
    double *sorted_items = NULL;
    double *work_items = NULL;
    long length;
    items = read_double_array_from(file_name, &length);
    bool error = items == NULL || length != size;
    if(error)
    {
        fprintf(stderr, "Error: Reading file '%s'.\n", file_name);
        goto terminate;
    }
    sorted_items = read_double_array_from(sorted_file_name, &length);
    error = sorted_items == NULL || length != size;
    if(error)
    {
        fprintf(stderr, "Error: Reading file '%s'.\n", sorted_file_name);
        goto terminate;
    }
    work_items = new_double_array_of(size);
    error = work_items == NULL;
    if(error)
    {
        fprintf(stderr, "Error: Allocating work array.\n");
        goto terminate;
    }
    double copy_time = copy_time_estimate(length, work_items, items);
    fprintf(output, "%ld", size);
    for(int a = 0; a != number_of_sorting_algorithms; a++)
    {
        struct algorithm_statistics statistics = initial_statistics;
        printf("\tStarting experiments for %s:\n",
               sorting_algorithms[a].name);
        if(!excessive_time_per_sort[a])
        {
            printf("\t\tPerforming experiments.\n");
            error = experiment_algorithm(sorting_algorithms[a],
                                         size, work_items, items,
                                         sorted_items, copy_time,
                                         &statistics);
            if(error)
                goto terminate;
        }
        else
            printf("\t\tSkipping experiments.\n");
        printf("\t\tWriting results, if any.\n");
        write_statistics(output, statistics);
        printf("\t\tEnding experiments for %s (median time = %g s).\n",
               sorting_algorithms[a].name, statistics.times.median);
        if(statistics.times.median > threshold_time_per_sort)
            excessive_time_per_sort[a] = true;
    }
    fputc('\n', output);
terminate:
    free(work_items);
    free(sorted_items);
    free(items);
    return error;
}



static bool experiment_all(const char *const path,
                           const char *const file_type,
                           const char *const statistics_file_name)
{
    assert(path != NULL);
    assert(file_type != NULL);
    assert(statistics_file_name != NULL);
    bool excessive_time_per_sort[number_of_sorting_algorithms];
    for(int a = 0; a != number_of_sorting_algorithms; a++)
        excessive_time_per_sort[a] = false;
    FILE *const output = fopen(statistics_file_name, "w");
    bool error = output == NULL;
    if(error)
    {
        fprintf(stderr, "Error: Could not open '%s' for writing!\n",
                statistics_file_name);
        goto terminate;
    }
    fprintf(output, "Size");
    for(int a = 0; a != number_of_sorting_algorithms; a++)
        write_statistics_headers(output, sorting_algorithms[a].name);
    fputc('\n', output);
    printf("Wrote headers to results.\n");
    for(long size = 1L << 1; size != maximum_file_size << 1; size <<= 1)
    {
        printf("Starting experiments for size %ld:\n", size);
        error = experiment_size(output, path, file_type, size,
                                excessive_time_per_sort);
        if(error)
            goto terminate;
        fflush(output);
        printf("\tEnded experiments for size %ld.\n", size);
    }
terminate:
    if(output != NULL)
        fclose(output);
    return error;
}


int main(const int argument_count,
         const char *const argument_values[argument_count])
{
    if(argument_count < 4)
    {
        fprintf(stderr, "Error: Insuficient number of arguments!\n");
        return EXIT_FAILURE;
    }
    const char *const path = argument_values[1];
    const char *const file_type = argument_values[2];
    const char *const statistics_file_name = argument_values[3];
    if(!valid_file_type(file_type))
    {
        fprintf(stderr, "Error: Unknown file type '%s'!\n", file_type);
        return EXIT_FAILURE;
    }
    printf("Starting experiments from %s files in %s. Storing results "
           "in %s.\n", file_type, path, statistics_file_name);
    if(experiment_all(path, file_type, statistics_file_name))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
