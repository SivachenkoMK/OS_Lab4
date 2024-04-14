#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Please provide a pid to attach to\n");
        return -1;
    }

    pid_t pid = atoi(argv[1]);
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    wait(NULL);

    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("Value at %llx: %d\n", regs.rsp, ptrace(PTRACE_PEEKDATA, pid, regs.rsp, NULL));

    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return 0;
}