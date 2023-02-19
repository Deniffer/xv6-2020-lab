#include "kernel/types.h"
#include "user/user.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

void sieve(int pipe_left[2]) {
    int prime,pipe_right[2];
    close(pipe_left[PIPE_WRITE]);
    if (read(pipe_left[PIPE_READ], &prime, sizeof(int)) == 0) {
        // have nothing to read
        exit(0);
    }
    pipe(pipe_right);

    if (fork() == 0) {
        sieve(pipe_right);
    } else {
        close(pipe_right[PIPE_READ]);
        printf("prime %d\n", prime);
        int num = prime;
        while(read(pipe_left[PIPE_READ], &num, sizeof(int))) {
            if (num % prime != 0) {
                write(pipe_right[PIPE_WRITE], &num, sizeof(int));
            }
        }
        close(pipe_right[PIPE_WRITE]);
        wait((int *)0);
        exit(0);
    }

}


int main (int argc, char *argv[]) {
    
    int pipe_left[2];
    pipe(pipe_left);
    if (fork() == 0) {
        sieve(pipe_left);
    } else {
        close(pipe_left[PIPE_READ]);
        for (int i=2;i<35;i++) {
            write(pipe_left[PIPE_WRITE], &i, sizeof(int));
        }
        close(pipe_left[PIPE_WRITE]);
        wait((int *)0);
    }
    exit(0);
}