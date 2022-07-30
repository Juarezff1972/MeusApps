/**
 * \file fibonacci.c
 * \brief Illustrates possible implementations of the calculation of terms of
 * the Fibonacci sequence.
 *
 * Iterative and recursive implementations of the calculation of terms of
 * the Fibonacci sequence. These implementations are used to illustrate
 * iteration vs. recursion, and also to show how a naïve implementation of
 * recursion may lead to serious problems and how lookup tables can be used to
 * speed up calculations.
 */
/** \brief The index of the largest term of the Fibonacci sequence that fits
 * within a `long`. */
#define MAXIMUM_TERM_FITTING_A_LONG 92
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "sequence_of_longs.h"
/** \brief Returns the `n`th term of the Fiboncci sequence.
 *
 * \param n The number of the term to return (first valid value is 0).
 * \return The value of the `n`th term \f$F_{\mathtt{n}}\f$ of the Fibonacci
 * sequence.
 * \pre `n` ≥ 0
 * \post result = \f$F_{\mathtt{n}}\f$
 *
 * Returns the `n`th term of the [Fibonacci
 * sequence](http://mathworld.wolfram.com/FibonacciNumber.html), i.e.,
 * \f$F_{\mathtt{n}}\f$. It is assumed the sequence \f$F_n\f$ starts at
 * \f$n=0\f$, with value 0, followed by value 1, at \f$n=1\f$. That is, the
 * sequence is defined by
 * \f[
 * F_n = \left\{\begin{array}{ll}
 *     0                 & \text{if } n=0, \\
 *     1                 & \text{if } n=1\text{, and} \\
 *     F_{n-2} + F_{n-1} & \text{if } n>1.
 *   \end{array}\right.
 * \f]
 * It can be shown that
 * \f[
 * F_n = \frac{\phi^n-\psi^n}{\sqrt{5}},
 * \f]
 * where \f$\phi=\frac{1+\sqrt{5}}{2}\f$ and \f$\psi=\frac{1-\sqrt{5}}{2}\f$,
 * and also that, even more simple,
 * \f[
 * F_n = \left[\frac{\phi^n}{\sqrt{5}}\right],
 * \f]
 * where \f$[\cdot]\f$ is the nearest integer function.
 *
 * The time taken by the function grows exponentially with `n`. More precisely,
 * the number of (recursive) executions of this function performed
 * while calculating \f$F_n\f$ by calling
 * `stupidly_recursive_fibonacci(`\f$n\f$`)` is \f$2F_{n+1}-1\f$ and the number
 * of aditions performed is \f$F_{n+1}-1\f$.
 */
long stupidly_recursive_fibonacci (int n)
{
    assert (n >= 0);
    assert (n <= MAXIMUM_TERM_FITTING_A_LONG);

    if (n == 0)
        return 0L;

    if (n == 1)
        return 1L;

    return stupidly_recursive_fibonacci (n - 2)
           + stupidly_recursive_fibonacci (n - 1);
}
/** \brief Returns the `n`th term of the Fiboncci sequence.
 *
 * \param n The number of the term to return (first valid value is 0).
 * \return The value of the `n`th term \f$F_{\mathtt{n}}\f$ of the Fibonacci
 * sequence.
 * \pre `n` ≥ 0
 * \post result = \f$F_{\mathtt{n}}\f$
 *
 * Returns the `n`th term of the [Fibonacci
 * sequence](http://mathworld.wolfram.com/FibonacciNumber.html), i.e.,
 * \f$F_{\mathtt{n}}\f$. See stupidly_recursive_fibonacci() for further
 * information.
 *
 * This function uses a static array of #MAXIMUM_TERM_FITTING_A_LONG `long`s to
 * store calculated values of the terms of the sequence. After a call to the
 * function with argument \f$n\f$, all future calls to the function with any
 * argument between 0 and \f$n\f$ will execute in constant time. When this
 * doesn't apply, the function will execute in linear time, i.e., \f$O(n)\f$.
 * More precisely, while calculating \f$F_n\f$ by calling
 * `recursive_fibonacci(`\f$n\f$`)`,
 * * the number \f$N(n)\f$ of (recursive) executions of this function is 1 if
 *   \f$n=0\f$ and is \f$2n-1\f$ if \f$n>0\f$;
 * * the number \f$S(n)\f$ of aditions performed is 0 if \f$n=0\f$ and is
 *   \f$n-1\f$ if \f$n>0\f$;
 * * the number \f$T(n)\f$ of array item assignments performed is 1 if \f$n=0\f$
 *   or \f$n=1\f$, and is \f$n+1\f$ if \f$n>1\f$;
 * * the number \f$R(n)\f$ of array item reads performed is 0 if \f$n=0\f$
 *   or \f$n=1\f$, and is \f$n-2\f$ if \f$n>1\f$.
 *
 * These numbers are exact if no other calls to the function have been performed
 * previously, otherwise the values will be either smaller or equal to these. If
 * other calls with an argument larger or equal to \f$n\f$ were performed
 * previously, then \f$N(n)=1\f$, \f$S(n)=0\f$, \f$T(n)=0\f$, and \f$R(n)=1\f$.
 */
