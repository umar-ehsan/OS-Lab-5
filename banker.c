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
#define NUM_CUSTOMERS 3
#define NUM_RESOURCES 3


// Put global environment variables here
pthread_mutex_t request_lock, release_lock;
pthread_t customers[NUM_CUSTOMERS];

// Available amount of each resource
int available[NUM_RESOURCES] = {0};
//int available[NUM_RESOURCES] = {1,1,1};
// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES] = {0};
//int maximum[NUM_CUSTOMERS][NUM_RESOURCES] = {{8,8,8},{8,8,8},{8,8,8},{8,8,8},{8,8,8}};
// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES] = {0};
//int allocation[NUM_CUSTOMERS][NUM_RESOURCES] = {{2,2,2},{2,2,2},{2,2,2},{2,2,2},{2,2,2}};
// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES] = {0};
//int need[NUM_CUSTOMERS][NUM_RESOURCES] = {{2,2,2},{2,2,2},{2,2,2},{2,2,2},{1,1,1}};

int main(int argc, char *argv[]){
    // ==================== YOUR CODE HERE ==================== //
    ///*
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
    if (pthread_mutex_init(&request_lock, NULL) != 0 || pthread_mutex_init(&release_lock, NULL) != 0){
        printf("\nError initializing mutex locks. Terminating...\n\n");
        return EXIT_FAILURE;
    }
    //*/
    getchar();
    // Run the threads and continually loop
    // The threads will request and then release random numbers of resources
    while (1){
        int n_arg = 0;
        printf("\nRequesting resources: \n\n");
        for (int i = 0; i < NUM_CUSTOMERS; i++){
            n_arg = i;
            printf("Process #%d:\n", n_arg);
            pthread_create(&customers[i], NULL, res_request, (void*)&n_arg);
            //if (pthread_create(&customers[i], NULL, res_request, (void*)&n_arg) != 0){
            //    printf("\nError creating thread, terminating...\n\n");
            //    return EXIT_FAILURE;
            //}
        }
        for (int i = 0; i < NUM_CUSTOMERS; i++){
            if (pthread_join(customers[i], NULL) != 0){
                printf("\nError joining thread, terminating...\n\n");
                return EXIT_FAILURE;
            }
        }
        printf("Press any key to continue...\n");
        getchar();
        printf("\nReleasing resources: \n\n");
        for (int i = 0; i < NUM_CUSTOMERS; i++){
            n_arg = i;
            if (pthread_create(&customers[i], NULL, res_release, (void*)&n_arg) != 0){
                printf("\nError creating thread, terminating...\n\n");
                return EXIT_FAILURE;
            }
        }
        for (int i = 0; i < NUM_CUSTOMERS; i++){
            if (pthread_join(customers[i], NULL) != 0){
                printf("\nError joining thread, terminating...\n\n");
                return EXIT_FAILURE;
            }
        }
        printf("Press any key to continue...\n");
        getchar();
    }

    return EXIT_SUCCESS;
}

// Requests a resource
void *res_request(void *arg){
    printf("hmm: %d\n", *((int*)arg));
    pthread_mutex_lock(&request_lock);
    int n_process = *((int*)arg);
    int request[NUM_RESOURCES];
    printf("Process %d requesting resources\n\n", n_process);
    for (int i = 0; i < NUM_RESOURCES; i++){
        request[i] = rand() % maximum[n_process][i];
    }
    printf("Process #%d requesting the resources: [", n_process);
    for (int i = 0; i < NUM_RESOURCES; i++){
        printf("%d,", request[i]);
    }
    printf("\b]\n");
    if(request_res(n_process, request) == true){
        printf("Resources were successfully allocated\n");
    }
    else {
        printf("Unable to allocate the specified resources.\n");
    }
    printf("\nProcess %d exiting resource request routine\n\n", n_process);
    pthread_mutex_unlock(&request_lock);
    return NULL;
}

// Releases a resource
void *res_release(void *arg){
    pthread_mutex_lock(&release_lock);
    int n_process = *((int*)arg);
    printf("Process %d releasing resources\n\n", n_process);
    int request[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; i++){
        request[i] = rand() % maximum[n_process][i];
    }
    printf("Process #%d releasing the resources: [", n_process);
    for (int i = 0; i < NUM_RESOURCES; i++){
        printf("%d,", request[i]);
    }
    printf("\b]\n");
    if(release_res(n_process, request) == true){
        printf("Resources were successfully released\n");
    }
    else {
        printf("Unable to release the specified resources.\n");
    }
    printf("\nProcess %d exiting resource release routine\n\n", n_process);
    pthread_mutex_unlock(&release_lock);
    return NULL;
}

// Request resources, returns true if successful
bool request_res(int n_customer, int request[]){
    for (int i = 0; i < NUM_RESOURCES; i++){
        if (request[i] > available[i] || request[i] > need[n_customer][i]){
            printf("Requested resources are higher than available resources, or are\n");
            printf("higher than the actual count of needed resources.\n");
            return false;
        }
    }
    for (int i = 0; i < NUM_RESOURCES; i++){
        allocation[n_customer][i] += request[i];
        need[n_customer][i] = maximum[n_customer][i] - allocation[n_customer][i];
        available[i] -= request[i];
    }
    if(is_safe()){
        printf("The system is in a safe state after the request.\n");
        return true;
    }
    else {
        for (int i = 0; i < NUM_RESOURCES; i++){
            allocation[n_customer][i] -= request[i];
            need[n_customer][i] = maximum[n_customer][i] - allocation[n_customer][i];
            available[i] += request[i];
        }
        printf("The system is in an unsafe state after the request.\n");
        return false;
    }
}

// Release resources, returns true if successful
bool release_res(int n_customer, int release[]){
    for (int i = 0; i < NUM_RESOURCES; i++){
        if (release[i] > allocation[n_customer][i]){
            printf("Released resources are higher than allocated resources.\n");
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
bool is_safe(){
    int true_count = 0;
    int comparison_count = 0;
    int work[NUM_RESOURCES];
    bool finish[NUM_CUSTOMERS] = {false};
    for (int i = 0; i < NUM_RESOURCES; i++){
        work[i] = available[i];
    }
    for (int i = 0; i < NUM_CUSTOMERS && true_count < 5; i++){
        for (int j = 0; j < NUM_CUSTOMERS && true_count < 5; j++){
            //printf("i, j = %d, %d\n", i, j);
            //getchar();
            for (int k = 0; k < NUM_RESOURCES; k++){
                if (need[j][k] <= work[k]){
                    comparison_count++;
                }
            }
            if (finish[j] == false && comparison_count == NUM_RESOURCES){
                finish[j] = true;
                true_count++;
                for (int k = 0; k < NUM_RESOURCES; k++){
                    work[k] += allocation[j][k];
                }
            }
            comparison_count = 0;
        }
    }
    if (true_count == 5){
        return true;
    }
    else {
        return false;
    }
}
