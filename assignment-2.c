//TO COMPILE: gcc -Wall assignment-2.c -o assignment-2 -lm
//TO RUN: ./assignment-2 inputFileName minFrames maxFrames numPages

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "string.h"

// Singly Linked List: Used to store data while parsing input file
struct Node {
    int val; 
    struct Node* next; 
};

// Pass Type: Used for clarity in runPasses method
enum PassType {Opt, Lru, Fifo, Clock};

// Input Validation Methods:
int validate(int argc, char* argv[]);

// Data Processing Methods:
void runPasses(struct Node* n, int minFrames, int maxFrames, enum PassType pt, int* results);
int optPass(int* frames, int* states, int numFrames, struct Node* n);
int lruPass(int* frames, int* states, int numFrames, int newPage);
int fifoPass(int* frames, int* states, int numFrames, int newPage);
int clockPass(int* frames, int* states, int clockPointer, int numFrames, int newPage);

// Data Display Methods:
void resultsToTable(int minFrames, int maxFrames, int* optResults, int* lruResults, int* fifoResults, int* clockResults);

// Utility Methods:
int fileToList(char* fName, struct Node* n);
void freeList(struct Node* n);
int indexOf(int* arr, int n, int target);
int maxAt(int* arr, int n);

int main(int argc, char* argv[])
{
    // Validate input parameters
    if(validate(argc, argv)) {
        return 1;
    }
    
    struct Node* head = (struct Node*) malloc(sizeof(struct Node));
    
    // Validate input file, parse input file
    if(fileToList(argv[1], head)) {
        freeList(head);
        return 1;
    }
    
    // Set up infrastructure to store processed data
    int minFrames = atoi(argv[2]);
    int maxFrames = atoi(argv[3]);
    int n = maxFrames - minFrames + 1;
    int* optResults = (int*) calloc(n, sizeof(int));
    int* lruResults = (int*) calloc(n, sizeof(int));
    int* fifoResults = (int*) calloc(n, sizeof(int));
    int* clockResults = (int*) calloc(n, sizeof(int));
    
    // Process data
    runPasses(head, minFrames, maxFrames, Opt, optResults);
    runPasses(head, minFrames, maxFrames, Lru, lruResults);
    runPasses(head, minFrames, maxFrames, Fifo, fifoResults);
    runPasses(head, minFrames, maxFrames, Clock, clockResults);
    
    // Display Data
    resultsToTable(minFrames, maxFrames, optResults, lruResults, fifoResults, clockResults);
    
    // Free all allocated memory to avoid memory leaks
    free(optResults);
    free(lruResults);
    free(fifoResults);
    free(clockResults);
    freeList(head);
    
    // Finish method, report no errors
    return 0;
}

int validate(int argc, char* argv[]) {
    // Validate that exactly five arguments are given
    if(argc != 5) {
        printf("Invalid Syntax: Requires 5 arguments, found %d\n", argc);
        printf("Correct Syntax: ./Assignment2 inputFileName minFrames maxFrames numPages\n");
        printf("Example Syntax: ./Assignment2 inputfile.txt 4 14 20\n\n");
        return 1;
    }
    
    // Validate min frames
    if(atoi(argv[2]) <= 0) {
        printf("Error: Requires positive integer for min frames, value provided: %s\n", argv[2]);
        return 1;
    }
    
    // Validate max frames
    if(atoi(argv[3]) <= 0) {
        printf("Error: Requires positive integer for max frames, value provided: %s\n", argv[3]);
        return 1;
    }
    
    // Validate num pages
    if(atoi(argv[4]) <= 0) {
        printf("Error: Requires positive integer for num pages, value provided: %s\n", argv[4]);
        return 1;
    }
    
    return 0;
}

// Parse a text file, returning a singly linked list of its contents
// It is assumed that the text file contains only positive numbers delimited by spaces or lines
int fileToList(char* fName, struct Node* n)
{
	FILE* fp = fopen(fName, "r");
	struct Node* nNext;
	
	if (fp == NULL) {
	    printf("Error: File %s does not exist\n\n", fName);
	    fclose(fp);
	    return 1;
	}
	
	char c;
	int i = -1;
	
	while((c = fgetc(fp)) != EOF)
	{
        if(c >= '0' && c <= '9')
        {
            if(i >= 0)
            {
                i *= 10;
                i += c - '0';
            }
            else
            {
                i = c - '0';
                nNext = (struct Node*) malloc(sizeof(struct Node));
                nNext->next = NULL;
			    n->next = nNext;
			    n = nNext;
            }
        }
		else if(c == ' ' || c == '\n' || c == '\r')
		{
		    if(i >= 0) {
		        n->val = i + 1;
			    i = -1;
		    }
		}
		else
		{
			printf("Error: Invalid character in file: %c\n\n", c);
			fclose(fp);
			return 1;
		}
	}
	
	if(i >= 0)
	{
		n->val = i + 1;
		i = -1;
	}
	
	fclose(fp);
	return 0;
}

