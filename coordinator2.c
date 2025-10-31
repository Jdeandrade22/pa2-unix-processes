#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

// Global variables to track child processes
pid_t *all_child_pids = NULL;
int total_children = 0;
int computation_done = 0;

// Signal handler for alarm
void alarm_handler(int sig) {
    if (!computation_done) {
        printf("time is up - worker process terminated\n");
        
        // Kill all child processes
        for (int i = 0; i < total_children; i++) {
            if (all_child_pids[i] > 0) {
                kill(all_child_pids[i], SIGKILL);
            }
        }
        
        // Free memory and exit
        if (all_child_pids) free(all_child_pids);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    // Set up signal handler for alarm
    signal(SIGALRM, alarm_handler);
    
    // Set alarm for 3 seconds
    alarm(3);
    
    // Check if we have at least one number
    if (argc < 2) {
        printf("Usage: %s num1 [num2 ...]\n", argv[0]);
        exit(1);
    }
    
    // Create initial array of numbers from command line arguments
    int count = argc - 1;
    int *numbers = malloc(count * sizeof(int));
    
    // Convert arguments to integers
    for (int i = 0; i < count; i++) {
        numbers[i] = atoi(argv[i + 1]);
    }
    
    // Initialize global child tracking array
    all_child_pids = malloc(100 * sizeof(pid_t));
    total_children = 0;
    computation_done = 0;
    
    // Main algorithm: keep reducing until we have one number
    while (count > 1) {
        // If odd number of elements, add a 0
        if (count % 2 == 1) {
            numbers = realloc(numbers, (count + 1) * sizeof(int));
            numbers[count] = 0;
            count++;
        }
        
        // Array to store child PIDs for current round
        pid_t *child_pids = malloc((count / 2) * sizeof(pid_t));
        int *results = malloc((count / 2) * sizeof(int));
        
        // Create worker processes for each pair
        for (int i = 0; i < count; i += 2) {
            pid_t pid = fork();
            
            if (pid == 0) {
                // Child process - execute worker
                char num1_str[10], num2_str[10];
                sprintf(num1_str, "%d", numbers[i]);
                sprintf(num2_str, "%d", numbers[i + 1]);
                
                execl("./worker2", "worker2", num1_str, num2_str, NULL);
                
                // If execl fails
                perror("execl failed");
                exit(1);
            } else if (pid > 0) {
                // Parent process - store child PID
                child_pids[i / 2] = pid;
                
                // Add to global tracking array
                if (total_children < 100) {
                    all_child_pids[total_children++] = pid;
                }
            } else {
                // Fork failed
                perror("fork failed");
                exit(1);
            }
        }
        
        // Wait for each child immediately after creating it to maintain order
        for (int i = 0; i < count / 2; i++) {
            int status;
            waitpid(child_pids[i], &status, 0);
            
            if (WIFEXITED(status)) {
                results[i] = WEXITSTATUS(status);
                printf("Partial sum in coordinator: %d\n", results[i]);
            } else if (WIFSIGNALED(status)) {
                // Child was killed by signal
                printf("Worker %d was terminated by signal\n", child_pids[i]);
                results[i] = 0;
            }
        }
        
        // Prepare for next round
        free(numbers);
        free(child_pids);
        
        numbers = results;
        count = count / 2;
    }
    
    // Computation completed successfully
    computation_done = 1;
    
    // Cancel the alarm since we finished in time
    alarm(0);
    
    // Final result
    printf("The final sum is %d\n", numbers[0]);
    
    // Clean up
    free(numbers);
    if (all_child_pids) free(all_child_pids);
    
    return 0;
}