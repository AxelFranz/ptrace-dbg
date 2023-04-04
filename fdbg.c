#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <stdarg.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

#define SYSCALL_MAX 128

noreturn void raler (int syserr, const char *msg, ...)
{
    va_list ap;

    va_start (ap, msg);
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");
    va_end (ap);

    if (syserr == 1)
        perror ("");

    exit (EXIT_FAILURE);
}

#define CHK(op) do { if ((op) == -1) raler (1, #op); } while (0)

#define WORD_SIZE 8

char**  parseFile(){
    FILE* fp;
    char** calls = NULL;
    size_t size, i;
    char buffer[SYSCALL_MAX];

    fp = fopen("syscalls.txt","r");
    if(fp == NULL){
        raler(1,"erreur fopen");
    }

    fscanf(fp,"%jd",&size);
    if((calls = malloc(size * sizeof(char*))) == NULL ){
        raler(1,"erreur malloc");
    }

    for(i = 0; i < size; i++){
        calls[i] = malloc(SYSCALL_MAX * sizeof(char));
        if(calls[i] == NULL) {
            raler(1,"erreur malloc");
        }
    }

    i = 0;
    while(fscanf(fp,"%s", buffer) != EOF){
        strncpy(calls[i],buffer,SYSCALL_MAX);
        i++;
    }


    if(fclose(fp) == EOF){
        raler(1,"erreur fclose");
    }
    return calls;
}

void freeSyscalls(char** calls){
    FILE* fp;
    size_t size;
    if((fp = fopen("syscalls.txt","r")) == NULL){
        raler(1,"erreur fopen");
    }

    fscanf(fp,"%jd",&size);
    for(size_t i = 0; i < size; i++){
        free(calls[i]);
    }
    free(calls);

    if(fclose(fp) == EOF){
        raler(1,"erreur, fopen");
    }
}

void sysTreat(pid_t child, char** calls){
    CHK(ptrace(PTRACE_SYSCALL, child, NULL, NULL));

    int status;
    char isReturn = 0;
    int orig_rax; 
    while(1){
        wait(&status);
        if(WIFEXITED(status)){
            exit(0);
        }

        CHK(orig_rax = ptrace(PTRACE_PEEKUSER, child, WORD_SIZE * ORIG_RAX,NULL));

        if(!isReturn){
            printf("Syscall number %d : %s\n", orig_rax, calls[orig_rax]);
        }
        isReturn = (isReturn + 1) % 2;
        CHK(ptrace(PTRACE_SYSCALL, child, NULL, NULL));

    }
}

void sbsTreat(pid_t child, char** calls){
    (void) calls;
    unsigned long line = 1;
    CHK(ptrace(PTRACE_SINGLESTEP,child,NULL,NULL));

    int status = 0;
    while(1){
        CHK(wait(&status));
        if(WIFEXITED(status)){
            exit(0);
        }

        printf("Ligne numero %ld\n",line);
        line++;

        CHK(ptrace(PTRACE_SINGLESTEP,child,NULL,NULL));
    }
}

int main(int argc, char** argv) {
    pid_t child;
    void (*treat)(pid_t, char**);
    char** calls = parseFile();

    if(argc < 3) {
        raler(0,"Usage : ./fdbg [--sbs | --sys]  <COMMAND TO EXEC>");
    } else if ( strncmp(argv[1], "--sys", strlen(argv[1])) == 0 ){
        treat = sysTreat;
    } else if ( strncmp(argv[1], "--sbs", strlen(argv[1])) == 0 ){
        treat = sbsTreat;
    } else {
        raler(0,"Usage : ./fdbg [--sbs | --sys]  <COMMAND TO EXEC>");
    }

    switch(child = fork()){
        case -1:
            raler(1,"error fork");

        case 0: 
            CHK(ptrace(PTRACE_TRACEME,0,NULL,NULL)); // On trace ce qui suit
            int fd;
            CHK(fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0666));

            // On redirige stdout dans tmp
            CHK(close(1));
            CHK(dup(fd));


            execvp(argv[2],&argv[2]);
            exit(EXIT_FAILURE);

    }

    // On ignore le premier syscall qui correspond à l'execvp
    CHK(wait(NULL));

    treat(child,calls);


    freeSyscalls(calls);
    return 0;
}
