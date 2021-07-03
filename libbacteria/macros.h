#pragma once
#define eprintf(...) fprintf (stderr, __VA_ARGS__)
#define EXIT_FAILURE 1

#define MAX_LISTEN 100

#define PROGRAM_INFO __DATE__ "\tBacteria\t" __TIME__
#ifndef __cplusplus
typedef enum{false,true}bool;
#else
#endif
