#ifndef CIRCUITROUTER_SHELL_H
#define CIRCUITROUTER_SHELL_H

#include "lib/vector.h"
#include <sys/types.h>
#include "lib/timer.h"
#include "lib/types.h"

typedef struct {
    pid_t pid;
    int status;
    TIMER_T begin;
    TIMER_T finish;
} child_t;

void printChildren(vector_t *children);

#endif /* CIRCUITROUTER_SHELL_H */
