/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef BANKER_H_
#define BANKER_H_

#include <stdbool.h>

// Add any additional data structures or functions you want here
// perhaps make a clean bank structure instead of several arrays...

// Request resources, returns true if successful
extern bool request_res(int n_customer, int request[]);

// Release resources, returns true if successful
extern bool release_res(int n_customer, int release[]);

// Returns true if state is safe, false otherwise
extern bool is_safe(void);

// Requests a resource
extern void *res_request(void *arg);

// Releases a resource
extern void *res_release(void *arg);


#endif /* BANKER_H_ */