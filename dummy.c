#include <unistd.h>

int main(){
    pid_t pid = getpid();
    pid_t ppid = getppid();

    return 0;
}
