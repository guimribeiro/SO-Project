#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>



#define MAXARGS 2
#define BUFFER_SIZE 100

void catch_sigpipe(int signo) {
    signal(SIGPIPE, catch_sigpipe);
    printf("Server has been closed\n");
    exit(-1);
}

int main (int argc, char** argv) {

    if (signal(SIGPIPE, catch_sigpipe) == SIG_ERR) {
        printf("Error setting a signal handler\n");
        exit(-1);
    }

    char caminho[] = "/tmp/diretorio.XXXXXX";
    int pserver;
    FILE* presposta;

    if (argc == 1){
      printf("Missing an argument\n");
    }

    else if (argc > 2){
      printf("Too many arguments\n");
    }

    else{
        //Abre pipe de escrita
        char path[strlen(argv[1]) + 1];
        strcpy(path, argv[1]);
        if ((pserver = open (path, O_WRONLY)) < 0){
            printf("Unable do open pipe\n");
            exit (-1);
        }

        //Cria diretoria aleatoria onde vai ser criado o pipe de resposta
        char dirtmp[BUFFER_SIZE];
        strcpy(dirtmp, mkdtemp(caminho));
        if(dirtmp == NULL){
             perror("mkdtemp failed ");
             exit(-1);
        }

        //Guarda o nome da diretoria em dir
        char dir[strlen(dirtmp)+1];
        strcpy(dir, dirtmp);
        strcat(dirtmp, "/PipeDeResposta.pipe");

        //Guarda o path do pipe de resposta em pres
        char pres[strlen(dirtmp)+1];
        strcpy(pres, dirtmp);

        //Cria pipe de resposta
        if ((mkfifo(dirtmp, 0777)<0) && (errno!= EEXIST)){
            perror("Cannot create pipe: ");
            exit(-1);
        }

        //Abre pipe de resposta
        if ((presposta = fopen (dirtmp,"r+")) == NULL){
            printf("Unable do open pipe\n");
            exit (-1);
        }

        //Ciclo para o pipe de escrita
        //Sai do ciclo quando premida a tecla q
        while(1){
            char resposta[BUFFER_SIZE];
            char buffer[BUFFER_SIZE];
            char pipe[BUFFER_SIZE] = "\0";

            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                printf("Error reading from stdin\n");
                exit(-1);
            }
            if (strcmp(buffer, "q\n") == 0) {
                break;
            }
            if (strcmp(buffer, "\n") != 0){
                strcat(pipe, pres);
                strcat(pipe, " ");
                strcat(pipe, buffer);
                strcat(pipe, "\n");
                int len = strlen(pipe);
                if (write(pserver, pipe, len) != len) {
                    printf("Error sending information to the server\n");
                    exit(-1);
                }
                if (fgets(resposta, BUFFER_SIZE, presposta) == NULL) {
                    printf("Error receiving information from the server\n");
                    exit(-1);
                }
                printf("%s",resposta);
            }
        }
        close(pserver);
        fclose(presposta);
        unlink(pres);
        if (rmdir(dir) == -1) {
            perror("rmdir failed: ");
            exit(-1);
        }
    }
    return 0;
}
