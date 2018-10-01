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
    cout << "sum = " << sum << endl;
    return sum;
}


// working, not technically probablities, really just bounds within 0 to 1 that correlate to the probabilities
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
    std::sort (pop.begin(), pop.end());
    cout << "Best fitenss: " << pop[pop.size()-1].fitness << " worst fitness: " << pop[0].fitness << endl;
    
    double * probability_of = probablities_by_rank();
    for (int i = 0; i < population_size; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        // cout << "random came up with : " << r << endl;
        for (int j = population_size; j > 0; j--) {
            if (r < probability_of[j]) {
                selected_pop.push_back(pop.at(population_size - j));
                // std::cout << "Selecting rank: " << j << endl;
                break;
            }
        }
    }
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

inline double * probabilities_by_boltz(vector<Individual> pop) {
    int sum = boltz_sum(pop);
    double * probability_of = new double [num_variables];
    for (int i = 0; i < pop.size() + 1; i++) {
        probability_of[i] = exp(pop[i].fitness) / sum;
    }
    return probability_of;
}

// working. fills next pop with selected individuals ready for breeding
inline vector<Individual> boltzmann_selection( vector<Individual> pop) {
    vector<Individual> selected;
    std::sort (pop.begin(), pop.end());
    double * probability_of = probabilities_by_boltz(pop);
    for (int i = 0; i < population_size; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        cout << "random came up with : " << r << endl;
        for (int j = population_size + 1; j > 0; j--) {
            if (r < probability_of[j]) {
                selected.push_back(pop.at(pop.size() - j));
                std::cout << "Selecting rank: " << j << endl;
                break;
            }
        }
    }
    return selected;
}

// ******************************************************************************************
// ****** Tournament Selection
// ******************************************************************************************

inline int * untaken_indecies(vector<Individual> pop) {
    int * untaken = new int[pop.size()];
    for (int i = 0; i < pop.size(); i++) {
        untaken[i] = 0;
    }
    return untaken;
}

inline vector<Individual> tournament( vector<Individual> pop ) {
    vector<Individual> new_pop;
    double pop_size = pop.size();
    int taken[pop.size()];
    for (int i =0; i < pop_size; i++) {
        taken[i] = 0;
    }
    for (int i = 0; i < (pop_size / 2); i++) {
        int r1 = rand() % pop.size();
        while (taken[r1]) {
            r1 = rand() % pop.size();
        }
        int r2 = rand() % pop.size();
        while (r1 == r2 || taken[r2]) {
            r2 = rand() % pop.size();
        }
        if (pop.at(r2) < pop.at(r1)) {
            new_pop.push_back(pop.at(r1));
        }
        else {
            new_pop.push_back(pop.at(r2));
        }
        taken[r1] = 1;
        taken[r1] = 1;
    }
    for (int i =0; i < pop_size; i++) {
        taken[i] = 0;
    }
    for (int i = 0; i < (pop_size / 2); i++) {
        int r1 = rand() % pop.size();
        while (taken[r1]) {
            r1 = rand() % pop.size();
        }
        int r2 = rand() % pop.size();
        while (r1 == r2 || taken[r2]) {
            r2 = rand() % pop.size();
        }
        if (pop.at(r2) < pop.at(r1)) {
            new_pop.push_back(pop.at(r1));
        }
        else {
            new_pop.push_back(pop.at(r2));
        }
        taken[r1] = 1;
        taken[r1] = 1;
    }
   return new_pop;
}


// ******************************************************************************************
// ****** Mutation & Crossover
// ******************************************************************************************
inline void print_ind( Individual ind ) {
    cout << "Individual with fitness " << ind.fitness << " and variables : " ;
    for (int j = 0; j < num_variables; j++) {
        cout << ind.variables[j] << ",";
    }
    cout << endl;
}