// Perform complete runs of any given frame allocation algorithm for a specified range of available frames
void runPasses(struct Node* n, int minFrames, int maxFrames, enum PassType pt, int* results)
{
    for(int numFrames = minFrames; numFrames <= maxFrames; numFrames++)
    {
        struct Node* tempN = n;
        int passResult = 0;
        int clockPointer = 0;               // Used exclusively for clockPass
        int* frames = (int*) calloc(numFrames, sizeof(int));
        int* states = (int*) calloc(numFrames, sizeof(int));
        
        while(tempN != NULL) {
            switch(pt)
            {
                case Opt:
                    passResult += optPass(frames, states, numFrames, tempN);
                    break;
                case Lru:
                    passResult += lruPass(frames, states, numFrames, tempN->val);
                    break;
                case Fifo:
                    passResult += fifoPass(frames, states, numFrames, tempN->val);
                    break;
                case Clock:
                    passResult += clockPass(frames, states, clockPointer, numFrames, tempN->val);
                    break;
                default:
                    printf("Error: Invalid PassType\n\n");
                    return;
            }
            
            tempN = tempN->next;
        }
        
        free(frames);
        free(states);
        
        results[numFrames - minFrames] = passResult;
    }
}

// Perform a single cycle of the optimal frame allocation algorithm
int optPass(int* frames, int* states, int numFrames, struct Node* n)
{
    int rtn = 0;
    int targetFrame = indexOf(frames, numFrames, n->val);
    int newState = 0;
    
    if(frames[0] == 0)
    {
        for(int i = 0; i < numFrames; i++)
        {
            states[i] = INT_MAX;
        }
    }
    
    if(targetFrame == -1)
    {
        targetFrame = maxAt(states, numFrames);
        frames[targetFrame] = n->val;
        rtn = 1;
    }
    
    n = n->next;
    
    while(n != NULL)
    {
        if(n->val == frames[targetFrame])
        {
            break;
        }
        newState++;
        n = n->next;
    }
    
    states[targetFrame] = newState;
    return rtn;
}

// Perform a single cycle of the least recently used frame allocation algorithm
int lruPass(int* frames, int* states, int numFrames, int newPage)
{
    int rtn = 0;
    int targetFrame = indexOf(frames, numFrames, newPage);
    
    for(int i = 0; i < numFrames; i++)
    {
        states[i]++;
    }
    
    if(targetFrame == -1)
    {
        targetFrame = maxAt(states, numFrames);
        frames[targetFrame] = newPage;
        rtn = 1;
    }
    
    states[targetFrame] = 0;
    return rtn;
}

// Perform a single cycle of the first in first out frame allocation algorithm
int fifoPass(int* frames, int* states, int numFrames, int newPage)
{
    int rtn = 0;
    
    for(int i = 0; i < numFrames; i++)
    {
        states[i]++;
    }
    
    if(indexOf(frames, numFrames, newPage) == -1)
    {
        int targetFrame = maxAt(states, numFrames);
        frames[targetFrame] = newPage;
        states[targetFrame] = 0;
        rtn = 1;
    }
    
    return rtn;
}

// Perform a single cycle of the clock / second chance frame allocation algorithm
int clockPass(int* frames, int* states, int clockPointer, int numFrames, int newPage)
{
    int rtn = 0;
    int targetFrame = indexOf(frames, numFrames, newPage);
    
    if(targetFrame == -1) {
        while(states[clockPointer] != 1) {
            states[clockPointer]++;
            clockPointer = (clockPointer + 1) % numFrames;
        }
        
        targetFrame = clockPointer;
        frames[targetFrame] = newPage;
        clockPointer = (clockPointer + 1) % numFrames;
        rtn = 1;
    }
    
    states[targetFrame] = 0;
    
    return rtn;
}

// Display results in ASCII table
void resultsToTable(int minFrames, int maxFrames, int* optResults, int* lruResults, int* fifoResults, int* clockResults)
{
    printf("+--------+-------+-------+-------+-------+\n");
    printf("| Frames |   OPT |   LRU |  FIFO | CLOCK |\n");
    printf("+--------+-------+-------+-------+-------+\n");
    for(int i = 0; i < maxFrames - minFrames + 1; i++) {
	printf ("|%7d | %5d | %5d | %5d | %5d |\n", minFrames + i, optResults[i], lruResults[i], fifoResults[i], clockResults[i]);
	printf("+--------+-------+-------+-------+-------+\n");
    }    
}

// Returns first index of target in arr
// Returns -1 if target is not in arr
int indexOf(int* arr, int n, int target) {
    for(int i = 0; i < n; i++)
    {
        if(arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

// Returns index of highest value in arr (earliest index, in cases of ties)
int maxAt(int* arr, int n) {
    int rtn = 0;
    
    for(int i = 1; i < n; i++)
    {
        if(arr[rtn] < arr[i])
        {
            rtn = i;
        }
    }
    
    return rtn;
}

// Deallocate memory from a singly linked list
void freeList(struct Node* head)
{
   struct Node* temp;
   while (head != NULL)
    {
       temp = head;
       head = head->next;
       free(temp);
    }
}
