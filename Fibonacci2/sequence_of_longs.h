/**
 * \file sequence_of_longs.h
 * \brief Header file for the `sequence_of_longs` module, containing the ADT
 * (Abstract Data Type) with the same name: `sequence_of_longs`.
 *
 * This header file declares the basic structure used to store the sequences
 * of longs and all the routines used to manipulate these sequences.
 */
#ifndef ISLA_EDA_SEQUENCE_OF_LONGS_H_INCLUDED
#define ISLA_EDA_SEQUENCE_OF_LONGS_H_INCLUDED
/** \struct sequence_of_longs
 * \brief C structure used to represent the sequence of `long`s and to
 * store its terms.
 *
 * This TAD guarantees constant time additions of new terms, but at the cost of
 * allocating more memory than strictly necessary. The memory allocated may
 * approach the double of the minimum strictly necessary memory.
 */
struct sequence_of_longs;
/** \brief Returns a pointer to a newly created and initialized
 * `sequence_of_longs`.
 *
 * \return A pointer to a newly (heap) allocated and initialized
 * `struct sequence_of_longs`.
 * \post The returned pointer refers to a new `struct sequence_of_longs`
 * representing an empty (i.e., length 0) sequence of `long`s.
 *
 * This function is a constructor of the sequence of `long`s ADT.
 */
struct sequence_of_longs *SEQL_new(void);
/** \brief Prints the sequence of longs in the format `{term_1, ... term_n}`.
 *
 * \param sl A pointer to the sequence of `long`s to print.
 * \pre `sl` ≠ null
 */
void SEQL_print(struct sequence_of_longs *sl);

/** \brief Prints the sequence of longs in the format `{term_1, ... term_n}` and
 * ends the line with `\n`.
 *
 * \param sl A pointer to the sequence of `long`s to print.
 * \pre `sl` ≠ null
 */
void SEQL_println(struct sequence_of_longs *sl);

/** \brief Returns the number of terms so far in a given sequence of `long`s.
 *
 * \param sl A pointer to the sequence of `long`s whose length will be returned.
 * \return The number of terms in the sequence so far.
 * \pre `sl` ≠ null
 */
int SEQL_length(struct sequence_of_longs *sl);

/** \brief Adds a given value as a further term of the given sequence of
 * `long`s.
 *
 * \param sl A pointer to the sequence of `long`s to which the new term will be
 * added.
 * \param new_term The new term to add to the sequence.
 * \post The sequence contains the same terms it contained before, in the same
 * order, plus the new term in its last position.
 * \pre `sl` ≠ null
 *
 * The time required to add the new term depends on the non-observable state of
 * the sequence. If this time is amortized over the series of additions prior to
 * addition being performed, then this time is constant with regards to the
 * length of the sequence.
 */
void SEQL_add(struct sequence_of_longs *sl, long new_term);

/** \brief Returns the term of the given sequence at the given position or
 * index.
 *
 * \param sl A pointer to the sequence of `long`s whose length will be returned.
 * \param index The index or position of the term of the sequence to return.
 * \return The term of the sequence given in the position or index given.
 * \pre `sl` ≠ null
 * \pre 0 ≤ `index` < `SEQL_length(sl)`
 */
long SEQL_term(struct sequence_of_longs *sl, int index);
#endif // ISLA_EDA_SEQUENCE_OF_LONGS_H_INCLUDED
