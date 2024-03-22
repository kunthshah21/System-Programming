#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command line arguments are provided
    if (argc != 5 || strcmp(argv[1], "-g") != 0 || strcmp(argv[3], "-c") != 0) {
        printf("Usage: %s -g <generator_command> -c <consumer_command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extract generator and consumer commands from command line arguments
    char *generator_cmd = argv[2];
    char *consumer_cmd = argv[4];

    // Declare pipe file descriptors
    int pipefd[2];
    pid_t pid;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process (data generator)
        // Close unused read end of the pipe
        close(pipefd[0]);
        // Redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Execute the generator command
        if (execlp(generator_cmd, generator_cmd, NULL) == -1) {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
        // Close unused write end of the pipe
        close(pipefd[1]);
        // Redirect stdin to the read end of the pipe
        dup2(pipefd[0], STDIN_FILENO);

        // Execute the consumer command
        if (execlp(consumer_cmd, consumer_cmd, NULL) == -1) {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
