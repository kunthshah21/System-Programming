#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_TEXT_LENGTH 50
#define MAX_BYTES_TO_DISPLAY 50
#define MAX_PATH_LENGTH 1024     // Define MAX_PATH_LENGTH here
#define LOG_FILE "my_fm_log.txt" // Log file name

const char *dir_path = ".";

// Function prototypes
void print_usage();
void create_file_or_directory(const char *filename, const char *operation,
                              const char *arg);
void print_first_50_bytes(const char *filename);
void delete_file_or_directory(const char *filename);
void rename_file_or_directory(const char *old_name, const char *new_name);
void append_text(FILE *file, const char *text);
void append_even_numbers(FILE *file, int start);
bool is_binary(const char *filename);
void process_files_recursive(const char *dir_name, const char *filename,
                             const char *arg, const char *operation);
void search_file_recursive(const char *dir_name, const char *operation,
                           const char *filename, const char *arg);
void recursively_append_text(const char *dir_path, const char *filename,
                             const char *text);
void recursively_append_even_numbers(const char *dir_path, const char *filename,
                                     int start);
void log_operation(const char *operation, const char *filename);

// Main function
int main(int argc, char *argv[]) {
  // Open the log file in append mode
  FILE *log_file = fopen(LOG_FILE, "a");
  if (log_file == NULL) {
    perror("Error opening log file");
    return 1;
  }

  if (argc < 3) {
    print_usage();
    fclose(log_file);
    return 1;
  }

  char *filename = argv[2];
  char *operation = argv[1];

  // Log the operation
  log_operation(operation, filename);

  // Handle different operations
  if (strcmp(operation, "-c") == 0) {
    if (argc >= 4) {
      create_file_or_directory(filename, argv[3], argv[4]);
    } else {
      create_file_or_directory(filename, NULL, NULL);
    }
  } else if (strcmp(operation, "-p") == 0) {
    print_first_50_bytes(filename);
  } else if (strcmp(operation, "-d") == 0) {
    delete_file_or_directory(filename);
  } else if (strcmp(operation, "-r") == 0) {
    if (argc >= 4) {
      rename_file_or_directory(filename, argv[3]);
    } else {
      fprintf(log_file, "Error: New filename not provided for renaming.\n");
      fclose(log_file);
      return 1;
    }
  } else if (strcmp(operation, "-s") == 0 || strcmp(operation, "-e") == 0) {
    if (argc >= 4) {
      process_files_recursive(".", filename, argv[3], operation);
    } else {
      fprintf(log_file, "Error: Text or start point not provided.\n");
      fclose(log_file);
      return 1;
    }
  } else {
    print_usage();
    fclose(log_file);
    return 1;
  }

  // Close the log file
  fclose(log_file);

  return 0;
}

// Function definitions

void log_operation(const char *operation, const char *filename) {
  // Open the log file in append mode
  FILE *log_file = fopen(LOG_FILE, "a");
  if (log_file != NULL) {
    fprintf(log_file, "[%s] %s\n", operation, filename);
    fclose(log_file);
  } else {
    perror("Error opening log file");
  }
}

void print_usage() {
  printf("Usage: my_fm [option] [filename]\n");
  printf("Options:\n");
  printf("  -c        Create a file or directory\n");
  printf("  -s TEXT   Create a text file and append given text\n");
  printf("  -e START  Create a binary file and append odd numbers\n");
  printf("  -p        Print the first 50 bytes of the file\n");
  printf("  -d        Delete the file or an empty directory\n");
  printf("  -r NEW    Rename the file or directory\n");
}

void create_file_or_directory(const char *filename, const char *operation,
                              const char *arg) {
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  if (operation != NULL && strcmp(operation, "-s") == 0) {
    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
      perror("Error creating text file");
      exit(1);
    }
    if (arg != NULL) {
      recursively_append_text(dir_path, filename, arg);
    }
    fclose(file);
    printf("Text file created: %s\n", filename);
  } else if (operation != NULL && strcmp(operation, "-e") == 0) {
    FILE *file = fopen(filename, "wb+");
    if (file == NULL) {
      perror("Error creating binary file");
      exit(1);
    }
    if (arg != NULL) {
      int start = atoi(arg);
      append_even_numbers(file, start);
      recursively_append_even_numbers(dir_path, filename, start);
    }
    fclose(file);
    printf("Binary file created: %s\n", filename);
  } else {
    if (mkdir(filename, mode) == 0) {
      printf("Directory created: %s\n", filename);
    } else {
      printf("File created: %s\n", filename);
    }
  }
}

void print_first_50_bytes(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  printf("First 50 bytes of the file:\n");
  int byte;
  for (int i = 0; i < MAX_BYTES_TO_DISPLAY && (byte = fgetc(file)) != EOF;
       ++i) {
    printf("%d ", byte);
  }
  printf("\n");
  fclose(file);
}

void delete_file_or_directory(const char *filename) {
  search_file_recursive(".", "-d", filename, NULL);
  if (remove(filename) == 0) {
    printf("File or directory deleted: %s\n", filename);
  } else {
    perror("Error deleting file or directory");
  }
}

