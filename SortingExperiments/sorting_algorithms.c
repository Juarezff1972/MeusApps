
#include "sorting_algorithms.h"


#include <stdlib.h>
#include <assert.h>

#include "array_of_doubles.h"

const struct sorting_algorithm sorting_algorithms[] =
{
    {
        .name = "bubble sort",
        .sort = bubble_sort,
        .sort_and_count = bubble_sort_and_count
    },
    {
        .name = "selection sort",
        .sort = selection_sort,
        .sort_and_count = selection_sort_and_count
    },
    {
        .name = "insertion sort",
        .sort = insertion_sort,
        .sort_and_count = insertion_sort_and_count
    },
    {
        .name = "Shell sort",
        .sort = shell_sort,
        .sort_and_count = shell_sort_and_count
    },
    {
        .name = "quicksort",
        .sort = quicksort,
        .sort_and_count = quicksort_and_count
    },
    {
        .name = "merge sort",
        .sort = merge_sort,
        .sort_and_count = merge_sort_and_count
    },
};

const int number_of_sorting_algorithms =
    sizeof(sorting_algorithms) / sizeof(sorting_algorithms[0]);

static void swap(const int length, double items[length],
                 const int i, const int j)
{
    assert(0 <= i && i < length);
    assert(0 <= j && j < length);
    assert(items != NULL);
    const double original_item_i = items[i];
    items[i] = items[j];
    items[j] = original_item_i;
}

static void swap_and_count(const int length, double items[length],
                           const int i, const int j,
                           struct algorithm_counts* counts)
{
    assert(0 <= i && i < length);
    assert(0 <= j && j < length);
    assert(items != NULL);
    const double original_item_i = items[i];
    items[i] = items[j];
    items[j] = original_item_i;
    counts->swaps++;
    counts->copies += 3;
}

bool bubble_sort(const int length, double items[length])
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    if(length <= 1)
        return false;
    for(int unsorted = length; unsorted != 1; unsorted--)
        for(int i = 0; i != unsorted - 1; i++)
            if(items[i] > items[i + 1])
                swap(length, items, i, i + 1);
    return false;
}

bool bubble_sort_and_count(const int length, double items[length],
                           struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    if(length <= 1)
        return false;
    for(int unsorted = length; unsorted != 1; unsorted--)
        for(int i = 0; i != unsorted - 1; i++)
        {
            counts->comparisons++;
            if(items[i] > items[i + 1])
                swap_and_count(length, items, i, i + 1, counts);
        }
    return false;
}

bool selection_sort(const int length, double items[length])
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    if(length <= 1)
        return false;
    for(int sorted = 0; sorted != length - 1; sorted++)
    {
        int i_of_smallest = sorted;
        for(int i = sorted + 1; i != length; i++)
            if(items[i] < items[i_of_smallest])
                i_of_smallest = i;
        if(i_of_smallest != sorted)
            swap(length, items, sorted, i_of_smallest);
    }
    return false;
}

bool selection_sort_and_count(const int length, double items[length],
                              struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    if(length <= 1)
        return false;
    for(int sorted = 0;
            sorted != length - 1;
            sorted++)
    {
        int i_of_smallest = sorted;
        for(int i = sorted + 1; i != length; i++)
        {
            counts->comparisons++;
            if(items[i] < items[i_of_smallest])
                i_of_smallest = i;
        }
        if(i_of_smallest != sorted)
            swap_and_count(length, items,
                           sorted, i_of_smallest, counts);
    }
    return false;
}

bool insertion_sort(const int length, double items[length])
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    if(length <= 1)
        return false;
    for(int sorted = 1; sorted != length; sorted++)
    {
        const double item_to_insert = items[sorted];
        int i = sorted;
        while(i != 0 && item_to_insert < items[i - 1])
        {
            items[i] = items[i - 1];
            i--;
        }
        if(i != sorted)
            items[i] = item_to_insert;
    }
    return false;
}

bool insertion_sort_and_count(const int length, double items[length],
                              struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    if(length <= 1)
        return false;
    for(int sorted = 1; sorted != length; sorted++)
    {
        counts->copies++;
        const double item_to_insert = items[sorted];
        int i = sorted;
        while(i != 0 && item_to_insert < items[i - 1])
        {
            counts->comparisons++;
            counts->copies++;
            items[i] = items[i - 1];
            i--;
        }
        if(i != 0)
            counts->comparisons++;
        if(i != sorted)
        {
            counts->copies++;
            items[i] = item_to_insert;
        }
    }
    return false;
}

bool shell_sort(const int length, double items[length])
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    if(length <= 1)
        return false;
    int step = 1;
    while(step < length / 3)
        step = 3 * step + 1;
    while(step >= 1)
    {
        for(int i = step; i != length; i++)
        {
            const double item_to_insert = items[i];
            int j = i;
            while(j >= step && item_to_insert < items[j - step])
            {
                items[j] = items[j - step];
                j -= step;
            }
            if(j != i)
                items[j] = item_to_insert;
        }
        step /= 3;
    }
    return false;
}

bool shell_sort_and_count(const int length, double items[length],
                          struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    if(length <= 1)
        return false;
    int step = 1;
    while(step < length / 3)
        step = 3 * step + 1;
    while(step >= 1)
    {
        for(int i = step; i != length; i++)
        {
            counts->copies++;
            const double item_to_insert = items[i];
            int j = i;
            while(j >= step && item_to_insert < items[j - step])
            {
                counts->comparisons++;
                counts->copies++;
                items[j] = items[j - step];
                j -= step;
            }
            if(j >= step)
                counts->comparisons++;
            if(j != i)
            {
                counts->copies++;
                items[j] = item_to_insert;
            }
        }
        step /= 3;
    }
    return false;
}

