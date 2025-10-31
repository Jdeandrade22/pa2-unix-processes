#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Pcb {
    char id;
    int estTime;
    int remainTime;
    int waitTime;
    int turnaroundTime;
    struct Pcb *next;
} Pcb_t;

typedef struct {
    char id;
    Pcb_t *first;
    Pcb_t *last;
} Queue_t;

// Function to print a single PCB
void printPCB(Pcb_t *pcb) {
    if (pcb == NULL) {
        printf("PCB is NULL\n");
        return;
    }
    printf("PCB ID: %c Est. Time: %d Remaining Time: %d Total Wait Time: %d\n", 
           pcb->id, pcb->estTime, pcb->remainTime, pcb->waitTime);
}

// Function to add PCB to end of queue
void enqueue(Pcb_t *pcb, Queue_t *queue) {
    if (pcb == NULL || queue == NULL) return;
    
    pcb->next = NULL;
    
    if (queue->first == NULL) {
        queue->first = pcb;
        queue->last = pcb;
    } else {
        queue->last->next = pcb;
        queue->last = pcb;
    }
}

// Function to remove PCB from front of queue
Pcb_t* dequeueFront(Queue_t *queue) {
    if (queue == NULL || queue->first == NULL) return NULL;
    
    Pcb_t *front = queue->first;
    queue->first = front->next;
    
    if (queue->last == front) {
        queue->last = NULL;
    }
    
    front->next = NULL;
    return front;
}

// Function to remove a specific PCB from queue
void dequeue(Queue_t *queue, Pcb_t *pcb) {
    if (queue == NULL || pcb == NULL || queue->first == NULL) return;
    
    if (queue->first == pcb) {
        queue->first = pcb->next;
        if (queue->last == pcb) {
            queue->last = NULL;
        }
        return;
    }
    
    Pcb_t *current = queue->first;
    while (current != NULL && current->next != pcb) {
        current = current->next;
    }
    
    if (current != NULL) {
        current->next = pcb->next;
        if (queue->last == pcb) {
            queue->last = current;
        }
    }
}

// Function to print all PCBs in a queue
void printQ(Queue_t *queue) {
    if (queue == NULL) return;
    
    if (queue->first == NULL) {
        printf("Queue is empty\n");
        return;
    }
    
    Pcb_t *current = queue->first;
    while (current != NULL) {
        printPCB(current);
        current = current->next;
    }
}

// Function to increment wait time for all PCBs in a queue
void incrementWaitTime(Queue_t *queue) {
    Pcb_t *current = queue->first;
    while (current != NULL) {
        current->waitTime++;
        current = current->next;
    }
}

// Test function for Step 1
void testStep1() {
    printf("*** Beginning Step 1 ***\n");
    
    // Create three PCBs
    printf("--- Create three PCBs\n");
    Pcb_t *pcbA = malloc(sizeof(Pcb_t));
    pcbA->id = 'A';
    pcbA->estTime = 6;
    pcbA->remainTime = 6;
    pcbA->waitTime = 0;
    pcbA->next = NULL;
    printPCB(pcbA);
    
    Pcb_t *pcbB = malloc(sizeof(Pcb_t));
    pcbB->id = 'B';
    pcbB->estTime = 2;
    pcbB->remainTime = 2;
    pcbB->waitTime = 0;
    pcbB->next = NULL;
    printPCB(pcbB);
    
    Pcb_t *pcbC = malloc(sizeof(Pcb_t));
    pcbC->id = 'C';
    pcbC->estTime = 3;
    pcbC->remainTime = 3;
    pcbC->waitTime = 0;
    pcbC->next = NULL;
    printPCB(pcbC);
    
    // Create ready queue
    printf("--- Define ready queue\n");
    Queue_t Rqueue;
    Rqueue.id = 'R';
    Rqueue.first = NULL;
    Rqueue.last = NULL;
    
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // Enqueue PCBs
    printf("--- enqueue PCBs\n");
    enqueue(pcbA, &Rqueue);
    enqueue(pcbB, &Rqueue);
    enqueue(pcbC, &Rqueue);
    
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // Remove middle PCB (B)
    printf("--- Remove the middle PCB from the R queue\n");
    dequeue(&Rqueue, pcbB);
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // Remove last PCB (C)
    printf("--- Remove the last PCB from the R queue\n");
    dequeue(&Rqueue, pcbC);
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // Remove remaining PCB (A)
    printf("--- Remove the remaining PCB from the R queue\n");
    dequeue(&Rqueue, pcbA);
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    printf("*** End Step 1 ***\n");
    
    free(pcbA);
    free(pcbB);
    free(pcbC);
}

