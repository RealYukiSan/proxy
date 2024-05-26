#ifdef VERBOSE

#define VERBOSE_PRINT(...) fprintf(stderr, __VA_ARGS__)

#else

#define VERBOSE_PRINT(...) { }

#endif

