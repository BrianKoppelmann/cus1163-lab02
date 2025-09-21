#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_reader.h"

void print_menu() {
    printf("\n=== Process Information Lab ===\n");
    printf("1. List all process directories\n");
    printf("2. Read process information\n");
    printf("3. Show system information\n");
    printf("4. Compare file operation methods\n");
    printf("5. Exit\n");
    printf("Choose an option (1-5): ");
}

int main() {
    int choice;
    char pid_str[16];

    printf("Operating System Interfaces Lab\n");
    printf("This program explores the /proc filesystem interface\n\n");

    while(1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) {
            case 1:
                printf("\nListing all process directories in /proc...\n");
                if(list_process_directories() == 0) {
                    printf("SUCCESS: Process directories listed!\n");
                } else {
                    printf("ERROR: Failed to list process directories\n");
                }
                break;

            case 2:
                printf("Enter process ID (PID) to examine: ");
                fgets(pid_str, sizeof(pid_str), stdin);
                pid_str[strcspn(pid_str, "\n")] = 0; // remove newline

                printf("\nReading information for PID %s...\n", pid_str);
                if(read_process_info(pid_str) == 0) {
                    printf("SUCCESS: Process information read!\n");
                } else {
                    printf("ERROR: Failed to read process information\n");
                }
                break;

            case 3:
                printf("\nReading system information...\n");
                if(show_system_info() == 0) {
                    printf("SUCCESS: System information displayed!\n");
                } else {
                    printf("ERROR: Failed to read system information\n");
                }
                break;

            case 4:
                printf("\nComparing file operation methods...\n");
                compare_file_methods();
                break;

            case 5:
                printf("Exiting program. Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
