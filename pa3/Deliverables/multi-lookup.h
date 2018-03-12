#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "util.h"
#include "lifo.h"

#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MAX_REQUESTER_THREADS 5
#define MAX_IP_LENGTH INET6_ADDRSTRLEN
#define MAX_ARRAY_SIZE 20

#define FILE1 0
#define FILE2 1
#define FILE3 2
#define FILE4 3
#define FILE5 4
#define ARRAY 5
#define SERVICED 6
#define RESULTS 7
#define ARGS 8

#define UNUSED(x) (void)(x)

void * requester_thread(void * ptr);
void * resolver_thread(void * ptr);
