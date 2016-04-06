#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <vector>


typedef enum _P_STATE {
	THINKING,
	THIRSTY,
	DRINKING
} state;

typedef struct _BOTTLE {
	sem_t semaphore;
	int i;
	int j;
} bottle;

typedef struct _PHILOSOPHER {
	state s;
	int t_id;
	std::vector<bottle*> bottles;
} philosopher;


// creates a bottle for left and right to share
void create_bottle(philosopher *left, philosopher *right);

// load a matrix from a file and create the needed philosophers and bottles
void parse_matrix(char *filename);

// actual philosopher thread
void* philosopher_thread(void *t_id);

// chooses random bottles for a drinking session, accepts ptr to array of sem_t's, and count
void choose_bottles(philosopher *p, bottle **chosen_bottles, int *chosen_bottle_count);

// busy waits for state changes. states do not change in functions.
void think();

// busy waits, but also requests bottles in order
void drink(bottle **chosen_bottles, int chosen_bottle_count);
