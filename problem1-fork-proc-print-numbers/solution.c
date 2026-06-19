#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int rank = 1;

    while (rank < N) {
        if (rank == 1)
            printf("%d", rank);
        else
            printf(" %d", rank);
        fflush(stdout);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid > 0) {
            waitpid(pid, NULL, 0);
            return 0;
        }
        rank++;
    }

    if (N == 1)
        printf("1\n");
    else
        printf(" %d\n", rank);
    fflush(stdout);

    return 0;
}
