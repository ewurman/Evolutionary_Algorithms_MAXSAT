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
#include "utils.cpp"

using namespace std;

static char * filename;
static int population_size;
static char * selection;
static char * crossover;
static double cprobablity;
static double mprobablity;
static int generations;
static char * algorithm;
static int num_variables;


//string filename;
//static int population_size;
//string selection;
//string crossover;
//static double cprobablity;
//static double mprobablity;
//static int generations;
//string * algorithm;
//static int num_variables;


inline vector<Individual> gen_one(int population_size, int num_variables) {
    // Generate Population
    vector<Individual> pop;
    bool * temp;
    for (int i = 0; i < population_size; i++) {
        Individual new_ind;
        temp = new (nothrow) bool[num_variables];
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
        pop.push_back(new_ind);
    }
    return pop;
}

// ******************************************************************************************
// ****** Rank Selection
// ******************************************************************************************


// working
inline double sum_to(int num) {
    double sum = 0;
    for (int i = 0; i < num + 1; i++) {
        sum += i;
    }
    //cout << "sum = " << sum << endl;
    return sum;
}


// not technically probablities, really just bounds within 0 to 1 that correlate to the probabilities
inline double * probablities_by_rank() {
    double sum = sum_to(population_size);
    double last_pos = 0;
    double curr_bound = 0;
    double * probability_of = new double [population_size + 1]; // no such thing as rank 0
    for (int i = population_size; i > 0; i--) {
        curr_bound = (i+last_pos) / sum;
        probability_of[i] = curr_bound;
        last_pos = last_pos + i;
    }
    return probability_of;
}


// fills next pop with selected individuals ready for breeding
inline vector<Individual> rank_selection( vector<Individual> pop ) {
    
    vector<Individual> selected_pop;
    
    // sort the population fittest to least fit
    std::sort (pop.begin(), pop.end());
    cout << "Best fitenss: " << pop[pop.size()-1].fitness << " worst fitness: " << pop[0].fitness << endl;
    
    double * probability_of = probablities_by_rank();
    for (int i = 0; i < population_size; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        // cout << "random came up with : " << r << endl;
        for (int j = population_size-1; j >= 0; j--) {
            if (r < probability_of[j]) {
                selected_pop.push_back(copyForBreeding(pop[j], num_variables));
                // std::cout << "Selecting rank: " << j << endl;
                break;
            }
        }
    }
    delete[] probability_of;
    free_mem(pop);
    return selected_pop;
}

// ******************************************************************************************
// ****** Boltzmann Selection
// ******************************************************************************************


inline double boltz_sum(vector<Individual> pop) {
    double sum = 0;
    for (unsigned i = 0; i < pop.size(); i++) {
        sum += exp(pop[i].fitness);
    }
    return sum;
}
inline double * probablities_by_boltz(vector<Individual> pop) {
    double sum = boltz_sum(pop);
    double last_pos = 0;
    double curr_bound = 0;
    double * probability_of = new double [population_size + 1]; // no such thing as rank 0
    for (int i = population_size; i > 0; i--) {
        curr_bound = last_pos + (exp(pop[i].fitness) / sum);
        probability_of[i] = curr_bound;
        last_pos = curr_bound;
    }
    
    return probability_of;
}


// fills next pop with selected individuals ready for breeding
inline vector<Individual> boltzmann_selection( vector<Individual> pop ) {
    vector<Individual> selected_pop;
    
    // sort the population fittest to least fit
    std::sort (pop.begin(), pop.end());
    cout << "Best fitenss: " << pop[pop.size()-1].fitness << " worst fitness: " << pop[0].fitness << endl;
    
    double * probability_of = probablities_by_boltz(pop);
    for (int i = 0; i < population_size; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        for (int j = population_size-1; j >= 0; j--) {
            if (r < probability_of[j]) {
                selected_pop.push_back(copyForBreeding(pop[j], num_variables));
                break;
            }
            if (j == population_size) {
                cout << "I should never get here" << endl;
            }
        }
    }
    delete[] probability_of;
    free_mem(pop);
    return selected_pop;
}

// ******************************************************************************************
// ****** Tournament Selection
// ******************************************************************************************


inline vector<Individual> tournament( vector<Individual> pop ) {
    vector<Individual> new_pop;
    double pop_size = pop.size();
    
    // need to
    int taken[pop.size()];
    for (int i =0; i < pop_size; i++) {
        taken[i] = 0;
    }
    
    // ideally make this a function so not copied code... holding off for time
    for (int i = 0; i < (pop_size / 2); i++) {
        
        // ensure everyone is selected
        int r1 = rand() % pop.size();
        while (taken[r1]) {
            r1 = rand() % pop.size();
        }
        int r2 = rand() % pop.size();
        while (r1 == r2 || taken[r2]) {
            r2 = rand() % pop.size();
        }
        
        // competition
        if (pop[r2] < pop[r1]) {
            new_pop.push_back(copyForBreeding(pop[r1], num_variables));
        }
        else {
            new_pop.push_back(copyForBreeding(pop[r2], num_variables));
        }
        taken[r1] = 1;
        taken[r2] = 1;
    }
    
    //reset taken for a full set of
    for (int i =0; i < pop_size; i++) {
        taken[i] = 0;
    }
    
    // get another set of tournaments
    for (int i = 0; i < (pop_size / 2); i++) {
        int r1 = rand() % pop.size();
        while (taken[r1]) {
            r1 = rand() % pop.size();
        }
        int r2 = rand() % pop.size();
        while (r1 == r2 || taken[r2]) {
            r2 = rand() % pop.size();
        }
        if (pop[r2] < pop[r2]) {
            new_pop.push_back(copyForBreeding(pop[r1], num_variables));
        }
        else {
            new_pop.push_back(copyForBreeding(pop[r2], num_variables));
        }
        taken[r1] = 1;
        taken[r2] = 1;
    }
    free_mem(pop);
    return new_pop;
}