// Step 2: FCFS Scheduler Simulation
void runScheduler() {
    printf("**** Start Step 2 *****\n");
    
    // Initialize queues
    Queue_t readyQueue = {'R', NULL, NULL};
    Queue_t waitQueue = {'W', NULL, NULL};
    Queue_t processorQueue = {'P', NULL, NULL};
    
    int cycle = 0;
    int totalProcesses = 0;
    int totalWaitTime = 0;
    int totalTurnaroundTime = 0;
    char input[10];
    
    Pcb_t *currentProcess = NULL;
    
    while (1) {
        cycle++;
        printf("\n--- Cycle: %d\n", cycle);
        
        // Print all queues
        printf("--- Queue: %c ---\n", processorQueue.id);
        printQ(&processorQueue);
        printf("--- Queue: %c ---\n", readyQueue.id);
        printQ(&readyQueue);
        printf("--- Queue: %c ---\n", waitQueue.id);
        printQ(&waitQueue);
        
        // Get user input
        printf("Enter event: ");
        if (scanf("%9s", input) != 1) {
            break;
        }
        getchar(); // Clear input buffer
        
        // Process events
        if (input[0] == 'E') {
            printf("End of simulation\n");
            break;
        }
        else if (input[0] == 'J') {
            // New process arrival: J<id><time>
            char processId = input[1];
            int estTime = atoi(&input[2]);
            
            Pcb_t *newPCB = malloc(sizeof(Pcb_t));
            newPCB->id = processId;
            newPCB->estTime = estTime;
            newPCB->remainTime = estTime;
            newPCB->waitTime = 0;
            newPCB->turnaroundTime = 0;
            newPCB->next = NULL;
            
            enqueue(newPCB, &readyQueue);
            totalProcesses++;
            
            printf("Process %c arrived with estimated time %d\n", processId, estTime);
        }
        else if (input[0] == 'N') {
            // No event
            printf("No event this cycle\n");
        }
        
        // FCFS Scheduling Logic
        printf("--- Scheduling ---\n");
        
        // If processor is empty, get next process from ready queue
        if (processorQueue.first == NULL && readyQueue.first != NULL) {
            currentProcess = dequeueFront(&readyQueue);
            enqueue(currentProcess, &processorQueue);
        }
        
        // Execute current process in processor
        if (processorQueue.first != NULL) {
            currentProcess = processorQueue.first;
            currentProcess->remainTime--;
            
            // If process finished
            if (currentProcess->remainTime <= 0) {
                currentProcess->turnaroundTime = cycle;
                totalWaitTime += currentProcess->waitTime;
                totalTurnaroundTime += currentProcess->turnaroundTime;
                
                printf("Process %c completed\n", currentProcess->id);
                dequeueFront(&processorQueue);
                // Don't free here - we need for statistics
            }
        }
        
        // Increment wait time for processes in ready queue
        incrementWaitTime(&readyQueue);
        
        // Print queues after scheduling
        printf("--- Queue: %c ---\n", processorQueue.id);
        printQ(&processorQueue);
        printf("--- Queue: %c ---\n", readyQueue.id);
        printQ(&readyQueue);
        printf("--- Queue: %c ---\n", waitQueue.id);
        printQ(&waitQueue);
    }
    
    // Calculate and print statistics
    if (totalProcesses > 0) {
        printf("\n=== Simulation Results ===\n");
        printf("Total processes: %d\n", totalProcesses);
        printf("Average wait time: %.2f\n", (float)totalWaitTime / totalProcesses);
        printf("Average turnaround time: %.2f\n", (float)totalTurnaroundTime / totalProcesses);
    }
    
    // Free remaining memory (in a real implementation)
    printf("**** End Step 2 *****\n");
}

int main() {
    // Run Step 1 test
    testStep1();
    
    printf("\n");
    
    // Run Step 2 scheduler
    runScheduler();
    
    return 0;
}