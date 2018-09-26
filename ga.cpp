//
//  main.cpp
//  NIProject1
//
//  Created by Ian Squiers on 9/24/18.
//

#include <vector>
#include <iostream>
#include <algorithm>    // std::sort

struct Individual {
	double fitness;
	bool[] variables;
}

// function for sort to work on the Individual struct
bool compare_fitness (Individual ind1, Individual ind2) {
    return ind1.fitness < ind2.fitness;
}

const char * filename;
const int population_size ;
const char * selection;
const char * crossover;
const double cprobablity;
const double mprobablity;
const int generations;
const char * algorithm;
const int num_variables;

std::vector <Individual> population;
std::vector <Individual> breeders;

int ga(char* argv[]) {

    filename = argv[1];
    population = std::atoi(argv[2]);
    selection = argv[3];
    crossover = argv[4];
    cprobablity = std::atof(argv[5]);
    mprobablity = std::atof(argv[6]);
    generations = std::atoi(argv[7]);
    algorithm = argv(8);

}

void genetic() {

	// Generate Population
	for (int i; i < population_size; i++) {
		Individual new_ind;
		new_ind.variables = new bool[num_variables];
		for (int j; j < num_variables; j++) {
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
	fitness(individuals);
	// selection

}

void rank() {
    std::sort (population.begin(), population.end(), compare_fitness); // could also use partial_sort http: www.cplusplus.com/reference/algorithm/partial_sort/
    double sum = 0
    double probability;
    for (int i = 1; i < population.size(); i++) {
        sum += i;
        Individual temp = population.back();
        population.pop_back();
        probability = i/sum;
        double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
        if (r < probability) {
            breeders.push_back(temp);
        }
    }
}

void tournament() {
    int range = population.size();
    for (int i = 0; i < population_size / 2; i++) // check the division by 2
        int r1 = rand() % range;
        int r2 = rand() % range;
        while (r1 == r2) {
            int r2 = rand() % range;
            std::cout << "r1 = r2 and range is " << range;
        }
}

void fitness() {

}

void mutate( Individual ind ) {
	for (int i = 0; i < variables; ++i) {
		double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
		if (swap < mprobablity) {
			ind[i] = !ind[i];
		}
	}	
}

void one_point_crossover( Individual parent1, Individual parent2) {
	double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
	if (r < cprobablity) {
		int crossover_loc = rand() % variables; //not the best for random nums but works
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
		for (int i = 0; i < variables; ++i) {
			double swap = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
			if (swap < 0.5) {
				bool temp = parent1.variables[i];
				parent1.variables[i] = parent2.variables[i];
				parent2.variables[i] = temp;
			}
		}
	}
}

