void rename_file_or_directory(const char *old_name, const char *new_name) {
  search_file_recursive(".", "-r", old_name, new_name);
  if (rename(old_name, new_name) == 0) {
    printf("File or directory renamed from %s to %s\n", old_name, new_name);
  } else {
    perror("Error renaming file or directory");
  }
}

void append_text(FILE *file, const char *text) { // single file
  fseek(file, 0, SEEK_END);
  fwrite(text, sizeof(char), strlen(text), file);
}

void append_even_numbers(FILE *file, int start) { // single file
  if (start < 51 || start > 199) {
    perror("Error in starting number");
    return;
  }
  if (start % 2 != 0) {
    start++;
  }
  for (int i = start; i < 200; i += 2) {
    fwrite(&i, sizeof(int), 1, file);
  }
}

bool is_binary(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return false;
  }

  int byte;
  while ((byte = fgetc(file)) != EOF) {
    if (byte == 0 || (byte >= 1 && byte <= 8) || (byte >= 14 && byte <= 31) ||
        byte == 127) {
      fclose(file);
      return true;
    }
  }

  fclose(file);
  return false;
}

void process_files_recursive(const char *dir_name, const char *filename,
                             const char *arg, const char *operation) {
  DIR *dir = opendir(dir_name);
  if (dir == NULL) {
    perror("Error opening directory");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char path[MAX_FILENAME_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

    struct stat st;
    if (stat(path, &st) == 0) {
      if (S_ISDIR(st.st_mode)) {
        // Recursively process subdirectories
        process_files_recursive(path, filename, arg, operation);
      } else if (S_ISREG(st.st_mode)) {
        // Process regular files
        if (strcmp(entry->d_name, filename) == 0) {
          if (strcmp(operation, "-s") == 0) {
            FILE *file = fopen(path, "a");
            if (file == NULL) {
              perror("Error opening file for append");
            } else {
              fprintf(file, "%s", arg);
              fclose(file);
              printf("Text appended to file: %s\n", path);
            }
          } else if (strcmp(operation, "-e") == 0) {
            FILE *file = fopen(path, "ab");
            if (file == NULL) {
              perror("Error opening file for append");
            } else {
              int start = atoi(arg);
              append_even_numbers(file, start);
              fclose(file);
              printf("Even numbers appended to binary file: %s\n", path);
            }
          }
        }
      }
    }
  }

  closedir(dir);
}

void search_file_recursive(const char *dir_name, const char *operation,
                           const char *filename, const char *arg) {
  DIR *dir = opendir(dir_name);
  if (dir == NULL) {
    perror("Error opening directory");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char path[MAX_FILENAME_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

    if (strcmp(entry->d_name, filename) == 0) {
      if (strcmp(operation, "-d") == 0) {
        delete_file_or_directory(path);
      } else if (strcmp(operation, "-r") == 0) {
        char new_path[MAX_FILENAME_LENGTH];
        snprintf(new_path, sizeof(new_path), "%s/%s", dir_name, arg);

        if (rename(path, new_path) == 0) {
          printf("File or directory renamed from %s to %s\n", path, new_path);
        } else {
          perror("Error renaming file or directory");
        }
      } else {
        printf("Invalid operation\n");
      }
    }

    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
      search_file_recursive(path, operation, filename, arg);
    }
  }

  closedir(dir);
}

void recursively_append_text(const char *dir_path, const char *filename,
                             const char *text) {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(dir_path)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type == DT_REG && strcmp(ent->d_name, filename) == 0) {
        char filepath[MAX_PATH_LENGTH];
        snprintf(filepath, MAX_PATH_LENGTH, "%s/%s", dir_path, filename);
        FILE *file = fopen(filepath, "a"); // Open the file
        if (file != NULL) {
          append_text(file,
                      text); // Pass the FILE* pointer to append_text function
          fclose(file);      // Close the file after appending text
        } else {
          perror("Failed to open file for append");
        }
      } else if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 &&
                 strcmp(ent->d_name, "..") != 0) {
        char subdir[MAX_PATH_LENGTH];
        snprintf(subdir, MAX_PATH_LENGTH, "%s/%s", dir_path, ent->d_name);
        recursively_append_text(subdir, filename, text);
      }
    }
    closedir(dir);
  } else {
    perror("Failed to open directory");
  }
}

void recursively_append_even_numbers(const char *dir_path, const char *filename,
                                     int start) {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(dir_path)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type == DT_REG && strcmp(ent->d_name, filename) == 0) {
        char filepath[MAX_PATH_LENGTH];
        snprintf(filepath, MAX_PATH_LENGTH, "%s/%s", dir_path, filename);
        FILE *file =
            fopen(filepath, "ab"); // Open the file in binary append mode
        if (file != NULL) {
          append_even_numbers(file,
                              start); // Pass the FILE* pointer and the starting
                                      // number to append_even_numbers function
          fclose(file); // Close the file after appending even numbers
        } else {
          perror("Failed to open file for append");
        }
      } else if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 &&
                 strcmp(ent->d_name, "..") != 0) {
        char subdir[MAX_PATH_LENGTH];
        snprintf(subdir, MAX_PATH_LENGTH, "%s/%s", dir_path, ent->d_name);
        recursively_append_even_numbers(subdir, filename, start);
      }
    }
    closedir(dir);
  } else {
    perror("Failed to open directory");
  }
}
