// This is our utils file.
// It contains the evaluation function for both GA and PBIL.
//
// Authors: Erik Wurman and Ian Squiers
//



#ifndef UTILS_CPP
#define UTILS_CPP

#include <vector>
#include <iostream>

using namespace std;

typedef struct Individual {
    double fitness;
    bool* variables;

    bool operator< (const Individual &rightInd) const {
        //compares fitness
        return fitness < rightInd.fitness;
    }

} Individuals;




inline double evaluateFitness(bool* vars, int numClauses, int** clauses){
    int score = 0;

    for (int i = 0; i < numClauses; i++){
        bool clauseEval = false;
        int j = 0;
        while (clauses[i][j] != 0){
            int index = clauses[i][j];
            bool negate = index < 0;
            if (negate){
                index *= -1; //need it to be an index
                clauseEval = clauseEval || !vars[index - 1];
            }
            else {
                // clauses has 1-based indices, vars 0-based so subtract 1
                clauseEval = clauseEval || vars[index - 1];
            }
            j++;
        }
        if (clauseEval){
            score++;
        }
    }
    
    return (double)score / (double)numClauses;
}



inline double evaluateFitness2(bool* vars, int numClauses, int** clauses){
    int score = 0;

    for (int i = 0; i < numClauses; i++){
        bool clauseEval = false;
        int j = 0;
        while (clauses[i][j] != 0){
            int index = clauses[i][j];
            bool negate = index < 0;
            if (negate){
                index *= -1; //need it to be an index
                clauseEval = clauseEval || !vars[index -1];
            }
            else {
                // clauses has 1-based indices, vars 0-based so subtract 1
                clauseEval = clauseEval || vars[index - 1];
            }
            j++;
        }
        if (clauseEval){
            score++;
        }
    }
    
    cout << "score: " << score << "    numClauses: " << numClauses << endl;
    return (double)score / (double)numClauses;
}



inline void findMaxMinIndividualIndex(vector<Individual> samples, int& max, int& min, int numSamples){
    max = 0;
    min = 0;
    for (int i = 1; i < numSamples; i++){
        if (samples[i] < samples[min]){
            min = i;
        } else if (samples[max] < samples[i]){
            max = i;
        }
    }
}

inline void printProbabilityVector(double* probs, int numBools){
    cout << probs[0];
    for (int i = 1; i < numBools; i++){
        cout << " " << probs[i];
    }
    cout << endl;
}

inline void printIndividualAsProbabilities(Individual sample, int numBools){
    for (int i = 0; i < numBools; i++){
        if(sample.variables[i]){
            cout << "1 ";
        } else {
            cout << "0 ";
        }
    }
    cout << endl;
}


inline Individual copyForBreeding(Individual sample, int numBools) {
    Individual copy;
    copy.fitness = sample.fitness;
    bool* vars = new bool[numBools];
    for (int i = 0; i < numBools; i++){
        vars[i] = sample.variables[i];
    }
    copy.variables = vars;
    return copy;
}

inline void free_mem( vector<Individual> freeme) {
    for (unsigned i = 0; i < freeme.size(); i++) {
        delete[] freeme[i].variables;
        // is this necessary? idk
        freeme[i].variables = nullptr;
    }
    
}

inline void print_ind( Individual ind, int num_vars ) {
    cout << "Individual with fitness " << ind.fitness << " and variables : " ;
    for (int j = 0; j < num_vars; j++) {
        cout << ind.variables[j] << ",";
    }
    cout << endl;
}

inline void print_pop( vector<Individual> pop, int num_vars) {
    for (unsigned i = 0; i < pop.size(); i++) {
        print_ind(pop[i], num_vars);
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

#endif

