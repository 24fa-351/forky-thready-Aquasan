// Alex Ngguyen

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  // Help run forky on Windows

void create_process_pattern(const char *pattern, int process_num);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_of_processes> <pattern>\n", argv[0]);
        return 1;
    }

    int         num_processes = atoi(argv[1]);
    const char *pattern = argv[2];  // Get pattern as a string

    // Variable for the number of processes
    if (num_processes < 1 || num_processes > 256) {
        printf("Error: number of processes must be between 1 and 256.\n");
        return 1;
    }

    // random number generator
    srand((unsigned int)time(NULL));
    // Notes to myself:Can not make it work with makefile so this is the
    // alterative method to use Made it where it opens the results.txt file for
    // appending
    FILE *file = fopen("results.txt", "a");
    if (file == NULL) {
        printf("Error opening file results.txt.\n");
        return 1;
    }

    fprintf(file, "** Pattern: %s, creating %d processes\n", pattern,
            num_processes);
    fclose(file);

    // Manually creating processes based on the selected pattern
    for (int i = 0; i < num_processes; i++) {
        create_process_pattern(pattern, i);
    }

    // Log the parent process exit
    file = fopen("results.txt", "a");
    if (file != NULL) {
        fprintf(file, "** Pattern: %s, All processes created\n", pattern);
        fclose(file);
    }

    return 0;
}

void create_process_pattern(const char *pattern, int process_num) {
    STARTUPINFO         si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    int  sleep_time = (rand() % 11) + 1;  // Sleep time between 1 and 11
    char command[256];

    // Create the command string
    snprintf(command, sizeof(command),
             "cmd /c \"timeout /t %d /nobreak >nul 2>&1\"", sleep_time);

    // Create the child process
    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL,
                       NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Open the results.txt file for appending
    FILE *file = fopen("results.txt", "a");
    if (file != NULL) {
        fprintf(file, "Parent: created child process %d (pid %lu)\n",
                process_num, pi.dwProcessId);
        fprintf(file, "Child %d (pid %lu) sleeping for %d seconds\n",
                process_num, pi.dwProcessId, sleep_time);
        fclose(file);
    }

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
