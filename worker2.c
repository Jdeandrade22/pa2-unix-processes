#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Basic check to ensure we have exactly 2 numbers
    if (argc != 3) {
        printf("Error: Worker requires exactly 2 arguments\n");
        exit(1);
    }
    
    // Convert arguments to integers
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int sum = num1 + num2;
    
    // Print as required by the assignment
    printf("%d + %d = %d\n", num1, num2, sum);
    printf("PID of worker is: %d\n", getpid());
    
    // Exit with the sum as status
    exit(sum);
}
