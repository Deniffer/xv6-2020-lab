#include "kernel/types.h"
#include "user/user.h"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf( "Usage: primes <n>\n");
        exit(1);
    }

    int limit = atoi(argv[1]);
    int primes_pipe[2];

    // Create the first process in the pipeline
    if (pipe(primes_pipe) == -1) {
        printf("Error creating pipe\n");
        exit(1);
    }

    int pid = fork();
    if (pid == 0) {
        // First process generates all prime numbers between 2 and limit
        close(primes_pipe[READ_END]);

        for (int i = 2; i <= limit; i++) {
            int is_prime = 1;
            for (int j = 2; j * j <= i; j++) {
                if (i % j == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                write(primes_pipe[WRITE_END], &i, sizeof(int));
            }
        }

        close(primes_pipe[WRITE_END]);
        exit(0);
    }

    // Create the rest of the processes in the pipeline
    int p;
    for (p = 2; p * p <= limit; p++) {
        int p_pipe[2];
        if (pipe(p_pipe) == -1) {
            printf("Error creating pipe\n");
            exit(1);
        }

        int p_pid = fork();
        if (p_pid == 0) {
            close(p_pipe[WRITE_END]);
            close(primes_pipe[READ_END]);

            int prime;
            read(primes_pipe[READ_END], &prime, sizeof(int));
            while (prime != -1) {
                if (prime % p != 0) {
                    write(p_pipe[WRITE_END], &prime, sizeof(int));
                }
                read(primes_pipe[READ_END], &prime, sizeof(int));
            }

            close(p_pipe[READ_END]);
            close(primes_pipe[READ_END]);
            exit(0);
        }

        close(primes_pipe[WRITE_END]);
        primes_pipe[WRITE_END] = p_pipe[WRITE_END];
        primes_pipe[READ_END] = p_pipe[READ_END];
    }

    // Last process outputs the remaining prime numbers
    close(primes_pipe[WRITE_END]);
    int prime;
    read(primes_pipe[READ_END], &prime, sizeof(int));
    while (prime != -1) {
        printf("%d ", prime);
        read(primes_pipe[READ_END], &prime, sizeof(int));
    }
    printf("\n");

    // Signal the end of the pipeline
    for (int i = 2; i <= p; i++) {
        write(primes_pipe[WRITE_END], &(i), sizeof(int));
    }

    wait(0);
    exit(0);
}
