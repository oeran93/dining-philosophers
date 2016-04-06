#include "drinking.hpp"

#include <vector>
#include <set>

#define MIN_PHILOSOPHERS 5
#define MAX_PHILOSOPHERS 15
#define MIN_DRINKS 5

#define MIN_THINKING_TIME 2
#define MIN_DRINKING_TIME 2
#define MAX_THINKING_TIME 10
#define MAX_DRINKING_TIME 5

#define DEBUG 1


std::vector<philosopher*> philosophers;
sem_t terminal;
pthread_t *threads;

int rand_range(int min, int max) {
	return rand() % (max + 1 - min) + min; // add 1 to make it inclusive
}

void create_bottle(philosopher *left, philosopher *right, int i, int j) {
	bottle *b = new bottle;
	b->i = i;
	b->j = j;
	sem_init(&(b->semaphore), 0, 1);
	left->bottles.push_back(b);
	right->bottles.push_back(b);
}

void parse_matrix(char *filename) {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	size_t read;

	unsigned int current_phil_col = 0;
	unsigned int current_phil_row = 0;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Unable to open file %s.\n", filename);
		exit(EXIT_FAILURE);
	}
	// get intial philosopher count
	read = getline(&line, &len, fp);
	for (size_t i = 0; i < len; i++) {
		if (line[i] == '0' || line[i] == '1') {
			philosophers.push_back(new philosopher);
			philosophers[current_phil_col]->t_id = current_phil_col;
			if (line[i] == '1') {
			create_bottle(
					philosophers[current_phil_row],
					philosophers[current_phil_col],
					current_phil_row,
					current_phil_col
				);
			}
			current_phil_col++;
		}
	}
	if (philosophers.size() < MIN_PHILOSOPHERS || philosophers.size() > MAX_PHILOSOPHERS) {
		printf("Incorrect number of philosophers or malformed matrix data.\n");
		exit(EXIT_FAILURE);
	}
	current_phil_row++;

	// assign bottles for rest of lines
	while ((read = getline(&line, &len, fp)) != -1 && current_phil_row < philosophers.size()) {
		current_phil_col = current_phil_row; // ignoring bottom left b/c symmetrical matrix
		for (size_t i = 0; i < len; i++) {
			if (line[i] == '0' || line[i] == '1') {
				if (line[i] == '1') {
					create_bottle(
						philosophers[current_phil_row],
						philosophers[current_phil_col],
						current_phil_row,
						current_phil_col
					);
				}
				current_phil_col++;
			}
		}
		current_phil_row++;
	}

	// print phils and bottles
	if (DEBUG) {
		for (unsigned int i = 0; i < philosophers.size(); i++) {
			printf("phil %d has: ", i + 1);
			for (unsigned int j = 0; j < philosophers[i]->bottles.size(); j++) {
				bottle *b = philosophers[i]->bottles[j];
				printf("(%d, %d) ", b->i + 1, b->j + 1);
			}
			printf("\n");
		}
	}
	
}

std::vector<bottle*> choose_bottles(philosopher *p) {
	std::vector<bottle*> chosen_bottles;
	std::set<int> bottle_indexes;
	int bottle_count = rand_range(1, p->bottles.size());
	for (int i = 0; i < bottle_count; i++) {
		int random_index = (rand() % p->bottles.size());
		bottle_indexes.insert(random_index);
	}
	for (int j : bottle_indexes) {
		chosen_bottles.push_back(p->bottles[j]);
	}
	return chosen_bottles;
}

void think() {
	sleep((rand() % MAX_THINKING_TIME) + MIN_THINKING_TIME);
}

void drink(std::vector<bottle*> chosen_bottles, philosopher *p) {
	for (unsigned int i = 0; i < chosen_bottles.size(); i++) {
		sem_wait(&(chosen_bottles[i]->semaphore));
	}

	// got bottles, wait on console and "drink"
	sem_wait(&terminal);
	printf("Philosopher %d: drinking from bottles ", p->t_id);
	for (unsigned int j = 0; j < chosen_bottles.size(); j++) {
		printf("(%d, %d) ", chosen_bottles[j]->i, chosen_bottles[j]->j);
	}
	printf("\n");
	sem_post(&terminal);
	
	p->s = DRINKING;
	sleep(rand_range(MIN_DRINKING_TIME, MAX_DRINKING_TIME)); // drink	
	
	sem_wait(&terminal);
	printf("Philosopher %d: putting down bottles ", p->t_id);
	for (unsigned int j = 0; j < chosen_bottles.size(); j++) {
		printf("(%d, %d) ", chosen_bottles[j]->i, chosen_bottles[j]->j);
	}
	printf("\n");
	sem_post(&terminal);

	// free up bottles
	for (unsigned int k = chosen_bottles.size(); k > 0; k--) {
		sem_post(&(chosen_bottles[k-1]->semaphore));
	}
}

void* philosopher_thread(void *t_p) {
	// remember to free after being done with drinking session bottles
	philosopher *p = (philosopher*) t_p;
	int drink_count = 0;
	while (drink_count < MIN_DRINKS) {
		p->s = THINKING;
		think();
		p->s = THIRSTY;
		std::vector<bottle*> session_bottles = choose_bottles(p);
		drink(session_bottles, p);
		drink_count++;
	}
	return NULL;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Incorrect number of command line arguments.\n");
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));

	sem_init(&terminal, 0, 1);
	parse_matrix(argv[1]);
	threads = new pthread_t[philosophers.size()];
	
	for (int i = 0; i < philosophers.size(); i++) {
		pthread_create(&threads[i], NULL, philosopher_thread, (void*)(philosophers[i]));
	}
	for (int i = 0; i < philosophers.size(); i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
