#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *executable = NULL;
    char *options = NULL;
    int background = 0;

    // Parsing command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "e:o:bf")) != -1) {
        switch (opt) { // switch between different inputs
            case 'e':
                executable = optarg;
                break;
            case 'o':
                options = optarg;
                break;
            case 'b':
                background = 1;
                break;
            case 'f':
                background = 0;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s -e <executable> [-o \"options\"] [-b | -f]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Error handling for missing executable option
    if (executable == NULL) {
        fprintf(stderr, "Error: Missing required option -e <executable>\n");
        exit(EXIT_FAILURE);
    }

    // Building the command to execute
    char command[1024]; // helps prevent overflow
    snprintf(command, sizeof(command), "%s %s", executable, (options == NULL) ? "" : options);

    // Forking to create child process
    pid_t pid = fork();

    if (pid == -1) { // Error in forking
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        // Executing the command
        int ret = system(command);
        exit(ret);
    } else { // Parent process
        if (!background) {
            // Wait for the child to finish if running in the foreground
            int status;
            waitpid(pid, &status, 0);
            // Print command line, process ID, exit code of the child, and any error codes
            printf("Command: %s\n", command);
            printf("Child PID: %d\n", pid);
            if (WIFEXITED(status))
                printf("Exit Code: %d\n", WEXITSTATUS(status));
        } else {
            // Print command line and process ID if running in the background
            printf("Command: %s\n", command);
            printf("Child PID: %d (Running in background)\n", pid);
        }
    }
    return 0;
}
