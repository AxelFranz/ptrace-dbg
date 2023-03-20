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
#include <sys/syscall.h>

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

const char* calls[314] = {
	"READ",
	"WRITE",
	"OPEN",
	"CLOSE",
	"STAT",
	"FSTAT",
	"LSTAT",
	"POLL",
	"LSEEK",
	"MMAP",
	"MPROTECT",
	"MUNMAP",
	"BRK",
	"RT_SIGACTION",
	"RT_SIGPROCMASK",
	"RT_SIGRETURN",
	"IOCTL",
	"PREAD64",
	"PWRITE64",
	"READV",
	"WRITEV",
	"ACCESS",
	"PIPE",
	"SELECT",
	"SCHED_YIELD",
	"MREMAP",
	"MSYNC",
	"MINCORE",
	"MADVISE",
	"SHMGET",
	"SHMAT",
	"SHMCTL",
	"DUP",
	"DUP2",
	"PAUSE",
	"NANOSLEEP",
	"GETITIMER",
	"ALARM",
	"SETITIMER",
	"GETPID",
	"SENDFILE",
	"SOCKET",
	"CONNECT",
	"ACCEPT",
	"SENDTO",
	"RECVFROM",
	"SENDMSG",
	"RECVMSG",
	"SHUTDOWN",
	"BIND",
	"LISTEN",
	"GETSOCKNAME",
	"GETPEERNAME",
	"SOCKETPAIR",
	"SETSOCKOPT",
	"GETSOCKOPT",
	"CLONE",
	"FORK",
	"VFORK",
	"EXECVE",
	"EXIT",
	"WAIT4",
	"KILL",
	"UNAME",
	"SEMGET",
	"SEMOP",
	"SEMCTL",
	"SHMDT",
	"MSGGET",
	"MSGSND",
	"MSGRCV",
	"MSGCTL",
	"FCNTL",
	"FLOCK",
	"FSYNC",
	"FDATASYNC",
	"TRUNCATE",
	"FTRUNCATE",
	"GETDENTS",
	"GETCWD",
	"CHDIR",
	"FCHDIR",
	"RENAME",
	"MKDIR",
	"RMDIR",
	"CREAT",
	"LINK",
	"UNLINK",
	"SYMLINK",
	"READLINK",
	"CHMOD",
	"FCHMOD",
	"CHOWN",
	"FCHOWN",
	"LCHOWN",
	"UMASK",
	"GETTIMEOFDAY",
	"GETRLIMIT",
	"GETRUSAGE",
	"SYSINFO",
	"TIMES",
	"PTRACE",
	"GETUID",
	"SYSLOG",
	"GETGID",
	"SETUID",
	"SETGID",
	"GETEUID",
	"GETEGID",
	"SETPGID",
	"GETPPID",
	"GETPGRP",
	"SETSID",
	"SETREUID",
	"SETREGID",
	"GETGROUPS",
	"SETGROUPS",
	"SETRESUID",
	"GETRESUID",
	"SETRESGID",
	"GETRESGID",
	"GETPGID",
	"SETFSUID",
	"SETFSGID",
	"GETSID",
	"CAPGET",
	"CAPSET",
	"RT_SIGPENDING",
	"RT_SIGTIMEDWAIT",
	"RT_SIGQUEUEINFO",
	"RT_SIGSUSPEND",
	"SIGALTSTACK",
	"UTIME",
	"MKNOD",
	"USELIB",
	"PERSONALITY",
	"USTAT",
	"STATFS",
	"FSTATFS",
	"SYSFS",
	"GETPRIORITY",
	"SETPRIORITY",
	"SCHED_SETPARAM",
	"SCHED_GETPARAM",
	"SCHED_SETSCHEDULER",
	"SCHED_GETSCHEDULER",
	"SCHED_GET_PRIORITY_MAX",
	"SCHED_GET_PRIORITY_MIN",
	"SCHED_RR_GET_INTERVAL",
	"MLOCK",
	"MUNLOCK",
	"MLOCKALL",
	"MUNLOCKALL",
	"VHANGUP",
	"MODIFY_LDT",
	"PIVOT_ROOT",
	"_SYSCTL",
	"PRCTL",
	"ARCH_PRCTL",
	"ADJTIMEX",
	"SETRLIMIT",
	"CHROOT",
	"SYNC",
	"ACCT",
	"SETTIMEOFDAY",
	"MOUNT",
	"UMOUNT2",
	"SWAPON",
	"SWAPOFF",
	"REBOOT",
	"SETHOSTNAME",
	"SETDOMAINNAME",
	"IOPL",
	"IOPERM",
	"CREATE_MODULE",
	"INIT_MODULE",
	"DELETE_MODULE",
	"GET_KERNEL_SYMS",
	"QUERY_MODULE",
	"QUOTACTL",
	"NFSSERVCTL",
	"GETPMSG",
	"PUTPMSG",
	"AFS_SYSCALL",
	"TUXCALL",
	"SECURITY",
	"GETTID",
	"READAHEAD",
	"SETXATTR",
	"LSETXATTR",
	"FSETXATTR",
	"GETXATTR",
	"LGETXATTR",
	"FGETXATTR",
	"LISTXATTR",
	"LLISTXATTR",
	"FLISTXATTR",
	"REMOVEXATTR",
	"LREMOVEXATTR",
	"FREMOVEXATTR",
	"TKILL",
	"TIME",
	"FUTEX",
	"SCHED_SETAFFINITY",
	"SCHED_GETAFFINITY",
	"SET_THREAD_AREA",
	"IO_SETUP",
	"IO_DESTROY",
	"IO_GETEVENTS",
	"IO_SUBMIT",
	"IO_CANCEL",
	"GET_THREAD_AREA",
	"LOOKUP_DCOOKIE",
	"EPOLL_CREATE",
	"EPOLL_CTL_OLD",
	"EPOLL_WAIT_OLD",
	"REMAP_FILE_PAGES",
	"GETDENTS64",
	"SET_TID_ADDRESS",
	"RESTART_SYSCALL",
	"SEMTIMEDOP",
	"FADVISE64",
	"TIMER_CREATE",
	"TIMER_SETTIME",
	"TIMER_GETTIME",
	"TIMER_GETOVERRUN",
	"TIMER_DELETE",
	"CLOCK_SETTIME",
	"CLOCK_GETTIME",
	"CLOCK_GETRES",
	"CLOCK_NANOSLEEP",
	"EXIT_GROUP",
	"EPOLL_WAIT",
	"EPOLL_CTL",
	"TGKILL",
	"UTIMES",
	"VSERVER",
	"MBIND",
	"SET_MEMPOLICY",
	"GET_MEMPOLICY",
	"MQ_OPEN",
	"MQ_UNLINK",
	"MQ_TIMEDSEND",
	"MQ_TIMEDRECEIVE",
	"MQ_NOTIFY",
	"MQ_GETSETATTR",
	"KEXEC_LOAD",
	"WAITID",
	"ADD_KEY",
	"REQUEST_KEY",
	"KEYCTL",
	"IOPRIO_SET",
	"IOPRIO_GET",
	"INOTIFY_INIT",
	"INOTIFY_ADD_WATCH",
	"INOTIFY_RM_WATCH",
	"MIGRATE_PAGES",
	"OPENAT",
	"MKDIRAT",
	"MKNODAT",
	"FCHOWNAT",
	"FUTIMESAT",
	"NEWFSTATAT",
	"UNLINKAT",
	"RENAMEAT",
	"LINKAT",
	"SYMLINKAT",
	"READLINKAT",
	"FCHMODAT",
	"FACCESSAT",
	"PSELECT6",
	"PPOLL",
	"UNSHARE",
	"SET_ROBUST_LIST",
	"GET_ROBUST_LIST",
	"SPLICE",
	"TEE",
	"SYNC_FILE_RANGE",
	"VMSPLICE",
	"MOVE_PAGES",
	"UTIMENSAT",
	"EPOLL_PWAIT",
	"SIGNALFD",
	"TIMERFD_CREATE",
	"EVENTFD",
	"FALLOCATE",
	"TIMERFD_SETTIME",
	"TIMERFD_GETTIME",
	"ACCEPT4",
	"SIGNALFD4",
	"EVENTFD2",
	"EPOLL_CREATE1",
	"DUP3",
	"PIPE2",
	"INOTIFY_INIT1",
	"PREADV",
	"PWRITEV",
	"RT_TGSIGQUEUEINFO",
	"PERF_EVENT_OPEN",
	"RECVMMSG",
	"FANOTIFY_INIT",
	"FANOTIFY_MARK",
	"PRLIMIT64",
	"NAME_TO_HANDLE_AT",
	"OPEN_BY_HANDLE_AT",
	"CLOCK_ADJTIME",
	"SYNCFS",
	"SENDMMSG",
	"SETNS",
	"GETCPU",
	"PROCESS_VM_READV",
	"PROCESS_VM_WRITEV",
	"KCMP",
	"FINIT_MODULE",
};

