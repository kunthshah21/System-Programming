#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

void create_file_or_directory(const char *path, int is_dir); // create file or directory
void delete_file_or_directory(const char *path); // delete file or directory
void rename_file_or_directory(const char *old_path, const char *new_path); // rename file or directory
void display_first_50_bytes(const char *file_path); // display first 50 bytes
void write_50byte_function(const char *file_path, const char *text); // write 50 bytes

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <operation> [arguments]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *operation = argv[1];

    if (strcmp(operation, "-c") == 0) {
        // Create file or directory
        if (argc != 4) {
            printf("Usage: %s -c <file/directory path> <is_dir>\n", argv[0]);
            return EXIT_FAILURE;
        }
        create_file_or_directory(argv[2], atoi(argv[3]));
    } else if (strcmp(operation, "-d") == 0) {
        // Delete file or directory
        if (argc != 3) {
            printf("Usage: %s -d <file/directory path>\n", argv[0]);
            return EXIT_FAILURE;
        }
        delete_file_or_directory(argv[2]);
    } else if (strcmp(operation, "-r") == 0) {
        // Rename file or directory
        if (argc != 4) {
            printf("Usage: %s -r <old path> <new path>\n", argv[0]);
            return EXIT_FAILURE;
        }
        rename_file_or_directory(argv[2], argv[3]);
    } else if (strcmp(operation, "-f") == 0) {
        // Display first 50 bytes
        if (argc != 3) {
            printf("Usage: %s -f <file path>\n", argv[0]);
            return EXIT_FAILURE;
        }
        display_first_50_bytes(argv[2]);
    } else if (strcmp(operation, "-w") == 0) {
        // Write 50 bytes
        if (argc != 4) {
            printf("Usage: %s -w <file path> <text>\n", argv[0]);
            return EXIT_FAILURE;
        }
        write_50byte_function(argv[2], argv[3]);
    } else {
        printf("Invalid operation. Please use one of the following:\n");
        printf("-c : Create file or directory\n");
        printf("-d : Delete file or directory\n");
        printf("-r : Rename file or directory\n");
        printf("-f : Display first 50 bytes\n");
        printf("-w : Write 50 bytes\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void create_file_or_directory(const char *path, int is_dir) {
    if (is_dir) {
        // Check if the directory name has an extension
        if (strchr(path, '.')) {
            printf("Error: Directory name should not have an extension.\n");
            exit(EXIT_FAILURE);
        }

        // Create a directory
        if (mkdir(path, 0777) == -1) {
            printf("MKDIR Error number: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("Successfully created directory %s\n", path);
    } else {
        // Create a file
        int fd = open(path, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            printf("Open Error number: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        close(fd);
        printf("Successfully created file %s.\n", path);
    }
}

void delete_file_or_directory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
        printf("Stat Error number: %d\n", errno);
        // perror("Error getting file information");
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(path_stat.st_mode)) { 
        // It's a directory, use rmdir
        if (rmdir(path) == -1) {
            printf("RMDIR Error number: %d\n", errno);
            // perror("Error deleting directory");
            exit(EXIT_FAILURE);
        }
    } else {
        // It's a file, use unlink
        if (unlink(path) == -1) {
            printf("UNLINK Error number: %d\n", errno);
            // perror("Error deleting file");
            exit(EXIT_FAILURE);
        }
    }

    printf("Successfully deleted %s.\n", path);
}

void rename_file_or_directory(const char *old_path, const char *new_path) {
    struct stat path_stat;
    if (stat(old_path, &path_stat) == -1) {
        printf("STAT Error number: %d\n", errno);
        // perror("Error getting file information");
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(path_stat.st_mode)) {
        // It's a directory
        if (rename(old_path, new_path) == -1) {
            printf("RENAME Error number: %d\n", errno);
            // perror("Error renaming directory");
            exit(EXIT_FAILURE);
        }
        printf("Successfully renamed directory from %s to %s.\n", old_path, new_path);
    } else if (S_ISREG(path_stat.st_mode)) {
        // It's a file
        if (rename(old_path, new_path) == -1) {
            printf("RENAME Error number: %d\n", errno);
            // perror("Error renaming file");
            exit(EXIT_FAILURE);
        }
        printf("Successfully renamed file from %s to %s.\n", old_path, new_path);
    } else {
        printf("Unsupported file type.\n");
        exit(EXIT_FAILURE);
    }
}

void display_first_50_bytes(const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("Open Error number: %d\n", errno);
        // perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[51];
    ssize_t num_read = read(fd, buffer, 50);
    if (num_read == -1) {
        printf("Read Error number: %d\n", errno);
        // perror("Error reading file");
        exit(EXIT_FAILURE);
    }

    buffer[num_read] = '\0';  // Null-terminate the buffer
    printf("First 50 bytes of %s: %s\n", file_path, buffer);

    close(fd);
}

void write_50byte_function(const char *file_path, const char *input) {
    int fd;

    // Open the file in read-write mode
    fd = open(file_path, O_RDWR);

    if (fd == -1) {
        printf("Open Error number: %d\n", errno);
        // perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[51];  // 50 bytes for data + 1 for null terminator

    struct stat file_info;
    if (stat(file_path, &file_info) == -1) {
        printf("Stat Error number: %d\n", errno);
        // perror("Error getting file information");
        close(fd);  // Close the file before exiting
        exit(EXIT_FAILURE);
    }

    if (S_ISREG(file_info.st_mode)) {
        // Regular file
        // Try to detect if it's a text file or binary file
        int is_text = 1;
        char ch;
        FILE *fp = fopen(file_path, "r");
        if (fp == NULL) {
            printf("fopen Error number: %d\n", errno);
            // perror("Error opening file");
            close(fd);  // Close the file before exiting
            exit(EXIT_FAILURE);
        }
        while ((ch = fgetc(fp)) != EOF && is_text) {
            if ((ch < 32 || ch > 126) && ch != '\t' && ch != '\n' && ch != '\r') {
                is_text = 0;
            }
        }
        fclose(fp);

        // Check file extension
        const char *ext = strrchr(file_path, '.');
        if (ext != NULL) {
            if (strcmp(ext, ".txt") == 0 || strcmp(ext, ".c") == 0 || strcmp(ext, ".cpp") == 0 || strcmp(ext, ".py") == 0) {
                is_text = 1;
            } else if (strcmp(ext, ".bin") == 0 || strcmp(ext, ".dat") == 0) {
                is_text = 0;
            }
        }

        // Move the cursor to the end of the file
        lseek(fd, 0, SEEK_END);

        if (is_text) {
            printf("Detected as a text file.\n");

            // Write the text to the end of the file
            write(fd, input, strlen(input));
        } else {
             printf("Detected as a binary file.\n");
             // Convert the provided text to an integer
            int start = atoi(input);
            printf("Start number: %d\n", start);

            if (start > 50 && start < 200) { // check if within range
                // Generate and write 50 bytes of odd numbers starting from the given number
                for (int i = start; i < start + 100 && i < 200 ; i += 2) {
                    char byte = (unsigned char)i;
                    write(fd, &byte, 1);
                } 
            } else {
                printf("Incorrect Input Between [50-200]\n"); 
                close(fd);
                return;
            }
        }
    } else {
        printf("Unsupported file type.\n");
        close(fd);  // Close the file before exiting, if not closed, received an error while performing further actions
        exit(EXIT_FAILURE);
    }

    close(fd);  // Close the file after writing
}