static void quicksort_segment(const int length, double items[length],
                              const int first, const int last)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(0 <= first);
    assert(last < length);
    if(first >= last)
        return;
    if(items[first] > items[last])
        swap(length, items, first, last);
    const double pivot = items[first];
    int i = first;
    int j = last + 1;
    do
    {
        do
        {
            i++;
        }
        while(items[i] < pivot);
        do
        {
            j--;
        }
        while(pivot < items[j]);
        if(i < j)
            swap(length, items, i, j);
    }
    while(i < j);
    if(j != first)
    {
        swap(length, items, first, j);
        quicksort_segment(length, items, first, j - 1);
    }
    quicksort_segment(length, items, j + 1, last);
}

bool quicksort(const int length, double items[length])
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    quicksort_segment(length, items, 0, length - 1);
    return false;
}

static void quicksort_segment_and_count(const int length, double items[length],
                                        const int first, const int last,
                                        struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    assert(0 <= first);
    assert(last < length);
    if(first >= last)
        return;
    int i = first;
    int j = last + 1;
    counts->comparisons++;
    if(items[first] > items[last])
        swap_and_count(length, items, first, last, counts);
    const double pivot = items[first];
    do
    {
        do
        {
            i++;
            counts->comparisons++;
        }
        while(items[i] < pivot);
        do
        {
            j--;
            counts->comparisons++;
        }
        while(pivot < items[j]);
        if(i < j)
            swap_and_count(length, items, i, j, counts);
    }
    while(i < j);
    if(j != first)
    {
        swap_and_count(length, items, first, j, counts);
        quicksort_segment_and_count(length, items, first, j - 1, counts);
    }
    quicksort_segment_and_count(length, items, j + 1, last, counts);
}

bool quicksort_and_count(const int length, double items[length],
                         struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    quicksort_segment_and_count(length, items, 0, length - 1, counts);
    return false;
}

static void merge(const int length, double items[length],
                  double temporary[length],
                  const int left, const int middle, const int right)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || temporary != NULL);
    assert(0 <= left && left < length);
    assert(0 <= right && right < length);
    assert(left <= middle && middle < right);
    if(items[middle] <= items[middle + 1])
        return;
    int i = left;
    int j = middle + 1;
    int k = left;
    for(; i <= middle && j <= right; k++)
        if(items[i] <= items[j])
            temporary[k] = items[i++];
        else
            temporary[k] = items[j++];
    for(int m = k; i <= middle; i++, m++)
        items[m] = items[i];
    for(int i = left; i < k; i++)
        items[i] = temporary[i];
}

static void merge_sort_segment(const int length,
                               double items[length], double temporary[length],
                               const int left, const int right)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || temporary != NULL);
    assert(length == 0 || left <= right);
    assert(0 <= left);
    assert(right < length);
    if(left >= right)
        return;
    int middle = (left + right) / 2;
    merge_sort_segment(length, items, temporary, left, middle);
    merge_sort_segment(length, items, temporary, middle + 1, right);
    merge(length, items, temporary, left, middle, right);
}

bool merge_sort(const int length, double items[length])
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    if(length <= 1)
        return false;
    double *const temporary = new_double_array_of(length);
    if(temporary == NULL)
        return true;
    merge_sort_segment(length, items, temporary, 0, length - 1);
    free(temporary);
    return false;
}

static void merge_and_count(const int length, double items[length],
                            double temporary[length],
                            const int left, const int middle, const int right,
                            struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || temporary != NULL);
    assert(length == 0 || counts != NULL);
    assert(0 <= left && left < length);
    assert(0 <= right && right < length);
    assert(left <= middle && middle < right);
    int i = left;
    int j = middle + 1;
    int k = left;
    while(i <= middle && j <= right)
    {
        counts->comparisons++;
        counts->copies++;
        if(items[i] <= items[j])
            temporary[k++] = items[i++];
        else
            temporary[k++] = items[j++];
    }
    while(i <= middle)
    {
        counts->copies++;
        temporary[k++] = items[i++];
    }
    for(int i = left; i < j; i++)
    {
        counts->copies++;
        items[i] = temporary[i];
    }
}

static void merge_sort_segment_and_count(const int length,
        double items[length],
        double temporary[length],
        const int left, const int right,
        struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || temporary != NULL);
    assert(length == 0 || counts != NULL);
    assert(length == 0 || left <= right);
    assert(0 <= left);
    assert(right < length);
    if(left >= right)
        return;
    int middle = (right + left) / 2;
    merge_sort_segment_and_count(length, items, temporary,
                                 left, middle, counts);
    merge_sort_segment_and_count(length, items, temporary,
                                 middle + 1, right, counts);
    merge_and_count(length, items, temporary, left, middle, right, counts);
}

bool merge_sort_and_count(const int length, double items[length],
                          struct algorithm_counts* counts)
{
    assert(length >= 0);
    assert(length == 0 || items != NULL);
    assert(length == 0 || counts != NULL);
    if(length <= 1)
        return false;
    double *const temporary = new_double_array_of(length);
    if(temporary == NULL)
        return true;
    merge_sort_segment_and_count(length, items, temporary, 0, length - 1,
                                 counts);
    free(temporary);
    return false;
}
