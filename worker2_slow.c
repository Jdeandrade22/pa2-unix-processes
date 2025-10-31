#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Simulate slow computation - sleep for 5 seconds
    sleep(5);
    
    // Convert arguments to integers
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int sum = num1 + num2;
    
    printf("%d + %d = %d\n", num1, num2, sum);
    printf("PID of worker is: %d\n", getpid());
    
    exit(sum);
}
