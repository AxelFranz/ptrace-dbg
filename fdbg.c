#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <stdarg.h>

#ifdef __x86_64__
#define WORD_SIZE 8
#else
#define WORD_SIZE 4
#endif

#define CHK(op) do { if ((op) == -1) raler (1, #op); } while (0)

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



int main(int argc, char** argv) {
    pid_t child;
    char* const* arg = argv + 1;
    child = fork();
    int orig_rax;

    if(argc == 1) {
        raler(0,"Usage : ./fdbg <COMMAND TO EXEC>");
    }

    if(child == 0){
        ptrace(PTRACE_TRACEME,0,NULL,NULL); // On trace ce qui suit
        int a = 3;
        (void)a;
        execvp(arg[0], &arg[0]);
    } else {
        wait(NULL);
        orig_rax = ptrace(PTRACE_PEEKUSER, child, WORD_SIZE * ORIG_RAX,NULL);
        printf("Le fils a fait un sycall %d\n",orig_rax);
    }

    return 0;
}
