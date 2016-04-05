#include <time.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>

/*
* checks that exactly 1 argument was passed to the program, prints usage and
* exits if that was not the case
*/
void check_args(int);
/*
* initializes 1 activity semaphore and as many forks semaphores as needed
*/
void __init();
/*
* simulates the actions of a philosopher @param void * philosopher id
*/
void * philosopher(void *);
/*
* changes the current state of a philosopher
* @param int philosopher id
* @param int maximum time to stay in that state
*/
void change_phil_state(int,int);
/*
* prints the state of the dining room; how many philosophers are thinking and
* how many are eating
*/
void print_dining_room();

