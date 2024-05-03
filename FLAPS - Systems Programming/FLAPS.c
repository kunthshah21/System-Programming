#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#ifndef DT_DIR
#define DT_DIR 4
#endif
#define MAX_LEVEL 10
#define PROC_DIR "/proc"

void universal_Process();
void pid_specific_details(int pid);
void process_tree_printer();
void process_set(int pid);
void process_user_realID(uid_t uid);
void process_user_effectiveID(uid_t uid);

int main(int argc, char *argv[]) {
    int pid = 0;
    uid_t uid = 0;

    if (argc == 1) {
        universal_Process();
    } else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'a') {
        pid = atoi(argv[2]);
        pid_specific_details(pid);
    } else if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 't') {
        process_tree_printer();
    } else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'g') {
        pid = atoi(argv[2]);
        process_set(pid);
    } else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'u') {
        uid = atoi(argv[2]);
        process_user_realID(uid);
    } else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'U') {
        uid = atoi(argv[2]);
        process_user_effectiveID(uid);
    } else {
        printf("Invalid arguments\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

// Function implementations

void universal_Process() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Iterate through each entry in /proc
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            int pid = atoi(entry->d_name);
            if (pid > 0) {
                printf("PID: %d\n", pid);
            }
        }
    }

    closedir(dir);
}
void pid_specific_details(int pid) {
    char path[500];

    // Print process ID
    printf("PID: %d\n", pid);

    // Print command name
    printf("--------- Command Name ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    FILE *cmdline_file_proc = fopen(path, "r"); 
    if (cmdline_file_proc != NULL) {
        char cmdline[500];
        if (fgets(cmdline, sizeof(cmdline), cmdline_file_proc) != NULL) {
            printf("Command Name: %s\n", cmdline);
        }
        fclose(cmdline_file_proc);
    }

    // Open status file to read more information about the process
    printf("\n\n\n--------- Status Information ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *status_file = fopen(path, "r");
    if (status_file != NULL) {
        char line[500];
        while (fgets(line, sizeof(line), status_file) != NULL) {
            printf("%s", line);
        }
        fclose(status_file);
    }

    // Open statm file to read memory information about the process
    printf("\n\n\n--------- Memory Information ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    FILE *statm_file = fopen(path, "r");
    if (statm_file != NULL) {
        int size, resident, share, text, lib, data, dt;
        fscanf(statm_file, "%d %d %d %d %d %d %d", &size, &resident, &share, &text, &lib, &data, &dt);
        printf("Memory Usage: Size: %d KB, Resident: %d KB\n", size, resident);
        fclose(statm_file);
    }

    // Open stat file to read CPU information about the process
    printf("\n\n\n--------- CPU Information ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *stat_file = fopen(path, "r");
    if (stat_file != NULL) {
        int utime, stime;
        fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %d %d", &utime, &stime);
        printf("CPU Usage: User Time: %d, System Time: %d\n", utime, stime);
        fclose(stat_file);
    }

    // Open cmdline file to read command line arguments
    printf("\n\n\n--------- Line Arguments ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    FILE *cmdline_file = fopen(path, "r");
    if (cmdline_file != NULL) {
        char cmdline[500];
        if (fgets(cmdline, sizeof(cmdline), cmdline_file) != NULL) {
            printf("Command Line: %s\n", cmdline);
        }
        fclose(cmdline_file);
    }

    // Open stat file again to read start time and uptime
    printf("\n\n\n--------- Start Time and Uptime ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    stat_file = fopen(path, "r");
    if (stat_file != NULL) {
        unsigned long starttime;
        long uptime;
        unsigned long now = time(NULL);
        int clock_ticks = sysconf(_SC_CLK_TCK);
        fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u %ld", &starttime, &uptime);
        long seconds = now - (starttime / clock_ticks);
        printf("Start Time: %lu seconds ago\n", seconds);
        fclose(stat_file);
    }

    // Open environ file to read environment variables
    printf("\n\n\n--------- Environment Variables ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/environ", pid);
    FILE *environ_file = fopen(path, "r");
    if (environ_file != NULL) {
        char line[500];
        while (fgets(line, sizeof(line), environ_file) != NULL) {
            printf("%s", line);
        }
        fclose(environ_file);
    }


    // Open fd directory to read open file descriptors
    printf("\n\n\n--------- Open File Descriptors ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/fd", pid);
    DIR *fd_dir = opendir(path);
    if (fd_dir != NULL) {
        struct dirent *fd_entry;
        while ((fd_entry = readdir(fd_dir)) != NULL) {
            if (fd_entry->d_type == DT_LNK) {
                printf("File Descriptor: %s\n", fd_entry->d_name);
            }
        }
        closedir(fd_dir);
    }

    // Open status file again to read process group and session information
    printf("\n\n\n--------- Process Group and Session Information ---------\n");
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    status_file = fopen(path, "r");
    if (status_file != NULL) {
        char line[500];
        while (fgets(line, sizeof(line), status_file) != NULL) {
            // Check for process group and session information
            if (strncmp(line, "Uid:", 4) == 0 || strncmp(line, "Gid:", 4) == 0 ||
                strncmp(line, "Groups:", 7) == 0 || strncmp(line, "FDSize:", 7) == 0) {
                printf("%s", line);
            }
        }
        fclose(status_file);
    }
}


void print_process_tree(int pid, int level) {
    if (level > MAX_LEVEL) {
        // Reached the maximum depth of the tree
        return;
    }

    DIR *dir;
    struct dirent *entry;
    char path[500];

    // Open directory corresponding to the process
    snprintf(path, sizeof(path), "/proc/%d/task", pid);
    dir = opendir(path);
    if (dir == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("|-- %d\n", pid);

    // Iterate over the tasks of the process
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        int child_pid = atoi(entry->d_name);
        print_process_tree(child_pid, level + 1);
    }

    closedir(dir);
}

void process_tree_printer() {
    pid_t session_leader = getsid(0); // Get session leader for the current process

    printf("Process Tree for Session Leader (PID: %d):\n", session_leader);
    print_process_tree(session_leader, 0);
}

void process_set(pid_t target_pid) {
    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            pid_t pid = atoi(entry->d_name);

            if (pid == target_pid || pid == getppid())
                continue;

            char stat_file_path[500];
            snprintf(stat_file_path, sizeof(stat_file_path), "/proc/%d/stat", pid);

            FILE *stat_file = fopen(stat_file_path, "r");
            if (stat_file == NULL) {
                perror("fopen");
                return;
            }

            int process_group_id;
            fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d %*u %*u %*d %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %d", &process_group_id);
            fclose(stat_file);

            if (process_group_id == getpgid(target_pid)) {
                printf("Process with PID %d is in the same group as PID %d\n", pid, target_pid);
            }
        }
    }

    closedir(proc_dir);
}

void process_user_realID(uid_t uid)
{
    DIR *dir;
    struct dirent *entry;
    char path[500];
    char line[500];

    dir = opendir("/proc");
    if (!dir)
    {
        perror("Error opening /proc directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0)
        {
            sprintf(path, "/proc/%s/status", entry->d_name);
            FILE *file = fopen(path, "r");
            if (file)
            {
                while (fgets(line, sizeof(line), file))
                {
                    if (strncmp(line, "Uid:", 4) == 0)
                    {
                        uid_t process_uid;
                        sscanf(line, "%*s %u", &process_uid);
                        if (process_uid == uid)
                        {
                            printf("Process ID: %s\n", entry->d_name);
                            break;
                        }
                    }
                }
                fclose(file);
            }
        }
    }
    closedir(dir);
}


void process_user_effectiveID(uid_t uid)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(PROC_DIR);
    if (dir == NULL)
    {
        perror("Unable to open /proc directory");
        exit(EXIT_FAILURE);
    }

    printf("Processes for user with effective UID %d:\n", uid);

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char proc_path[500];
            snprintf(proc_path, sizeof(proc_path), "%s/%s", PROC_DIR, entry->d_name);

            if (atoi(entry->d_name) > 0)
            {
                char status_path[600];
                snprintf(status_path, sizeof(status_path), "%s/status", proc_path);
                FILE *status_file = fopen(status_path, "r");

                if (status_file != NULL)
                {
                    char line[500];
                    uid_t process_uid = 0;

                    while (fgets(line, sizeof(line), status_file))
                    {
                        if (strncmp(line, "Uid:", 4) == 0)
                        {
                            sscanf(line, "Uid:\t%u", &process_uid);
                            break;
                        }
                    }

                    fclose(status_file);

                    if (process_uid == uid)
                    {
                        printf("PID: %s\n", entry->d_name);
                    }
                }
                else
                {
                    perror("Unable to open status file");
                }
            }
        }
    }
    closedir(dir);
}
