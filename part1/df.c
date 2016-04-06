#include "df.h"

#define MIN_MEALS 5
#define MAX_EATING_TIME 3
#define MAX_THINKING_TIME 10

int num_phils;
pthread_t * threads;
sem_t state;
sem_t *forks;
enum {thinking, eating};
int * phils_state; 
int * phils_id;

int 
main(int argc, char ** argv) 
{
    check_args(argc);
    num_phils = atoi(argv[1]);
    __init();
    for (int i = 0; i < num_phils; i++) {
		pthread_create(&threads[i],NULL,philosopher,&phils_id[i]);
    }
    for (int i = 0; i < num_phils; i++) {
		pthread_join(threads[i],NULL);
    }
}

void 
check_args(int argc)
{
    if (argc != 2) {
		printf("usage:\n ./df [number philosophers]\n");
		_exit(1);
    }
}

void 
__init()
{
    threads = malloc(sizeof(pthread_t)*num_phils);
    forks = malloc(sizeof(sem_t)*num_phils);
    phils_state = malloc(sizeof(int)*num_phils);
    phils_id = malloc(sizeof(int)*num_phils);
    for (int i = 0; i < num_phils; i++) {
		phils_state[i] = thinking;
		phils_id[i] = i;
		sem_init(&forks[i],0,1);
    }
    sem_init(&state,0,1);
}

void * 
philosopher(void * t_id) 
{ 
    int id = *(int *)t_id;
    int numMeals = 0;
    srand(time(NULL)); /*seeding rand num generator*/
    while (numMeals < MIN_MEALS) {
		sem_wait(&forks[id]);
		sem_wait(&forks[(id+1)%num_phils]);
		change_phil_state(id,MAX_EATING_TIME);
		sem_post(&forks[(id+1)%num_phils]);
		sem_post(&forks[id]);
		numMeals++;
		change_phil_state(id,MAX_THINKING_TIME);
    }
    return 0;
}

void 
change_phil_state(int id, int max_time) 
{
    int min_time = 1;
    int sleep_time = (rand() % max_time) + min_time;
    sem_wait(&state);
    phils_state[id] = !phils_state[id];
    print_dining_room();
    sem_post(&state);
    sleep(sleep_time);
}

void 
print_dining_room()
{
    for (int i = 0; i < num_phils; i++) {
		printf("%c ", (phils_state[i]==thinking) ? ' ' : '*');
    }
    printf("\n");
    fflush(stdout);
}
