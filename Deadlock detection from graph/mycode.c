
//osd project 2
//  DeadlockDetection


#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAX_COUNT 1000

int deadlockDetected = FALSE;
struct Stack {
    int top;
    int array[MAX_COUNT];
};

void push(struct Stack *stack, int data) {
    if (stack -> top == MAX_COUNT) {
        printf("Overflow");
        exit(1);
    }
    stack -> array[++(stack -> top)] = data;
}

int pop(struct Stack *stack) {
    if (stack -> top == -1) {
        printf("Underflow");
        exit(1);
    }
    int temp = stack -> array[(stack -> top)--];
    return temp;
}

/**
 * util method to print an array
 */
void printArray(int *array, int n) {
    int i = 0;
    printf("\nArray : ");
    for (i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

/** * util method to print a stack*/
void printStack(struct Stack *stack) {
    int i = stack -> top;
    printf("\nPrinting stack : ");
    while (i >= 0) {
        printf("%d, ", stack -> array[i--]);
    }
}

/**
 * method to show a graph
 */
void showGraph(int **graph, int totalNodes) {
    int i = 0;
    int j = 0;
    printf("\nGraph:");
    for (i = 0; i < totalNodes; i++) {
        printf("\n");
        for (j = 0; j < totalNodes; j++) {
            printf("%d  ", graph[i][j]);
        }
    }
    printf("\n");
}

/**
 * validates if edge that has entered is correct or not.
 */
int validateEdge(int source, int sink, int numProcesses, int numResources, int *usedResource) {

    if (source < 1 || sink < 1) {
        printf("Invalid id. can't be less than 1");
        return 0;
    }

    if ((source >= 1 && source <= numProcesses) &&
        (sink >= 1 && sink <= numProcesses)) {
        printf("Both ids belong to process.");
        return 0;
    }

    if ((source > numProcesses && source <= numProcesses + numResources) &&
        (sink > numProcesses && sink <= numProcesses + numResources)) {
        printf("Both ids belong to resource.");
        return 0;
    }

    if (source > numProcesses + numResources || sink > numResources + numProcesses) {
        printf("Invalid id.");
        return 0;
    }
    
    if (source > numProcesses) { //means source is a resource
        if (usedResource[source - numProcesses - 1]) {
            printf("Resource %d already used. Can't assign 1 resource to 2 processes.", source);
            return 0;
        } else {
            usedResource[source - numProcesses - 1] = TRUE;
        }
    }
    return 1;
}

/**
 * this is a depth first search algorithm which keeps on pushing all visited nodes on to a stack.
 * The moment a node is visited and is already present on stack, it means that there is a deadlock.
 */
void dfs(int **graph, int *visited, int *isOnStack, int source, int totalNodes, struct Stack *stack, int *isPartOfCycle) {
    int i = 0;
    if (isOnStack[source]) {
        if (!isPartOfCycle[source]) {
            printf("\nDeadlock detected : (");
            deadlockDetected = TRUE;
            i = stack -> top;
            while (stack -> array[i] != source) {
                i--;
            }
            while (i <= stack -> top) {
                isPartOfCycle[stack -> array[i]] = TRUE;
                printf("%d -> ", (stack -> array[i] + 1));
                i++;
            }
            printf("%d)", (source + 1));
        }
        return;
    }
    visited[source] = TRUE;
    push(stack, source);
    isOnStack[source] = TRUE;
    for (i = 0; i < totalNodes; i++) {
        if (graph[source][i] == 1) {
            dfs(graph, visited, isOnStack, i, totalNodes, stack, isPartOfCycle);
        }
    }
    pop(stack);
    isOnStack[source] = FALSE;
}

/**
 * This the method that detects a deadlock
 */
void detectDeadlock(int** graph, int n) {
    int* visited = (int *) malloc (sizeof(int) * n);
    int* isPartOfCycle = (int *) malloc (sizeof(int) * n);
    int* isOnStack = (int *) malloc (sizeof(int) * n);
    int i = 0;
    int j = 0;
    struct Stack stack;
    stack.top = -1;
    
    for (i = 0; i < n; i++) {
        visited[i] = FALSE;
        isOnStack[i] = FALSE;
        isPartOfCycle[i] = FALSE;
    }

    for (i = 0; i < n; i++) {
        if (visited[i] == FALSE) {
            dfs(graph, visited, isOnStack, i, n, &stack, isPartOfCycle);
            for (j = 0; j < n; j++) {
                isOnStack[j] = FALSE;
            }
        }
    }
    if (!deadlockDetected) {
    		printf("\nThere is no deadlock in the system.");
    }
    free(visited);
    free(isPartOfCycle);
    free(isOnStack);
}

/**
 * main method
 */
int main() {
    
    int numProcesses = 0;
    int numResources = 1;
    int numEdges = 0;
    int i = 0;
    int j = 0;
    int source;
    int sink;
    printf(" enter number of processes");
    scanf("%d", &numProcesses);
    printf(" enter number of resources");
	scanf("%d", &numResources);

     printf("Enter the matrix values");
    int *usedResource = (int *) malloc (sizeof(int) * numResources);
    for (i = 0; i < numResources; i++) {
        usedResource[i] = FALSE;
    }
     
    int totalNodes = numProcesses + numResources;
    int** graph = (int **) malloc(sizeof(int *) * totalNodes);
    for (i = 0; i < totalNodes; i++) {
        graph[i] = (int *) malloc (sizeof(int) * totalNodes);
    }

    for (i = 0; i < totalNodes; i++) {
        for (j = 0; j < totalNodes; j++) {
            
			scanf("%d", &graph[i][j]);

        }
    }

    detectDeadlock(graph, totalNodes);
    free(usedResource);
    for (i = 0; i < totalNodes; i++) {
        free(graph[i]);
    }
    free(graph);
    printf("\n");
    return 0;
}

