#ifndef mymacros_h
#define mymacros_h

#define get_new(type) ((type *) chk_malloc(sizeof(type)))
#define make_between(x, lo, hi) ((x) > (lo) ? ((x) < (hi) ? (x):(hi)) : (lo))
#define is_between(x, lo, hi) ((x) <= (hi) && (x) >= (lo))
#define step_arg  (argc--, argv++)
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sqr(x) ((x)*(x))
#define MAXINT 0x7fffffff
#define MININT 0x80000000
#define MAXFLOAT 1e40
#define YES 1
#define NO 0

#endif