inline void free_mem( vector<bool *> freeme) {
    //remove duplicates
    vector<bool *>::iterator it;
    it = std::unique (freeme.begin(), freeme.end());
    freeme.resize(distance(freeme.begin(),it) );
    
    for (unsigned i = 0; i < freeme.size(); i++) {
        // delete[] freeme.at(i);
    }
    freeme.clear();
}

inline vector<Individual> mutate( vector<Individual> pop ) {
    vector<Individual> mutated_pop;
    vector<bool *> free_me;
    bool * temp;
    for (int it = 0; it < pop.size(); it++) {
        Individual ind = pop.at(it);
        temp = new (nothrow) bool[num_variables];
        //cout << "starting mutation at " << it;
        print_ind(ind);
        for (int i = 0; i < num_variables; ++i) {
            double r = (double)rand() / (double)RAND_MAX;
            cout << r << endl;
            if (r < mprobablity) {
                //cout << "mutating at " << i << endl;
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
        
        //cout << "end mutation : ";
        print_ind(ind);
        mutated_pop.push_back(ind);
        cout << endl << endl<< endl;
    }
    
    free_mem(free_me);
    return mutated_pop;
}

inline vector<Individual> one_point_crossover( vector<Individual> pop ) {
    vector<Individual> breeded_pop;
    vector<bool *> free_me;
    bool * temp1;
    bool * temp2;
    double pop_size = pop.size();
    for (int i = 0; i < (pop_size / 2); i++) {
        int r1 =  rand() % pop.size();
        int r2 =  rand() % pop.size();
        while (r2 == r1) {
            r2 = rand() % pop.size();
        }
        Individual parent1 = pop.at(r1);
        Individual parent2 = pop.at(r2);
        double r = (double)rand() / (double)RAND_MAX;
        if (r < cprobablity) { // run crossover
            cout << "crossing over" << endl;
            print_ind(parent1);
            print_ind(parent2);
            
            temp1 = new (nothrow) bool[num_variables];
            temp2 = new (nothrow) bool[num_variables];
            int crossover_loc = rand() % num_variables;
            cout << crossover_loc << endl;
            for (int j = 0; j < crossover_loc; j++) {
                temp1[j] = parent1.variables[j];
                temp2[j] = parent2.variables[j];
            }
            for (int j = crossover_loc; j < num_variables; j++) {
                temp1[j] = parent2.variables[j];
                temp2[j] = parent1.variables[j];
            }
            free_me.push_back(parent1.variables);
            free_me.push_back(parent2.variables);
            parent1.variables = temp1;
            parent2.variables = temp2;
            breeded_pop.push_back(parent1);
            breeded_pop.push_back(parent2);
            print_ind(parent1);
            print_ind(parent2);
            double first = max(r1, r2);
            double second = min(r1, r2);
            pop.erase(pop.begin()+first);
            pop.erase(pop.begin()+second);
        }
        else {
            breeded_pop.push_back(parent1);
            breeded_pop.push_back(parent2);
            double first = max(r1, r2);
            double second = min(r1, r2);
            pop.erase(pop.begin()+first);
            pop.erase(pop.begin()+second);
        }
    }
    free_mem(free_me);
    return breeded_pop;
}


inline vector<Individual> uniform_crossover( vector<Individual> pop ) {
    vector<Individual> breeded_pop;
    vector<bool *> free_me;
    bool * temp1;
    bool * temp2;
    double pop_size = pop.size();
    for (int i = 0; i < (pop_size / 2); i++) {
        int r1 =  rand() % pop.size();
        int r2 =  rand() % pop.size();
        while (r2 == r1) {
            r2 =rand() % pop.size();
        }
        Individual parent1 = pop.at(r1);
        Individual parent2 = pop.at(r2);
        double r = (double)rand() / (double)RAND_MAX;
        if (r < cprobablity) { // run crossover
            cout << "crossing over" << endl;
            print_ind(parent1);
            print_ind(parent2);
            
            temp1 = new (nothrow) bool[num_variables];
            temp2 = new (nothrow) bool[num_variables];
            
            for (int j = 0; j < num_variables; ++j) {
                r = (double)rand() / (double)RAND_MAX;
                if (r > 0.5) {
                    temp1[j] = parent1.variables[j];
                    temp2[j] = parent2.variables[j];
                }
                else {
                    temp1[j] = parent2.variables[j];
                    temp2[j] = parent1.variables[j];
                }
            }
            
            free_me.push_back(parent1.variables);
            free_me.push_back(parent2.variables);
            parent1.variables = temp1;
            parent2.variables = temp2;
            breeded_pop.push_back(parent1);
            breeded_pop.push_back(parent2);
            print_ind(parent1);
            print_ind(parent2);
            double first = max(r1, r2);
            double second = min(r1, r2);
            pop.erase(pop.begin()+first);
            pop.erase(pop.begin()+second);
        }
        else {
            breeded_pop.push_back(parent1);
            breeded_pop.push_back(parent2);
            double first = max(r1, r2);
            double second = min(r1, r2);
            pop.erase(pop.begin()+first);
            pop.erase(pop.begin()+second);
        }
    }
    free_mem(free_me);
    return breeded_pop;
}

inline void print_pop( vector<Individual> vect) {
    for (unsigned i = 0; i < vect.size(); i++) {
        cout << "Individual with fitness " << vect.at(i).fitness << " and variables : " ;
        for (int j = 0; j < num_variables; j++) {
            cout << vect.at(i).variables[j] << ",";
        }
        cout << endl;
    }
}

inline int max_fitness(vector<Individual> pop) {
    double max = 0;
    int max_index = 0;
    for (int i = 0; i < pop.size(); i++) {
        if (pop[i].fitness > max) {
            max = pop[i].fitness;
            max_index = i;
        }
    }
    return max_index;
}

inline Individual ga(char* argv[], int** clauses, int numClauses, int numVariables) {
    srand(time(0));
    
    Individual null;
    null.fitness = -1;
    
    filename = argv[1];
    population_size = std::atoi(argv[2]);
    selection = argv[3];
    crossover = argv[4];
    cprobablity = std::atof(argv[5]);
    mprobablity = std::atof(argv[6]);
    generations = std::atoi(argv[7]);
    algorithm = argv[8];
    num_variables = numVariables;
    
    vector<Individual> population = gen_one(population_size, num_variables);
    
    Individual best_ind;
    for (int i = 0; i < generations; i++) {
        
        // update fitnesses
        for (int j = 0; j < population_size; j++) {
            population[j].fitness = evaluateFitness(population[j].variables, numClauses, clauses);
            if (population[j].fitness > 0.99) {
                cout << "Solution Found in Generation " << i << endl;
                cout << "Highest fitness of final : " << population[j].fitness << ". With solution ";
                for (int i = 0; i < numVariables; i++) {
                    cout << population[j].variables[i] << ",";
                }
                return population[j];
            }
        }
        
        int curr_best = max_fitness(population);
        best_ind = population[curr_best];
        
        // selection
        if (!strcmp(selection, "rs")) {
            population = rank_selection(population);
        } else if (!strcmp(selection, "bs")) {
            population = boltzmann_selection(population);
        } else if (!strcmp(selection, "ts")) {
            population = tournament(population);
        } else {
            cout << "Enter valid selection method ( rs, ts, bs )" << endl;
            return null;
        }
        
        // breeding
        if (!strcmp(crossover, "1c")) {
            population = one_point_crossover(population);
        } else if (!strcmp(crossover, "uc")) {
            population = uniform_crossover(population);
        } else {
            cout << "Enter valid crossover method ( 1c, uc )" << endl;
            return null;
        }
        
        //mutation
        population = mutate(population);
    }
    cout << "Highest fitness of final : " << best_ind.fitness << ". With solution ";
    for (int i = 0; i < numVariables; i++) {
        cout << best_ind.variables[i] << ",";
    }
    return best_ind;
}




















