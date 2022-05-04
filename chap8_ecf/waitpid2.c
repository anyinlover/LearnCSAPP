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
    pid_t pid[N], retpid;

    for (i = 0; i < N; i++) {
        if ((pid[i] = fork()) < 0) {
            fprintf(stderr, "fork error: %s\n", strerror(errno));
            exit(0);
        }
        if (pid[i] == 0) {
            exit(100+i);
        }
    }

    i = 0;
    while ((retpid = waitpid(pid[i++], &status, 0)) > 0) {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n", retpid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", retpid);
    }

    if (errno != ECHILD) {
        fprintf(stderr, "waitpid error: %s\n", strerror(errno));
        exit(0);
    }
    
    exit(0);
}