/* This is our PBIL MAXSAT Problem file
 * Authors: Erik Wurman and Ian Squiers
 *
 */

#include <vector>
#include "utils.cpp"


using namespace std;


//clock_t MAX_RUNTIME = 300 * CLOCKS_PER_SEC;
int MAX_ITERATIONS = 1000;


void pbil(int numBools, int numClauses, int** clauses, int numSamples, double posLR, double negLR, double mutateProb, double mutateVal, int iterations){
    cout << "Entered PBIL algorithm" <<endl;
    double* probabilities = new double[numBools];
    // initialize these all to be 0.5
    // A higher probability means more likely to be true
    for (int i = 0; i < numBools; i++){
        probabilities[i] = 0.5;
    }

    int i = 0;
    while (i < iterations){
        vector<Individual>* samplesVector = new vector<Individuals>(numSamples);
        //bool** samplesArray = new bool*[numSamples];
        for (int i=0; i < numSamples; i++){
            // we want to generate a sample based on the probabilities
            Individual sample;
            bool* vars = new bool[numBools];
             
            for(int j=0; j < numBools; j++){
                double randNum = (double) rand() / RAND_MAX;
                if (randNum < probabilities[j]){
                    // set to false
                    vars[j] = false;
                }
                else {
                    vars[j] = true;
                }
            }
            double fitness = evaluateFitness(vars, numClauses, clauses);
            sample.fitness = fitness;
            sample.variables = vars;
            samplesVector->push_back(sample);
        } // end for loop creating samples

         
        Individual best;//= **max_element(samplesVector, samplesVector+numSamples);
        Individual worst;//= **min_element(samplesVector, samplesVector+numSamples);
        findMaxMinIndividual(*samplesVector, best, worst, numSamples);
        // now update the probability vectors based on these; 
        for(int j = 0; j < numBools; j++){
            if (best.variables[j] != worst.variables[j]){
                // we want to update away from the worst
                probabilities[j] = probabilities[j]*(1-negLR) + best.variables[j]*negLR;
            }
            // do nothing but towards the best otherwise
            probabilities[j] = probabilities[j]*(1-posLR) + best.variables[j]*posLR;
        }

        //NOW we mutate.
        for (int j = 0; j < numBools; j++){
            double randNum = (double) rand() / RAND_MAX;
            if (randNum < mutateProb){
                double randNum2 = (double) rand() / RAND_MAX;
                int mutateDir = 0;
                if (randNum2 > 0.5){
                    mutateDir = 1;
                }
                probabilities[j] = probabilities[j]*(1-mutateVal)+mutateDir*mutateVal;
            }

        }

        i++;
    } //end while loop for iterations
    
    //NOW round probabilities and evaluate and return score?
    bool* finalResult = new bool[numBools];
    for (int i = 0; i < numBools; i++){
        if (probabilities[i] >= 0.5){
            finalResult[i] = true;
        }
        else{
            finalResult[i] = false;
        }
    }
    double score = evaluateFitness(finalResult, numClauses, clauses);
    cout << "PBIL finished with a fitness of " << score << endl;
}