long recursive_fibonacci (int n)
{
    assert (n >= 0);
    assert (n <= MAXIMUM_TERM_FITTING_A_LONG);
    static long F[MAXIMUM_TERM_FITTING_A_LONG + 1] = {0, 1};
    static int number_of_calculated_terms = 2;

    if (n < number_of_calculated_terms)
        return F[n];

    F[n] = recursive_fibonacci (n - 2) + recursive_fibonacci (n - 1);
    number_of_calculated_terms++;
    return F[n];

    if (number_of_calculated_terms <= n)
    {
        F[n] = recursive_fibonacci (n - 2) +
               recursive_fibonacci (n - 1);
        number_of_calculated_terms++;
    }

    return F[n];
    number_of_calculated_terms++;
    return F[n] = recursive_fibonacci (n - 2) + recursive_fibonacci (n - 1);
}
/** \brief Returns the `n`th term of the Fiboncci sequence.
 *
 * \param n The number of the term to return (first valid value is 0).
 * \return The value of the `n`th term \f$F_{\mathtt{n}}\f$ of the Fibonacci
 * sequence.
 * \pre `n` ≥ 0
 * \post result = \f$F_{\mathtt{n}}\f$
 *
 * Returns the `n`th term of the [Fibonacci
 * sequence](http://mathworld.wolfram.com/FibonacciNumber.html), i.e.,
 * \f$F_{\mathtt{n}}\f$. See stupidly_recursive_fibonacci() and
 * recursive_fibonacci() for further information.
 *
 * This function uses the ADT `struct sequence_of_longs` store calculated values
 * of the terms of the sequence. After a call to the function with argument
 * \f$n\f$, all future calls to the function with any argument between 0 and
 * \f$n\f$ will execute in constant time. When this doesn't apply, the function
 * will execute in linear time, i.e., \f$O(n)\f$. More precisely, while
 * calculating \f$F_n\f$ by calling `recursive_fibonacci_using_ADT(`\f$n\f$`)`,
 * * the number \f$N(n)\f$ of (recursive) executions of this function is 1 if
 *   \f$n=0\f$ and is \f$2n-1\f$ if \f$n>0\f$;
 * * the number \f$S(n)\f$ of aditions performed is 0 if \f$n=0\f$ and is
 *   \f$n-1\f$ if \f$n>0\f$;
 * * the number \f$T(n)\f$ of additions to the memory of terms is 1 if
 *   \f$n=0\f$ or \f$n=1\f$, and is \f$n+1\f$ if \f$n>1\f$;
 * * the number \f$R(n)\f$ of accesses to the memory of terms is 0 if \f$n=0\f$
 *   or \f$n=1\f$, and is \f$n-2\f$ if \f$n>1\f$.
 *
 * These numbers are exact if no other calls to the function have been performed
 * previously, otherwise the values will be either smaller or equal to these. If
 * other calls with an argument larger or equal to \f$n\f$ were performed
 * previously, then \f$N(n)=1\f$, \f$S(n)=0\f$, \f$T(n)=0\f$, and \f$R(n)=1\f$.
 *
 * Additions of calculated terms to the term memory are less efficient when the
 * ADT `struct sequence_of_longs` is used than when a fixed size array is used.
 * However:
 * * The ADT guarantees that the amortized number of copies of terms perfomed
 *   while adding a new term is always less than 2, approaching 1 as the number
 *   of terms in the sequence grows to infinity.
 * * An ADT such as the one used would be necessary if big integers were used
 *   for the result of this function.
 *
 * See iterative_fibonacci() and tail_recursive_fibonacci() for fast
 * implementations of the Fibonacci sequence that do not require static storage
 * and that execute in linear time.
 */
