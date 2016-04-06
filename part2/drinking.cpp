#include "drinking.hpp"

#include <vector>

#define MIN_PHILOSOPHERS 5
#define MAX_PHILOSOPHERS 15
#define MIN_DRINKS 5

#define MIN_THINKING_TIME 2
#define MIN_DRINKING_TIME 2
#define MAX_THINKING_TIME 10
#define MAX_DRINKING_TIME 5


std::vector<philosopher*> philosophers;


void create_bottle(philosopher *left, philosopher *right, int i, int j) {
	bottle *b = new bottle;
	b->i = i;
	b->j = j;
	sem_init(&(b->semaphore), 0, 1);
	left->bottles.push_back(b);
	right->bottles.push_back(b);
	printf("Added (%d, %d)\n", i, j);
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

	printf("Philosophers: %d\n", philosophers.size());
	
}

void choose_bottles(philosopher *p, bottle **chosen_bottles, int *chosen_bottle_count) {
}

void think() {
}

void drink(bottle **chosen_bottles, int chosen_bottle_count) {
}

void* philosopher_thread(void *t_id) {
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Incorrect number of command line arguments.\n");
		exit(EXIT_FAILURE);
	}

	parse_matrix(argv[1]);	

	return 0;
}
