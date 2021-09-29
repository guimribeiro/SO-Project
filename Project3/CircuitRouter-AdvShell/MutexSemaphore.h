#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void mutex_init (pthread_mutex_t* mutex);

void mutex_lock (pthread_mutex_t* mutex);

void mutex_unlock (pthread_mutex_t* mutex);

void mutex_destroy (pthread_mutex_t* mutex);

void semaphore_init(sem_t* semaphore, int count);

void semaphore_post(sem_t* semaphore);

void semaphore_getvalue(sem_t* semaphore, int* sval);

void semaphore_wait(sem_t* semaphore);

void semaphore_destroy(sem_t* semaphore);