long recursive_fibonacci_using_ADT (int n)
{
    assert (n >= 0);
    assert (n <= MAXIMUM_TERM_FITTING_A_LONG);
    static struct sequence_of_longs *F = NULL;

    if (F == NULL)
        F = SEQL_new();

    if (n < SEQL_length (F))
        return SEQL_term (F, n);

    long F_n;

    if (n == 0)
        F_n = 0L;
    else
        if (n == 1)
            F_n = 1L;
        else
            F_n = recursive_fibonacci_using_ADT (n - 2) +
                  recursive_fibonacci_using_ADT (n - 1);

    F_n = n == 0 ? 0L : n == 1 ? 1L :
               recursive_fibonacci_using_ADT (n - 2) +
               recursive_fibonacci_using_ADT (n - 1);
    SEQL_add (F, F_n);
    return F_n;
}
static long tr_fibonacci (int n, long previous_value, long value);
/** \brief Returns the `n`th term of the Fiboncci sequence.
 *
 * \param n The number of the term to return (first valid value is 0).
 * \return The value of the `n`th term \f$F_{\mathtt{n}}\f$ of the Fibonacci
 * sequence.
 * \pre `n` ≥ 0
 * \post result = \f$F_{\mathtt{n}}\f$
 *
 * Returns the `n`th term of the [Fibonacci
 * sequence](http://mathworld.wolfram.com/FibonacciNumber.html), i.e.,
 * \f$F_{\mathtt{n}}\f$. It is assumed the sequence \f$F_n\f$ starts at
 * \f$n=0\f$, with value 0, followed by value 1, at \f$n=1\f$. That is, the
 * sequence is defined by
 * \f[
 * F_n = \left\{\begin{array}{ll}
 *     0                 & \text{if } n=0, \\
 *     1                 & \text{if } n=1\text{, and} \\
 *     F_{n-2} + F_{n-1} & \text{if } n>1.
 *   \end{array}\right.
 * \f]
 * It can be shown that
 * \f[
 * F_n = \frac{\phi^n-\psi^n}{\sqrt{5}},
 * \f]
 * where \f$\phi=\frac{1+\sqrt{5}}{2}\f$ and \f$\psi=\frac{1-\sqrt{5}}{2}\f$,
 * and also that, even more simple,
 * \f[
 * F_n = \left[\frac{\phi^n}{\sqrt{5}}\right],
 * \f]
 * where \f$[\cdot]\f$ is the nearest integer function.
 *
 * The time taken by the function grows linearly with `n`. More precisely,
 * the number of (recursive) executions of function `tr_fibonacci()` performed
 * while calculating \f$F_n\f$ by calling
 * `tail_recursive_fibonacci(`\f$n\f$`)` is \f$n\f$ and the number
 * of aditions performed is 0 if \f$n=0\f$ and \f$n-1\f$ if \f$n>0\f$.
 *
 * See iterative_fibonacci() for a fast, iterative implementation of the
 * Fibonacci sequence with approximately the same efficiency as this
 * implementation.
 */
long tail_recursive_fibonacci (int n)
{
    assert (n >= 0);
    assert (n <= MAXIMUM_TERM_FITTING_A_LONG);

    if (n == 0)
        return 0L;

    return tr_fibonacci (n, 0L, 1L);
}
static long tr_fibonacci (int n, long previous_value, long value)
{
    assert (n >= 1);
    assert (n <= MAXIMUM_TERM_FITTING_A_LONG);

    if (n == 1)
        return value;

    return tr_fibonacci (n - 1, value, previous_value + value);
}
/** \brief Returns the `n`th term of the Fiboncci sequence.
 *
 * \param n The number of the term to return (first valid value is 0).
 * \return The value of the `n`th term \f$F_{\mathtt{n}}\f$ of the Fibonacci
 * sequence.
 * \pre `n` ≥ 0
 * \post result = \f$F_{\mathtt{n}}\f$
 *
 * Returns the `n`th term of the [Fibonacci
 * sequence](http://mathworld.wolfram.com/FibonacciNumber.html), i.e.,
 * \f$F_{\mathtt{n}}\f$. It is assumed the sequence \f$F_n\f$ starts at
 * \f$n=0\f$, with value 0, followed by value 1, at \f$n=1\f$. That is, the
 * sequence is defined by
 * \f[
 * F_n = \left\{\begin{array}{ll}
 *     0                 & \text{if } n=0, \\
 *     1                 & \text{if } n=1\text{, and} \\
 *     F_{n-2} + F_{n-1} & \text{if } n>1.
 *   \end{array}\right.
 * \f]
 * It can be shown that
 * \f[
 * F_n = \frac{\phi^n-\psi^n}{\sqrt{5}},
 * \f]
 * where \f$\phi=\frac{1+\sqrt{5}}{2}\f$ and \f$\psi=\frac{1-\sqrt{5}}{2}\f$,
 * and also that, even more simple,
 * \f[
 * F_n = \left[\frac{\phi^n}{\sqrt{5}}\right],
 * \f]
 * where \f$[\cdot]\f$ is the nearest integer function.
 *
 * The time taken by the function grows linearly with `n`. More precisely,
 * the number of aditions and subtractions performed is 0 if \f$n=0\f$ or
 * \f$n=1\f$, and \f$2(n-2)\f$ if \f$n>1\f$.
 *
 * See tail_recursive_fibonacci() for a fast, tail recursive implementation of
 * the Fibonacci sequence with approximately the same efficiency as this
 * implementation.
 */