// ******************************************************************************************
// ****** Mutation & Crossover
// ******************************************************************************************


inline vector<Individual> mutate( vector<Individual> pop ) {
    vector<Individual> mutated_pop;
    for (int i = 0; i < pop.size(); i++) {
        Individual temp = copyForBreeding(pop[i], num_variables);
        for (int j = 0; j < num_variables; ++j) {
            double r = (double)rand() / (double)RAND_MAX;
            if (r < mprobablity) {
                // flip bool
                temp.variables[j] = !(pop[i].variables[j]);
            }
        }
        mutated_pop.push_back(temp);
    }
    free_mem(pop);
    return mutated_pop;
}

inline vector<Individual> one_point(vector<Individual> pop) {
    vector<Individual> breeded_pop;
    int pop_size = (int)pop.size();
    
    int taken[pop.size()];
    for (int i =0; i < pop_size; i++) {
        taken[i] = 0;
    }
    
    for (int i = 0; i < (pop_size / 2); i++) {
        
        // select individuals that have not been crossed over
        int r1 = rand() % pop_size;
        while (taken[r1]) {
            r1 = rand() % pop_size;
        }
        int r2 = rand() % pop_size;
        while (r1 == r2 || taken[r2]) {
            r2 = rand() % pop_size;
        }
        
        Individual parent1 = copyForBreeding(pop[r1], num_variables);
        Individual parent2 = copyForBreeding(pop[r2], num_variables);
        
        double r = (double)rand() / (double)RAND_MAX;
        
        if (r < cprobablity) { // run crossover
            //cout << "crossing over" << endl;
            int crossover_loc = rand() % num_variables;
            for (int j = 0; j < crossover_loc; j++) {
                bool temp = parent1.variables[j];
                parent1.variables[j] = parent2.variables[j];
                parent2.variables[j] = temp;
            }
        }
        breeded_pop.push_back(parent1);
        breeded_pop.push_back(parent2);
        taken[r1] = 1;
        taken[r2] = 1;
    }
    free_mem(pop);
    return breeded_pop;
}

inline vector<Individual> uniform(vector<Individual> pop) {
    vector<Individual> breeded_pop;
    int pop_size = (int)pop.size();
    
    int taken[pop.size()];
    for (int i =0; i < pop_size; i++) {
        taken[i] = 0;
    }
    
    for (int i = 0; i < (pop_size / 2); i++) {
        
        // select individuals that have not been crossed over
        int r1 = rand() % pop_size;
        while (taken[r1]) {
            r1 = rand() % pop_size;
        }
        int r2 = rand() % pop_size;
        while (r1 == r2 || taken[r2]) {
            r2 = rand() % pop_size;
        }
        
        Individual parent1 = copyForBreeding(pop[r1], num_variables);
        Individual parent2 = copyForBreeding(pop[r2], num_variables);
        
        // swap each variable with 50/50 chance
        for (int j = 0; j < num_variables; j++) {
            double r = (double)rand() / (double)RAND_MAX;
            if (r > 0.5) {
                bool temp = parent1.variables[j];
                parent1.variables[j] = parent2.variables[j];
                parent2.variables[j] = temp;
            }
        }
        breeded_pop.push_back(parent1);
        breeded_pop.push_back(parent2);
    }
    free_mem(pop);
    return breeded_pop;
}





inline Individual ga(char* args[], int** clauses, int numClauses, int numVariables) {
    srand((unsigned)time(0));
    cout << "here111" << endl;

    Individual null;
    null.fitness = -1;
    
    filename = args[1];
    population_size = std::atoi(args[2]);
    selection = args[3];
    crossover = args[4];
    cprobablity = std::atof(args[5]);
    mprobablity = std::atof(args[6]);
    generations = std::atoi(args[7]);
    algorithm = args[8];
    num_variables = numVariables;
    
    vector<Individual> population = gen_one(population_size, num_variables);
    
    // Individual global_best;
    Individual best_ind;
    for (int gen = 0; gen < generations; gen++) {
        
        // update fitnesses
        for (int i = 0; i < population_size; i++) {
            population[i].fitness = evaluateFitness(population[i].variables, numClauses, clauses);
            
            // if solution found
            if (population[i].fitness > 0.99) {
                cout << "Solution Found in Generation " << gen << endl;
                cout << "Highest fitness of final : " << population[i].fitness << ". With solution ";
                for (int k = 0; k < numVariables; k++) {
                    cout << population[i].variables[k] << ",";
                }
                return population[i];
            }
        }
        
        int curr_best = max_fitness(population);
        best_ind = population[curr_best];
        
        // selection
        if (!strcmp(selection, "rs")) {
            population = rank_selection(population);
        } else if (!strcmp(selection, "bs")) {
            cout << "here" << endl;
            population = boltzmann_selection(population);
        } else if (!strcmp(selection, "ts")) {
            cout << "here" << endl;
            population = tournament(population);
        } else {
            cout << "Enter valid selection method ( rs, ts, bs )" << endl;
            return null;
        }
        
        // breeding
        if (!strcmp(crossover, "1c")) {
            population = one_point(population);
        } else if (!strcmp(crossover, "uc")) {
            population = uniform(population);
        } else {
            cout << "Enter valid crossover method ( 1c, uc )" << endl;
            return null;
        }
        
        //mutation
        population = mutate(population);
        int pop_size = population.size();
        print_ind(best_ind, numVariables);
    }
    return best_ind;
}






