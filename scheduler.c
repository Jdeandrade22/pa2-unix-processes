#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pcb {
    char id;
    int estTime;
    int remainTime;
    int waitTime;
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
        // empty[]
        queue->first = pcb;
        queue->last = pcb;
    } else {
        // add
        queue->last->next = pcb;
        queue->last = pcb;
    }
}

// removing fun
void dequeue(Queue_t *queue, Pcb_t *pcb) {
    if (queue == NULL || pcb == NULL || queue->first == NULL) return;
    
    //pos.
    if (queue->first == pcb) {
        queue->first = pcb->next;
        if (queue->last == pcb) {
            queue->last = NULL;
        }
        return;
    }
    
    // Search for the PCB in the queue
    Pcb_t *current = queue->first;
    while (current != NULL && current->next != pcb) {
        current = current->next;
    }
    
    // If found, remove it
    if (current != NULL) {
        current->next = pcb->next;
        if (queue->last == pcb) {
            queue->last = current;
        }
    }
}

// printing in queue
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
//test
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
    
    // Create
    printf("--- Define ready queue\n");
    Queue_t Rqueue;
    Rqueue.id = 'R';
    Rqueue.first = NULL;
    Rqueue.last = NULL;
    
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    //PCBs
    printf("--- enqueue PCBs\n");
    enqueue(pcbA, &Rqueue);
    enqueue(pcbB, &Rqueue);
    enqueue(pcbC, &Rqueue);
    
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // (B)
    printf("--- Remove the middle PCB from the R queue\n");
    dequeue(&Rqueue, pcbB);
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // (C)
    printf("--- Remove the last PCB from the R queue\n");
    dequeue(&Rqueue, pcbC);
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    // (A)
    printf("--- Remove the remaining PCB from the R queue\n");
    dequeue(&Rqueue, pcbA);
    printf("--- Queue: %c ---\n", Rqueue.id);
    printQ(&Rqueue);
    
    printf("*** End Step 1 ***\n");
    
    free(pcbA);
    free(pcbB);
    free(pcbC);
}

int main() {
    testStep1();
    return 0;
}