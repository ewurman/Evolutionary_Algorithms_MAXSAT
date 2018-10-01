// This is our utils file.
// It contains the evaluation function for both GA and PBIL.
//
// Authors: Erik Wurman and Ian Squiers
//



#ifndef UTILS_CPP
#define UTILS_CPP

#include <vector>

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
    
    return (double)score / (double)numClauses;
}


inline void findMaxMinIndividual(vector<Individual> samples, Individual& max, Individual& min, int numSamples){
    max = samples[0];
    min = samples[0];
    for (int i = 1; i < numSamples; i++){
        if (samples[i] < min){
            min = samples[i];
        } else if (max < samples[i]){
            max = samples[i];
        }
    }
}



#endif

