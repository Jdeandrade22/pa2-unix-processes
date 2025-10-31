// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <sys/types.h>
// #include <string.h>

// int main(int argc, char *argv[]) {
//     // Check if we have at least one number
//     if (argc < 2) {
//         printf("Usage: %s num1 [num2 ...]\n", argv[0]);
//         exit(1);
//     }
    
//     // Create initial array of numbers from command line arguments
//     int count = argc - 1;
//     int *numbers = malloc(count * sizeof(int));
    
//     // Convert arguments to integers
//     for (int i = 0; i < count; i++) {
//         numbers[i] = atoi(argv[i + 1]);
//     }
    
//     // Main algorithm: keep reducing until we have one number
//     while (count > 1) {
//         printf("Current round with %d numbers: ", count);
//         for (int i = 0; i < count; i++) {
//             printf("%d ", numbers[i]);
//         }
//         printf("\n");
        
//         // If odd number of elements, add a 0
//         if (count % 2 == 1) {
//             numbers = realloc(numbers, (count + 1) * sizeof(int));
//             numbers[count] = 0;
//             count++;
//             printf("Added zero, now have %d numbers\n", count);
//         }
        
//         // Array to store child PIDs and results
//         pid_t *child_pids = malloc((count / 2) * sizeof(pid_t));
//         int *results = malloc((count / 2) * sizeof(int));
        
//         // Create worker processes for each pair
//         for (int i = 0; i < count; i += 2) {
//             pid_t pid = fork();
            
//             if (pid == 0) {
//                 // Child process - execute worker
//                 char num1_str[10], num2_str[10];
//                 sprintf(num1_str, "%d", numbers[i]);
//                 sprintf(num2_str, "%d", numbers[i + 1]);
                
//                 execl("./worker", "worker", num1_str, num2_str, NULL);
                
//                 // If execl fails
//                 perror("execl failed");
//                 exit(1);
//             } else if (pid > 0) {
//                 // Parent process - store child PID
//                 child_pids[i / 2] = pid;
//             } else {
//                 // Fork failed
//                 perror("fork failed");
//                 exit(1);
//             }
//         }
        
//         // Wait for all children to complete and collect results
//         for (int i = 0; i < count / 2; i++) {
//             int status;
//             waitpid(child_pids[i], &status, 0);
            
//             if (WIFEXITED(status)) {
//                 results[i] = WEXITSTATUS(status);
//                 printf("Partial sum in coordinator: %d\n", results[i]);
//             } else {
//                 results[i] = 0; // Default if child failed
//             }
//         }
        
//         // Prepare for next round
//         free(numbers);
//         free(child_pids);
        
//         numbers = results;
//         count = count / 2;
        
//         printf("End of round, now have %d numbers\n\n", count);
//     }
    
//     // Final result
//     printf("The final sum is %d\n", numbers[0]);
//     free(numbers);
    
//     return 0;
// }