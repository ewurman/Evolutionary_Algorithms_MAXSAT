/* This is our PBIL MAXSAT Problem file
 * Authors: Erik Wurman and Ian Squiers
 *
 */

#include <vector>
#include "utils.cpp"


using namespace std;


//clock_t MAX_RUNTIME = 300 * CLOCKS_PER_SEC;
int MAX_ITERATIONS = 1000;


void pbil(int numBools, int numClauses, int** clauses, int numSamples){
    double* probabilities = new double[numBools];
    // initialize these all to be 0.5
    // A higher probability means more likely to be true
    for (int i = 0; i < numBools; i++){
        probabilities[i] = 0.5;
    }

    int i = 0;
    while (i < MAX_ITERATIONS){
        
        bool** samplesArray = new bool*[numSamples];
        for (int i=0; i < numSamples; i++){
            // we want to generate a sample based on the probabilities
            samplesArray[i] = new bool[numBools];
            for(int j=0; j < numBools; j++){
                double randNum = (double) rand() / RAND_MAX;
                if (randNum < probabilities[j]){
                    // set to false
                    samplesArray[i][j] = false;
                }
                else {
                    samplesArray[i][j] = true;
                }
            }

        } // end for loop creating samples



        i++;
    } //end while loop for iterations
    
}


