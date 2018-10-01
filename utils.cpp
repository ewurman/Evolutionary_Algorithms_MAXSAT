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

void printProbabilityVector(double* probs, int numBools){
    cout << probs[0];
    for (int i = 1; i < numBools; i++){
        cout << " " << probs[i];
    }
    cout << endl;
}


#endif

