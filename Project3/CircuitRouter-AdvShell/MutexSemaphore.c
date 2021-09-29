#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "MutexSemaphore.h"

 void mutex_unlock (pthread_mutex_t* mutex){
     if (pthread_mutex_unlock(mutex)){
         fprintf(stderr, "Error trying to unlock a mutex\n");
         exit(-1);
     }
 }

 void mutex_lock (pthread_mutex_t* mutex){
     if (pthread_mutex_lock(mutex)){
         fprintf(stderr, "Error trying to lock a mutex\n");
         exit(-1);
     }
 }


 void mutex_destroy (pthread_mutex_t* mutex) {
     if (pthread_mutex_destroy(mutex)){
         fprintf(stderr, "Error trying to destroy a mutex\n");
         exit(-1);
     }
 }

 void mutex_init (pthread_mutex_t* mutex) {
     if (pthread_mutex_init(mutex, NULL)){
         fprintf(stderr, "Error trying to initialize a mutex\n");
         exit(-1);
     }
 }

void semaphore_init(sem_t* semaphore, int count) {
    if (sem_init(semaphore, 0, count) != 0) {
        printf("Error initializing a semaphore\n");
        exit(-1);
    }
}

void semaphore_post(sem_t* semaphore) {
    if (sem_post(semaphore) != 0) {
        printf("Error posting a semaphore\n");
        exit(-1);
    }
}

void semaphore_getvalue(sem_t* semaphore, int* sval) {
    if (sem_getvalue(semaphore, sval) != 0) {
        printf("Error trying to get a semaphore count\n");
        exit(-1);
    }
}

void semaphore_wait(sem_t* semaphore) {
    if (sem_wait(semaphore) != 0) {
        printf("Error waiting for a semaphore\n");
        exit(-1);
    }
}


void semaphore_destroy(sem_t* semaphore) {
    if (sem_destroy(semaphore) != 0) {
        printf("Error trying to destroy a semaphore\n");
        exit(-1);
    }
}

