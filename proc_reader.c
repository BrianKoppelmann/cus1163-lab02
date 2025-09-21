#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "proc_reader.h"

int is_number(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

int list_process_directories(void) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir("/proc");
    if (!dir) {
        perror("opendir");
        return -1;
    }

    printf("PID\tType\n");
    printf("---\t----\n");

    while ((entry = readdir(dir)) != NULL) {
        if (is_number(entry->d_name)) {
            printf("%s\tprocess\n", entry->d_name);
            count++;
        }
    }

    closedir(dir);
    printf("Found %d process directories\n", count);
    return 0;
}

// === 2. Read process info ===
int read_process_info(const char *pid) {
    char path[256];
    int fd;
    ssize_t bytes_read;
    char buffer[1024];

    // --- status file ---
    snprintf(path, sizeof(path), "/proc/%s/status", pid);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open status");
        return -1;
    }

    printf("\n--- Process Information for PID %s ---\n", pid);

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    close(fd);

    // --- cmdline file ---
    snprintf(path, sizeof(path), "/proc/%s/cmdline", pid);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open cmdline");
        return -1;
    }

    printf("\n--- Command Line ---\n");
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\0') buffer[i] = ' ';
        }
        printf("%s", buffer);
    }
    printf("\n");

    close(fd);
    return 0;
}

// === 3. Show system info ===
int show_system_info(void) {
    const char *files[] = {"/proc/cpuinfo", "/proc/meminfo"};
    const char *names[] = {"CPU Information", "Memory Information"};
    char buffer[256];

    for (int i = 0; i < 2; i++) {
        FILE *f = fopen(files[i], "r");
        if (!f) {
            perror("fopen");
            return -1;
        }

        printf("\n--- %s (first 10 lines) ---\n", names[i]);
        for (int line = 0; line < 10 && fgets(buffer, sizeof(buffer), f); line++) {
            printf("%s", buffer);
        }

        fclose(f);
    }
    return 0;
}

// === 4. Compare file methods ===
void compare_file_methods(void) {
    const char *filename = "/proc/version";
    char buffer[256];
    ssize_t bytes_read;

    printf("Comparing file reading methods for: %s\n", filename);

    // --- Method 1: system calls ---
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    printf("\n=== Method 1: Using System Calls ===\n");
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    close(fd);

    // --- Method 2: library functions ---
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return;
    }

    printf("\n=== Method 2: Using Library Functions ===\n");
    while (fgets(buffer, sizeof(buffer), f)) {
        printf("%s", buffer);
    }
    fclose(f);
}