long iterative_fibonacci (int n)
{
    assert (n >= 0);
    assert (n <= MAXIMUM_TERM_FITTING_A_LONG);

    if (n == 0)
        return 0L;

    if (n == 1)
        return 1L;

    long previous_term = 1L;
    long current_term = 1L;

    for (int i = 2; i != n; i++)
    {
        current_term += previous_term;
        previous_term = current_term - previous_term;
    }

    return current_term;
}
/** \brief Experiments a given Fibonacci sequence term calculation function for
 * a sequence of terms and reports the extimated execution times to `stdout`.
 *
 * \param title The title of the experiment.
 * \param fibonacci Pointer to the function to experiment with.
 * \param last_term_to_test The function will be experimented with terms from 0
 * up to this number.
 * \pre `title` ≠ null
 * \pre `fibonacci` ≠ null
 * \pre `last_term_to_test` ≥ 0
 * \post The results of the experiment are writen in `stdout`.
 *
 * This procedure performs experiments with the provided Fibonacci function
 * `fibonacci()`, reporting the estimated execution times for terms 0 up to
 * `last_term_to_test`. In order to overcome clock resolution issues, each
 * experiment is repeated until a total of at least 0.1 seconds is reached. For
 * all but the first term, the relative increases in execution time are also
 * reported.
 *
 * \bug The minimum accumulated time should not be hardcoded as 0.1 seconds. It
 * should be a parameter of the procedure instead.
 */
void experiment_efficiency_of (char title[], long fibonacci (int),
                               int last_term_to_test)
{
    assert (title != NULL);
    assert (fibonacci != NULL);
    assert (last_term_to_test >= 0);
    const clock_t minimum_accumulated_clocks =
        (clock_t) (0.1 * CLOCKS_PER_SEC);
    printf ("%s\n", title);
    double previous_time = 0.0;

    for (int n = 0; n != last_term_to_test + 1; n++)
    {
        long f_n;
        clock_t start = clock();
        int repetitions = 0;

        do
        {
            f_n = fibonacci (n);
            repetitions++;
        }
        while (clock() < start + minimum_accumulated_clocks);

        start = clock();

        for (int r = 0; r != repetitions; r++)
            f_n = fibonacci (n);

        clock_t end = clock();
        double time =
            (double) (end - start) / CLOCKS_PER_SEC / repetitions;

        if (previous_time == 0.0)
            printf ("F(%d) = %ld in %g s\n", n, f_n, time);
        else
            printf ("F(%d) = %ld in %.3g seconds, %+.1f%%\n", n, f_n,
                    time, time / previous_time * 100.0 - 100.0);

        previous_time = time;
    }
}
int main (void)
{
    const int last_term_to_test_with_stupid_algorithm = 42;
    experiment_efficiency_of (
        "Stupidly recursive implementation of the fibonacci sequence:",
        stupidly_recursive_fibonacci,
        last_term_to_test_with_stupid_algorithm);
    experiment_efficiency_of (
        "Recursive implementation of the fibonacci sequence using array"
        " for storage:",
        recursive_fibonacci, MAXIMUM_TERM_FITTING_A_LONG);
    experiment_efficiency_of (
        "Recursive implementation of the fibonacci sequence using ADT"
        " for storage:",
        recursive_fibonacci_using_ADT, MAXIMUM_TERM_FITTING_A_LONG);
    experiment_efficiency_of (
        "Tail recursive implementation of the fibonacci sequence:",
        iterative_fibonacci, MAXIMUM_TERM_FITTING_A_LONG);
    experiment_efficiency_of (
        "Two variable iterative implementation of the fibonacci"
        " sequence:",
        iterative_fibonacci, MAXIMUM_TERM_FITTING_A_LONG);

    for (int n = 0; n != MAXIMUM_TERM_FITTING_A_LONG + 1; n++)
    {
        if (n <= last_term_to_test_with_stupid_algorithm)
            printf ("F(%d) [stupidly recursive] = %ld\n",
                    n, stupidly_recursive_fibonacci (n));

        printf ("F(%d) [recursive         ] = %ld\n",
                n, recursive_fibonacci (n));
        printf ("F(%d) [recursive with ADT] = %ld\n",
                n, recursive_fibonacci_using_ADT (n));
        printf ("F(%d) [tail recursive    ] = %ld\n",
                n, tail_recursive_fibonacci (n));
        printf ("F(%d) [iterative         ] = %ld\n",
                n, iterative_fibonacci (n));
        putchar ('\n');
    }

    return EXIT_SUCCESS;
}

