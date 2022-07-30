#include "sequence_of_longs.h"
#include <stdio.h>
#include <stdlib.h>
struct sequence_of_longs {
	long *terms;
	int length;
	int capacity;
};
void SEQL_print(struct sequence_of_longs *sl)
{
	putchar('{');
	for (int i = 0; i != sl->length; i++) {
		if (i != 0)
			printf(", ");
		printf("%ld", sl->terms[i]);
	}
	putchar('}');
}

void SEQL_println(struct sequence_of_longs *sl)
{
	SEQL_print(sl);
	putchar('\n');
}
struct sequence_of_longs *SEQL_new(void)
{
	struct sequence_of_longs *sl = malloc(sizeof(struct sequence_of_longs));
	sl->length = 0;
	sl->capacity = 1;
	sl->terms = malloc(sl->capacity * sizeof(long));
	return sl;
}
int SEQL_length(struct sequence_of_longs *sl)
{
	return sl->length;
}
void SEQL_add(struct sequence_of_longs *sl, long new_term)
{
	if (sl->length == sl->capacity)
		sl->terms = realloc(sl->terms,
				    (sl->capacity *= 2) * sizeof(long));

if (sl->length == sl->capacity) {
   sl->capacity *= 2;
   long *new_array = malloc(sl->capacity * sizeof(long));
   for (int i = 0; i != sl->length; i++)
       new_array[i] = sl->terms[i];
   free(sl->terms);
   sl->terms = new_array;
}

sl->terms[sl->length] = new_term;
sl->length++;

	sl->terms[sl->length++] = new_term;
}
long SEQL_term(struct sequence_of_longs *sl, int index)
{
	return sl->terms[index];
}


