#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include "coordinate.h"
#include "grid.h"
#include "gridMutex.h"
#include "lib/types.h"
#include "lib/vector.h"

#define MAX_TIME 100

/* =============================================================================
 * grid_mutex_alloc
 * =============================================================================
 */
grid_mutex_t* grid_mutex_alloc (long width, long height, long depth){
    grid_mutex_t* gridPtr;

    gridPtr = (grid_mutex_t*)malloc(sizeof(grid_mutex_t));
    if (gridPtr) {
        gridPtr->width  = width;
        gridPtr->height = height;
        gridPtr->depth  = depth;
        long n = width * height * depth;
        pthread_mutex_t* mutexes = (pthread_mutex_t*)malloc(n * sizeof(pthread_mutex_t));
        assert(mutexes);
        for (long i = 0; i < n; i++){
            if (pthread_mutex_init(&mutexes[i],NULL) != 0){
                fprintf(stderr, "Error initializing mutex\n");
                exit(-1);
            }
        }
        gridPtr->mutexes = mutexes;
    }

    return gridPtr;
}

/* =============================================================================
 * grid_mutex_free
 * =============================================================================
 */
void grid_mutex_free (grid_mutex_t* gridPtr){
    long n = gridPtr->width * gridPtr->height * gridPtr->depth;
      for (long i = 0; i < n; i++){
        mutex_destroy(&(gridPtr->mutexes[i]));
    }
    free(gridPtr->mutexes);
    free(gridPtr);
}


/* =============================================================================
 * grid_getMutex
 * =============================================================================
 */
pthread_mutex_t* grid_getMutex (grid_mutex_t* gridPtr, long x, long y, long z){
    return &(gridPtr->mutexes[(z * gridPtr->height + y) * gridPtr->width + x]);
}


/* =============================================================================
 * grid_pointRefToMutex
 * =============================================================================
 */
pthread_mutex_t* grid_pointRefToMutex (grid_t* gridPtr, grid_mutex_t* gridMutexPtr, long* gridPointPtr){
    long index = (gridPointPtr - gridPtr->points);
    return &(gridMutexPtr->mutexes[index]);
}


/* =============================================================================
 * grid_mutex_lock
 * =============================================================================
 */
void grid_mutex_lock (grid_mutex_t* gridPtr, long x, long y, long z){
    if (pthread_mutex_lock(grid_getMutex(gridPtr, x, y, z)) != 0){
        fprintf(stderr, "Error locking a mutex\n");
        exit(-1);
    }
}


/* =============================================================================
 * grid_mutex_unlock
 * =============================================================================
 */


void grid_mutex_unlock (grid_mutex_t* gridPtr, long x, long y, long z){
    if (pthread_mutex_unlock(grid_getMutex(gridPtr, x, y, z))){
        fprintf(stderr, "Error locking a mutex\n");
        exit(-1);
    }
}