#define CHK(op) do { if ((op) == -1) raler (1, #op); } while (0)

#define WORD_SIZE 8


int main(int argc, char** argv) {
    pid_t child;
    int orig_rax; 
    int status = 0;
    int isReturn = 0;
    if(argc == 1) {
        raler(0,"Usage : ./fdbg <COMMAND TO EXEC>");
    }

    switch(child = fork()){
        case -1:
            raler(0,"error fork");

        case 0: 
            printf("Je suis le fils et je suis tracé avec la commande ls\n");
            CHK(ptrace(PTRACE_TRACEME,0,NULL,NULL)); // On trace ce qui suit
            execvp(argv[1],&argv[1]);
            exit(EXIT_FAILURE);

    }

    // On ignore le premier syscall qui correspond à l'execvp
    CHK(wait(NULL));
    CHK(ptrace(PTRACE_SYSCALL, child, NULL, NULL));

    while(1){
        wait(&status);
        if(WIFEXITED(status)){
            break;
        }
        
        CHK(orig_rax = ptrace(PTRACE_PEEKUSER, child, WORD_SIZE * ORIG_RAX,NULL));

        if(!isReturn){
            printf("Syscall number %d : %s\n",orig_rax, calls[orig_rax] );
        }
        isReturn = (isReturn + 1) % 2;
        CHK(ptrace(PTRACE_SYSCALL, child, NULL, NULL));


    }

    return 0;
}
