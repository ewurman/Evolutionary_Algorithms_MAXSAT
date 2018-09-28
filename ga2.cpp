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
#include <math.h>

using namespace std;

struct Individual {
    int id;
    double fitness;
    bool * variables;
};

// 0 is best rank, N is worst
bool compare_fitness(Individual ind1, Individual ind2) {
    return ind1.fitness > ind2.fitness;
}

char * filename;
int population_size = 4;
char * selection;
char * crossover;
double cprobablity;
double mprobablity = .5;
int generations;
char * algorithm;
int num_variables = 3;

std::vector <Individual> population;
std::vector <Individual> next_pop;

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

void gen_one(int num_variables) {
    // Generate Population
    bool * temp;
    for (int i = 0; i < population_size; i++) {
        Individual new_ind;
        temp = new bool[num_variables];
        new_ind.variables = temp;
        
        for (int j = 0; j < num_variables; j++) {
            double r = (double)rand() / (double)RAND_MAX; //not the best for random nums but works
            if (r > .5) {
                new_ind.variables[j] = true;
            }
            else {
                new_ind.variables[j] = false;
            }
            new_ind.fitness = r;
            new_ind.id = i;
        }
        population.push_back(new_ind);
    }
}

// ******************************************************************************************
// ****** Rank Selection
// ******************************************************************************************


// // working
// double sum_to(int num) {
//     double sum = 0;
//     for (int i = 0; i < num + 1; i++) {
//         sum += i;
//     }
//     cout << "sum = " << sum << endl;
//     return sum;
// }


// // working, not technically probablities, really just bounds within 0 to 1 that correlate to the probabilities
// double * probablities_by_rank() {
//     double sum = sum_to(population_size);
//     double last_pos = 0;
//     double curr_bound = 0;
//     double * probability_of = new double [population_size + 1]; // no such thing as rank 0
//     for (int i = population_size; i > 0; i--) {
//         curr_bound = (i+last_pos) / sum;
//         probability_of[i] = curr_bound;
//         last_pos = last_pos + i;
//     }
//     return probability_of;
// }


// // working. fills next pop with selected individuals ready for breeding
// void rank_selection() {
//     std::sort (population.begin(), population.end(), compare_fitness);
//     double * probability_of = probablities_by_rank();
//     for (int i = 0; i < population_size; i++) {
//         double r = (double)rand() / (double)RAND_MAX;
//         cout << "random came up with : " << r << endl;
//         for (int j = population_size + 1; j > 0; j--) {
//             if (r < probability_of[j]) {
//                 next_pop.push_back(population.at(population_size - j));
//                 std::cout << "Selecting rank: " << j << endl;
//                 break;
//             }
//         }
//     }
// }

// ******************************************************************************************
// ****** Boltzmann Selection
// ******************************************************************************************


// double boltz_sum() {
//     double sum = 0;
//     for (unsigned i = 0; i < population_size; i++) {
//         sum += exp(population[i].fitness);
//     }
//     return sum;
// }

// double * probabilities_by_boltz() {
//     int sum = boltz_sum();
//     double * probability_of = new double [num_variables];
//     for (int i = 0; i < population_size + 1; i++) {
//         probability_of[i] = exp(population[i].fitness) / sum;
//     }
//     return probability_of;
// }

// // working. fills next pop with selected individuals ready for breeding
// void boltzmann_selection() {
//     std::sort (population.begin(), population.end(), compare_fitness);
//     double * probability_of = probablities_by_rank();
//     for (int i = 0; i < population_size; i++) {
//         double r = (double)rand() / (double)RAND_MAX;
//         cout << "random came up with : " << r << endl;
//         for (int j = population_size + 1; j > 0; j--) {
//             if (r < probability_of[j]) {
//                 next_pop.push_back(population.at(population_size - j));
//                 std::cout << "Selecting rank: " << j << endl;
//                 break;
//             }
//         }
//     }
// }

// ******************************************************************************************
// ****** Tournament Selection
// ******************************************************************************************


vector<Individual> tournament( vector<Individual> pop ) {
    vector<Individual> new_pop;
    for (int i = 0; i < pop.size(); i++) {
        int r1 = rand() % pop.size();
        int r2 = rand() % pop.size();
        while (r1 == r2) {
            r2 = rand() % pop.size();
        }
        if (compare_fitness(pop.at(r1), pop.at(r2))) {
            new_pop.push_back(pop.at(r1));
        }
        else {
            new_pop.push_back(pop.at(r2));
        }
    }
    return new_pop;
}


// ******************************************************************************************
// ****** Mutation & Crossover
// ******************************************************************************************
void print_ind( Individual ind ) {
    cout << "  IIndividual " << ind.id << " with fitness " << ind.fitness << " and variables : " ;
    for (int j = 0; j < num_variables; j++) {
        cout << ind.variables[j] << ",";
    }
    cout << endl;
}

void free_mem( vector<bool *> freeme) {
    //remove duplicates
    vector<bool *>::iterator it;
    it = std::unique (freeme.begin(), freeme.end());
    freeme.resize(distance(freeme.begin(),it) );
    
    for (unsigned i = 0; i < freeme.size(); i++) {
        delete[] freeme.at(i);
    }
    freeme.clear();
}

 vector<Individual> mutate( vector<Individual> pop ) {
     vector<Individual> mutated_pop;
     vector<bool *> free_me;
     bool * temp;
     for (int it = 0; it < pop.size(); it++) {
         Individual ind = pop.at(it);
         temp = new bool[num_variables];
         
         cout << "starting mutation at " << it;
         print_ind(ind);
         for (int i = 0; i < num_variables; ++i) {
             double r = (double)rand() / (double)RAND_MAX;
             cout << r << endl;
             if (r < mprobablity) {
                 cout << "mutating " << "  " << ind.id << " at " << i << endl;
                 temp[i] = !(ind.variables[i]);
             }
             else {
                 temp[i] = (ind.variables[i]);
             }
         }
         // free memory of old variables
         
         free_me.push_back(ind.variables);
         ind.variables = temp;
         
//         delete[] temp;
//         temp = nullptr;
         
         cout << "end mutation : ";
         print_ind(ind);
         mutated_pop.push_back(ind);
         cout << endl << endl<< endl;
     }
     free_mem(free_me);
     return mutated_pop;
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

void print_pop( vector<Individual> vect) {
    for (unsigned i = 0; i < vect.size(); i++) {
        cout << "Individual " << vect.at(i).id << " with fitness " << vect.at(i).fitness << " and variables : " ;
        for (int j = 0; j < num_variables; j++) {
            cout << vect.at(i).variables[j] << ",";
        }
        cout << endl;
    }
}



int main() {
    srand(time(0));
    gen_one(num_variables);
    
    print_pop(population);
    cout << endl << endl << endl;
    
    population = tournament(population);
    print_pop(population);
    cout << endl << endl<< endl;

    population = mutate(population);
    print_pop(population);
    cout << endl << endl << endl;
    
    
    
    cout << endl;
    
    return 0;
}





