/* =============================================================================
 * mutex_lock
 * =============================================================================
 */
 void mutex_unlock (pthread_mutex_t* mutex){
     if (pthread_mutex_unlock(mutex)){
         fprintf(stderr, "Error trying to unlock a mutex\n");
         exit(-1);
     }
 }


 /* =============================================================================
  * mutex_unlock
  * =============================================================================
  */
 void mutex_lock (pthread_mutex_t* mutex){
     if (pthread_mutex_lock(mutex)){
         fprintf(stderr, "Error trying to lock a mutex\n");
         exit(-1);
     }
 }


 /* =============================================================================
  * mutex_destroy
  * =============================================================================
  */
 void mutex_destroy (pthread_mutex_t* mutex) {
     if (pthread_mutex_destroy(mutex)){
         fprintf(stderr, "Error trying to destroy a mutex\n");
         exit(-1);
     }
 }


 /* =============================================================================
  * mutex_init
  * =============================================================================
  */
 void mutex_init (pthread_mutex_t* mutex) {
     if (pthread_mutex_init(mutex, NULL
     )){
         fprintf(stderr, "Error trying to initialize a mutex\n");
         exit(-1);
     }
 }


 /* =============================================================================
  * barrier_init
  * =============================================================================
  */
 void barrier_init (pthread_barrier_t* pthread_barrier, unsigned nthreads){
      if (pthread_barrier_init(pthread_barrier, NULL, nthreads)){
          fprintf(stderr, "Error trying to initialize a barrier\n");
          exit(-1);
      }
 }


 /* =============================================================================
  * barrier_wait
  * =============================================================================
  */
 void barrier_wait (pthread_barrier_t* pthread_barrier){
      int result = pthread_barrier_wait(pthread_barrier);
      if (!(result == 0 || result == PTHREAD_BARRIER_SERIAL_THREAD)){
          fprintf(stderr, "Error waiting for threads in a barrier\n");
          exit(-1);
      }
 }


 /* =============================================================================
  * barrier_destroy
  * =============================================================================
  */
 void barrier_destroy (pthread_barrier_t* pthread_barrier){
      if (pthread_barrier_destroy(pthread_barrier)){
          fprintf(stderr, "Error trying to destroy a barrier\n");
          exit(-1);
      }
 }


/* =============================================================================
 * mutex_trylock
 * =============================================================================
 */
bool_t mutex_trylock (pthread_mutex_t* mutex){
    int result = pthread_mutex_trylock(mutex);
    if (result == 0)
        return TRUE;
    else if (result == EBUSY)
            return FALSE;
    else{
        fprintf(stderr, "Error trying to lock a mutex\n");
        exit(-1);
    }
}


/* =============================================================================
 * grid_mutex_addPath_Ptr
 * =============================================================================
 */


bool_t grid_mutex_addPath_Ptr (grid_t* gridPtr, grid_mutex_t* gridMutexPtr, vector_t* pointVectorPtr){
    long i;
    long j;
    long n = vector_getSize(pointVectorPtr);

    for (i = 1; i < (n-1); i++) {
        long* gridPointPtr = (long*)vector_at(pointVectorPtr, i);
        pthread_mutex_t* mutex = grid_pointRefToMutex (gridPtr, gridMutexPtr, gridPointPtr);
        if (mutex_trylock(mutex) == FALSE){
            for (j = 1; j < i; j++){
                gridPointPtr = (long*)vector_at(pointVectorPtr, j);
                *gridPointPtr = GRID_POINT_EMPTY;
                mutex = grid_pointRefToMutex (gridPtr, gridMutexPtr, gridPointPtr);
                mutex_unlock(mutex);
            }
            return FALSE;
        }
        else
            *gridPointPtr = GRID_POINT_FULL;
    }
    return TRUE;
}


/* =============================================================================
 * grid_mutex_unlockPath_Ptr
 * =============================================================================
 */


void grid_mutex_unlockPath_Ptr (grid_t* gridPtr, grid_mutex_t* gridMutexPtr, vector_t* pathVectorPtr){
    long i, j;
    long nPaths = vector_getSize(pathVectorPtr);
    long nPoints;
    for (j = 0; j < nPaths ; j++){
        vector_t* pointVectorPtr = (vector_t*)vector_at(pathVectorPtr, j);
        nPoints = vector_getSize(pointVectorPtr);
        for (i = 1; i < (nPoints-1); i++) {
            long* gridPointPtr = (long*)vector_at(pointVectorPtr, i);
            pthread_mutex_t* mutex = grid_pointRefToMutex (gridPtr, gridMutexPtr, gridPointPtr);
            mutex_unlock(mutex);
        }
    }
}


/* =============================================================================
 * backoff
 * =============================================================================
 */


void backoff(long n_attempts) {
    double time = ldexp((random() % MAX_TIME), n_attempts);
    if (usleep(time) != 0){
        printf("Sleeping thread has been interrupted\n");
        exit(-1);
    }
}
