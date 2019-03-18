/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"

// Put any other macros or constants here using #define
// May be any values >= 0
//#define NUM_CUSTOMERS 5
#define NUM_CUSTOMERS 2
#define NUM_RESOURCES 3


// Put global environment variables here
pthread_mutex_t res_lock;
bool finish[NUM_CUSTOMERS] = {false};

// Available amount of each resource
int available[NUM_RESOURCES] = {0};

// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES] = {0};

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES] = {0};

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES] = {0};

int main(int argc, char *argv[])
{
    // ==================== YOUR CODE HERE ==================== //

    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments   
    if (argc != 4){
        printf("\nUsage: ./banker <argument 1> <argument 2> <argument 3>\nWhere <argument n> is an integer representing the resource amount.\n\n");
        return EXIT_FAILURE;
    }
    else {
        for (int i = 0; i < NUM_RESOURCES; i++){
            available[i] = atoi(argv[i + 1]);
        }
    }

    // Allocate the available resources
    for (int i = 0; i < NUM_CUSTOMERS; i++){
        printf("\nFor customer %d, please enter the resources as indicated.\n", i + 1);
        int j = 0;
        while (j < NUM_RESOURCES){
            printf("\nEnter the maximum amount of available resource #%d (available: %d): ", j + 1, available[j]);
            scanf("%d", &maximum[i][j]);
            need[i][j] = maximum[i][j];
            if (maximum[i][j] > available[j]){
                printf("Error, you entered a number higher than maximum available resources\n");
            }
            else {
                j++;
            }
        }
    }

    // Initialize the pthreads, locks, mutexes, etc.
    pthread_t customers[NUM_CUSTOMERS];
    if (pthread_mutex_init(&res_lock, NULL) != 0){
        printf("\nError initializing mutex locks. Terminating...\n\n");
        return EXIT_FAILURE;
    }

    // Run the threads and continually loop


    // The threads will request and then release random numbers of resources

    // If your program hangs you may have a deadlock, otherwise you *may* have
    // implemented the banker's algorithm correctly
    
    // If you are having issues try and limit the number of threads (NUM_CUSTOMERS)
    // to just 2 and focus on getting the multithreading working for just two threads

    return EXIT_SUCCESS;
}

// Requests a resource
void *resource_req(void *arg){
    pthread_mutex_lock(&res_lock);
    int n_process = *((int*)arg);
    int request[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; i++){
        request[i] = rand() % maximum[n_process][i];
    }
    request_res(n_process, request);
    pthread_mutex_unlock(&res_lock);
    return NULL;
}

// Releases a resource
void *res_release(void *arg){
    pthread_mutex_lock(&res_lock);
    int n_process = *((int*)arg);
    int request[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; i++){
        request[i] = rand() % maximum[n_process][i];
    }
    release_res(n_process, request);
    pthread_mutex_unlock(&res_lock);
    return NULL;
}

// Request resources, returns true if successful
bool request_res(int n_customer, int request[]){
    for (int i = 0; i < NUM_RESOURCES; i++){
        if (request[i] > available[i] || request[i] > need[i]){
            return false;
        }
    }
    return is_safe(n_customer, request);
}

// Release resources, returns true if successful
bool release_res(int n_customer, int release[]){
    for (int i = 0; i < NUM_RESOURCES; i++){
        if (release[i] > allocation[n_customer][i]){
                return false;
        }    
    }
    for (int i = 0; i < NUM_RESOURCES; i++){
        allocation[n_customer][i] -= release[i];
        available[i] += release[i];
        need[n_customer][i] += release[i];
    }
    return true;
}

// Returns true if state is safe, false otherwise
bool is_safe(int n_customer, int request[]){
    int false_count = 0;
    int work[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; i++){
        work[i] = available[i];
    }

}
