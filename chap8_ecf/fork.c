#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int x = 1;

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error: %s\n", strerror(errno));
        exit(0);
    }
    if (pid == 0) {
        printf("child : x=%d\n", ++x);
        exit(0);
    }

    printf("parent: x=%d\n", --x);
    exit(0);
}