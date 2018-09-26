//
//  main.cpp
//  NIProject1
//
//  Created by Ian Squiers on 9/24/18.
//

#include <vector>
#include <iostream>
#include <algorithm>    // std::sort
#include <string>

struct Individual {
	double fitness;
    bool * variables;
};

// function for sort to work on the Individual struct
bool compare_fitness (Individual ind1, Individual ind2) {
    return ind1.fitness > ind2.fitness;
}

char * filename;
int population_size;
char * selection;
char * crossover;
double cprobablity;
double mprobablity;
int generations;
char * algorithm;
int num_variables;

std::vector <Individual> population;

void ga(char* argv[]) {

    filename = argv[1];
    population_size = std::atoi(argv[2]);
    selection = argv[3];
    crossover = argv[4];
    cprobablity = std::atof(argv[5]);
    mprobablity = std::atof(argv[6]);
    generations = std::atoi(argv[7]);
    algorithm = argv[8];

}

void genetic(int num_variables) {
	// Generate Population
    
	for (int i = 0; i < population_size; i++) {
		Individual new_ind;
        bool * temp = new bool[num_variables];
        new_ind.variables = temp;
		for (int j = 0; j < num_variables; j++) {
			double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
			if (r > .5) {
				new_ind.variables[j] = true;
			}
			else {
				new_ind.variables[j] = false;
			}
		}
		population.push_back(new_ind);
	}
	// selection

}

void rank() {
    std::sort (population.begin(), population.end(), compare_fitness); // could also use partial_sort http: www.cplusplus.com/reference/algorithm/partial_sort/
    double sum = 0;
    double probability;
    for (int i = 1; i < population.size(); i++) {
        sum += i;
        Individual temp = population.back();
        population.pop_back();
        probability = i/sum;
        double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
        if (r < probability) {
        }
    }
}

void tournament() {
    for (int i = 0; i < population_size / 2; i++) { // check the division by 2
        int r1 = rand() % population.size();
        int r2 = rand() % population.size();
        while (r1 == r2) {
            r2 = rand() % population.size();
            std::cout << "r1 = r2 and range is " << population.size();
        }
        if (compare_fitness(population.at(r1), population.at(r2))) {
        }
    }
}

void fitness() {

}

void mutate( Individual ind ) {
	for (int i = 0; i < num_variables; ++i) {
		double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
		if (r < mprobablity) {
			ind.variables[i] = !ind.variables[i];
		}
	}	
}

void one_point_crossover( Individual parent1, Individual parent2) {
	double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
	if (r < cprobablity) {
		int crossover_loc = rand() % num_variables; //not the best for random nums but works
		bool temp [crossover_loc];
		for (int i = 0; i < crossover_loc; ++i) {
			temp[i] = parent1.variables[i];
			parent1.variables[i] = parent2.variables[i];
			parent2.variables[i] = temp[i];
		}
	}
}

void uniform_crossover( Individual parent1, Individual parent2) {
	double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
	if (r < cprobablity) {
		for (int i = 0; i < num_variables; ++i) {
			double swap = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
			if (swap < 0.5) {
				bool temp = parent1.variables[i];
				parent1.variables[i] = parent2.variables[i];
				parent2.variables[i] = temp;
			}
		}
	}
}

























