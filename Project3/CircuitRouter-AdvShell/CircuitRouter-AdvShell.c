/*
// Projeto SO - exercise 1, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
*/
#include "MutexSemaphore.h"
#include "lib/commandlinereader.h"
#include "lib/vector.h"
#include "CircuitRouter-AdvShell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include "lib/timer.h"
#include "lib/types.h"

#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"
#define STDOUT 1

#define MAXARGS 3
#define BUFFER_SIZE 100
#define ERR_MSG "Command not supported\n"
#define ERR_TAM strlen(ERR_MSG)

typedef struct {
    FILE* server_pipe;
} argsReadFromPipe_t;

//GLOBALS

int MAXCHILDREN = -1;
int runningChildren = 0;
pthread_t id;
sem_t nChildren;
pthread_mutex_t  vector;
vector_t *children;

void waitForChild(){
    pid_t pid;
    int status;
    pid = wait(&status);
    if (pid < 0) {
        printf("Error waiting for child\n");
        exit(-1);
    }
    mutex_lock(&vector);
    TIMER_T stopTime;
    TIMER_READ(stopTime);

    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        if(child->pid == pid){
            child->status = status;
            child->finish = stopTime;
        }
    }
    mutex_unlock(&vector);
    if (MAXCHILDREN == -1) {
        runningChildren--;
    }
    else {
        semaphore_post(&nChildren);
    }
}

void handler_signal(int sinal){
    signal(SIGCHLD, handler_signal);
    waitForChild();
}

void addChild(int pid) {
    if (MAXCHILDREN == -1) {
        runningChildren++;
    }
    child_t *child = malloc(sizeof(child_t));
    if (child == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    mutex_lock(&vector);
    TIMER_T startTime;
    TIMER_READ(startTime);
    child->pid = pid;
    child->begin = startTime;
    vector_pushBack(children, child);
    mutex_unlock(&vector);
}

void printChildren(vector_t *children) {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;
        TIMER_T begin = child->begin;
        TIMER_T finish = child->finish;
        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            float time = TIMER_DIFF_SECONDS(begin, finish);
            printf("CHILD EXITED: (PID=%d; return %s; %.4f s)\n", pid, ret, time);
        }
    }
    puts("END.");
}

void evalArgs(char** args, int numArgs, int outputfile, char* client_pipe_name) {
    if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0){
        int pid;
        if (numArgs < 2) {
            if (write(outputfile, ERR_MSG, ERR_TAM) != ERR_TAM) {
                printf("Error sending information to a client\n");
                exit(-1);
            }
            return;
        }

        if (MAXCHILDREN != -1) {
            semaphore_wait(&nChildren);
        }

        pid = fork();

        if (pid < 0) {
            perror("Failed to create new process.");
            exit(EXIT_FAILURE);
        }

        if (pid > 0) {
            addChild(pid);
            printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
            return;

        } else {
            char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
            char *newArgs[4] = {seqsolver, args[1], client_pipe_name, NULL};
            execv(seqsolver, newArgs);
            perror("Error while executing child process"); // Nao deveria chegar aqui
            exit(EXIT_FAILURE);
        }
    }
    else{
        if (write(outputfile, ERR_MSG, ERR_TAM) != ERR_TAM) {
            printf("Error sending information to a client\n");
            exit(-1);
        }
    }
}

void readFromStdin() {
    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int outputfile = STDOUT;
    char* client_pipe_name = NULL;

    while (1) {
        int numArgs;
        numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE, stdin);

        if (numArgs == 0)
            continue;

        else if ((numArgs < 0 || (numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0)))) {
            printf("CircuitRouter-AdvShell will exit.\n--\n");

            if (pthread_cancel(id)) {
                printf("Error trying to cancel a thread\n");
                exit(-1);
            }
            /* Espera pela terminacao de cada filho */
            if (MAXCHILDREN == -1) {
                while (runningChildren > 0) {
                    pause();
                }
            }
            else {
                int sval = 0;
                semaphore_getvalue(&nChildren, &sval);
                while (sval < MAXCHILDREN) {
                    pause();
                    semaphore_getvalue(&nChildren, &sval);
                }
            }
            printChildren(children);
            printf("--\nCircuitRouter-AdvShell ended.\n");
            break;
        }

        evalArgs(args, numArgs, outputfile, client_pipe_name);
        }
}

void readFromPipe(argsReadFromPipe_t* argsReadClients) {
    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int outputfile = 0;
    char* client_pipe_name = NULL;

    FILE* pserv = argsReadClients -> server_pipe;

    while (1) {
        int numArgs;
        numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE, pserv);

        if (numArgs == 0)
            continue;

        client_pipe_name = args[0];
        if ((outputfile = open (client_pipe_name,O_WRONLY)) < 0){
            printf("Unable to open client pipe\n");
            exit (-1);
        }
        numArgs--;

        evalArgs((args + 1), numArgs, outputfile, client_pipe_name);
        close(outputfile);
    }
}

int main (int argc, char** argv) {

    FILE* pserv;

    if(argv[1] != NULL){
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN);

    printf("Welcome to CircuitRouter-AdvShell\n\n");

    char pipe_buffer[BUFFER_SIZE];
    char* pipe_name = NULL;
    if (!(pipe_name = getcwd(pipe_buffer, BUFFER_SIZE))) {
        printf("Error trying to access directory name\n");
        exit(-1);
    }
    strcat(pipe_name, ".pipe");

    unlink(pipe_name);

    if (mkfifo (pipe_name, 0666) < 0) {
        printf("Unable to create pipe\n");
        exit (-1);
    }

    if ((pserv = fopen (pipe_name,"r+")) == NULL){
        printf("Unable do open pipe\n");
        exit (-1);
    }

    argsReadFromPipe_t remoteClientsArgs = {pserv};
    //argsReadClients_t localClientsArgs = {stdin};

    if (MAXCHILDREN != -1) {
        semaphore_init(&nChildren, MAXCHILDREN);
    }
    mutex_init(&vector);

    //Instala a rotina de tratamento para o SIGCHLD

    if (signal(SIGCHLD, handler_signal) == SIG_ERR){
        printf("Error trying to install a signal\n");
    }

    if (pthread_create(&id, NULL, (void*)readFromPipe, (void*)&remoteClientsArgs)) {
        printf("Error creating a thread\n");
        exit(-1);
    }

    readFromStdin();

    if (pthread_join(id, NULL)) {
        printf("Error joining a thread\n");
        exit(-1);
    }

    for (int i = 0; i < vector_getSize(children); i++) {
        free(vector_at(children, i));
    }
    vector_free(children);

    if (MAXCHILDREN != -1) {
        semaphore_destroy(&nChildren);
    }
    mutex_destroy(&vector);

    fclose (pserv);
    unlink(pipe_name);

    return EXIT_SUCCESS;
}
