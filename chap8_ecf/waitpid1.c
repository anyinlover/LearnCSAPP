#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define N 2

int main()
{
    int status, i;
    pid_t pid;

    for (i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error: %s\n", strerror(errno));
            exit(0);
        }
        if (pid == 0) {
            exit(100+i);
        }
    }

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n", pid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", pid);
    }

    if (errno != ECHILD) {
        fprintf(stderr, "waitpid error: %s\n", strerror(errno));
        exit(0);
    }
    
    exit(0);
}