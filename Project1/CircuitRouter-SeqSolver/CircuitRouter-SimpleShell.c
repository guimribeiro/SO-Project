#include <lib/vector.h>
#include <lib/commandlinereader.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFERSIZE 1000
#define MAXCOM 4
#define OK 1
#define NOK 0

typedef struct process{
    int pid;
    int exitstatus;
}process;

process* process_alloc(int pid);
void process_free(process* processPtr);
void processVector_free(vector_t* vetor, long size);
void execute_run(char* inputfile, vector_t* vector, long maxChil, int* contador);
void run(char* inputfile, int* contador);
void progEnd(vector_t* vector, int* contador);
int is_number(char* string);
void printVetor( vector_t* vector);

/* =============================================================================
 * process_alloc
 * =============================================================================
 */
process* process_alloc(int pid){
        process* processPtr = (process*)malloc(sizeof(process));
        processPtr->pid = pid;
        return processPtr;
}

/* =============================================================================
 * process_free
 * =============================================================================
 */
void process_free(process* processPtr){
    free(processPtr);
}

/* =============================================================================
 * processVector_free: Liberta a memoria alocada para cada processo no vetor
 * =============================================================================
 */
void processVector_free(vector_t* vetor, long size){
    long i;
    for(i = 0; i < size; i++){
        process_free(vetor->elements[i]);
    }
}

/* =============================================================================
 * execute_run: Recebe um ficheiro de input e executa os comandos nele contidos.
 *              Espera que um dos filhos termine a execução (guardando os seus
                dados), caso não possa haver mais processos filhos a decorrer
                em simultâneo
 * =============================================================================
 */
void execute_run(char* inputfile, vector_t* vector, long maxChil, int* contador){
    int pid;
    int estado;
    process* processPtr = NULL;
    if (maxChil > *contador || maxChil == -1){
        (*contador)++;
        run(inputfile, contador);
    }
    else{
        pid = wait(&estado);
        processPtr = process_alloc(pid);
        vector_pushBack(vector, processPtr);
        if (WIFEXITED(estado) && WEXITSTATUS(estado) == 0){
            processPtr->exitstatus = OK;
        }
        else
            processPtr->exitstatus = NOK;
        run(inputfile, contador);
    }
}

/* =============================================================================
 * run: Cria os processos filhos de acordo com os comandos fornecidos no input
        file
 * =============================================================================
 */
void run(char* inputfile, int* contador){
    int pid;
    pid = fork();
    if (pid<0){
        printf("Unable to fork");
        (*contador)--;
    }
    else if (pid==0)
        execl("CircuitRouter-SeqSolver", "CircuitRouter-SeqSolver", inputfile, NULL);
}

/* =============================================================================
 * progEnd: Avalia se existem processos filhos em execucao, aguarda o seu termino
 *          para poder terminar o programa.
 * =============================================================================
 */
void progEnd(vector_t* vector, int* contador){
    int pid;
    int estado;
    process* processPtr = NULL;
    while((*contador) != 0){
        pid = wait(&estado);
        processPtr = process_alloc(pid);
        vector_pushBack(vector, processPtr);
        (*contador)--;
        if (WIFEXITED(estado) && WEXITSTATUS(estado) == 0){
            processPtr->exitstatus = OK;
        }
        else
            processPtr->exitstatus = NOK;
    }

    printVetor(vector);

}

/* =============================================================================
 * printVector: Imprime no stdout o pid dos processos filhos e a forma como finalizaram
 * =============================================================================
 */
void printVetor( vector_t* vector){

    long i;
    process* elem_atual;
    long tam = vector_getSize(vector);
    for(i = 0; i < tam; i++){
        elem_atual = vector_at(vector, i);
        printf("CHILD EXITED (PID=%d; return", elem_atual->pid);
        if(elem_atual->exitstatus == OK){
            printf(" OK)\n");
        }
        else{
            printf(" NOK)\n");
        }
    }
    printf("END.\n");
}

/* =============================================================================
 * is_number: verifica se a string fornecida e um inteiro ou nao
 * =============================================================================
 */
int is_number(char* string){
    int i;
    int tam = strlen(string);
    for (i = 0; i < tam; i++){
        if (string[i] < '0' || string[i] > '9')
            return 0;
    }
    return 1;
}

int main(int argc,char** argv){
    int contador = 0;
    long maxchildren = -1;

    if (argc>2){
        fprintf(stderr, "Number of arguments should be 0 or 1\n");
        exit(-1);
    }

    else if (argc==2){
        if (is_number(argv[1])){
            maxchildren = atoi(argv[1]);
            if (maxchildren == 0){
                fprintf(stderr, "Second argument must be a positive integer\n");
                exit(-1);
            }
        }
        else{
            fprintf(stderr, "Second argument must be a positive integer\n");
            exit(-1);
        }
    }

    vector_t* vector = vector_alloc(maxchildren);

    char* argVector[MAXCOM];
    char buffer[BUFFERSIZE];
    int numTokens;
    char command[10];

    while (1){

        numTokens = readLineArguments(argVector, MAXCOM, buffer, BUFFERSIZE);
        if (numTokens == 0){
            fprintf(stderr, "Commands: run or exit\n");
            continue;
        }
        if (numTokens > 2){
            fprintf(stderr, "Invalid number of arguments\n");
            continue;
        }

        strcpy(command, argVector[0]);

        if (!(strcmp("run", command))){
            if (argVector[1] != NULL){
                execute_run(argVector[1], vector, maxchildren, &contador);
            }
            else
                fprintf(stderr, "Run command must have a file as an argument\n");
        }
        else if (!(strcmp("exit", command))){
            if (argVector[1] != NULL)
                fprintf(stderr, "Exit command does't have arguments\n");
            else{
                progEnd(vector, &contador);
                break;
            }
        }
        else
            fprintf(stderr, "Commands: run or exit\n");
    }

    processVector_free(vector, vector->size);
    vector_free(vector);

    return 0;
}
