//
//  main.cpp
//
//
//  Created by Ian Squiers and Erik Wurman
//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include "pbil.cpp"
#include "ga2.cpp"



bool DEBUG_ON = false;

using namespace std;
int** parse(char* filename, int& numClauses, int& numVariables);
void printClauses(int** clauses, int numClauses);


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    cout << "filename: "<< filename << endl; // need for output
    if (DEBUG_ON){
        cout << argc << " arguments" << endl; 
    }
    int numClauses;
    int numVariables;
    int** clauses = parse(filename, numClauses, numVariables);
    cout << "numVariables: " << numVariables << endl; //need for output
    cout << "numClauses: " << numClauses << endl;
    if (DEBUG_ON){ 
        printClauses(clauses, numClauses);
    }
    
    // seed random so not always the same
    srand(time(0));


    if (!strcmp(argv[8],"p")){ 
        int numSamples = stoi(argv[2]);
        double posLR = stod(argv[3]);
        double negLR = stod(argv[4]);
        double mutateProb = stod(argv[5]);
        double mutateVal = stod(argv[6]);
        int numIterations = stoi(argv[7]);
        

        clock_t start = clock();

        pbil(numVariables, numClauses, clauses, numSamples, posLR, negLR, mutateProb, mutateVal, numIterations);
    

        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        cout << "PBIL took "<< duration << " seconds" << endl;
    }
    

    if (!strcmp(argv[8], "g")) {
        clock_t start = clock();
        ga(argv, clauses, numClauses, numVariables);

        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        cout << "GA took "<< duration << " seconds" << endl;

        /* 
        cout << endl << "Crossover = 0.2" << endl;
        argv[5] = (char*)"0.2";
        ga(argv, clauses, numClauses, numVariables);
        ga(argv, clauses, numClauses, numVariables);
        ga(argv, clauses, numClauses, numVariables);
        
        cout << endl << "Crossover = 0.5" << endl;
        argv[5] = (char*)"0.5";
        ga(argv, clauses, numClauses, numVariables);
        ga(argv, clauses, numClauses, numVariables);
        ga(argv, clauses, numClauses, numVariables);

        cout << endl << "Crossover = 0.9" << endl;
        argv[5] = (char*)"0.9";
        ga(argv, clauses, numClauses, numVariables);
        ga(argv, clauses, numClauses, numVariables);
        ga(argv, clauses, numClauses, numVariables);
        */
    }
    
    return 0;
}

/*
 * This is the parsing function for reading in the
 * .cnf files and returning our array of array of ints and
 * the number of clauses and number of variables
 */
int** parse(char* filename, int& numClauses, int& numVariables){
    // parse the file and return the array of each clause array
    // of bool indices.
    ifstream infile(filename);
    int **clauses = nullptr;
    numVariables = 0;
    numClauses = 0;
    bool initialized = false;
    int clauseNum = 0;
    string line;
    while (getline(infile, line)){
        if (line[0] == 'c'){
            continue; //lines that start with c are comments
        }
        if (line[0] == 'p'){
            // then the begining is "p cnf "
            int pcnf = line.find("p cnf ");
            if (DEBUG_ON){
                cout << "found line begining with p" <<endl;
                cout << line <<endl;
                cout << "pcnf int is: " << pcnf <<endl;
            }
            string subline = line.substr(pcnf+6);
            //six is length of "p cnf "
            int space = subline.find(" ");
            if (DEBUG_ON){
                cout << "substr of line is now:" <<endl;
                cout << subline << endl;
            }

            numVariables = stoi(subline.substr(0,space));
            numClauses = stoi(subline.substr(space));
            while (numClauses == 0){
                space = subline.find(" ");
                subline = subline.substr(space);
                numClauses = stoi(subline);
            }

            clauses = new int*[numClauses];
            initialized = true;
            if (DEBUG_ON){
                cout << "loop initialized" <<endl;
            }
        }
        else if (initialized){
            // grab each index from line,
            // then add that index to the end of the clause array.

            if (DEBUG_ON){
                cout << "in initialized else if" <<endl;
            }

            int varIndex = stoi( line.substr(0,line.find(" ")));
            int clauseIndex = 0;

            if(DEBUG_ON){
                cout << "starting the loop" <<endl;
            }
            //initialize this clause in memory:
            //first need to know how many numbers in it = numSpaces + 1
            int numSpaces = 0;
            for(int i =0; i < line.size(); i++){
                if (line[i] == ' '){
                    numSpaces++;
                }
            }

            if (DEBUG_ON){
                cout << "numSpaces = " << numSpaces << endl;
            }
            clauses[clauseNum] = new int[numSpaces+1];

            while (varIndex != 0) { //0 ends the line
                clauses[clauseNum][clauseIndex] = varIndex;
                clauseIndex++;
                line = line.substr(line.find(" ")+1);
                if (DEBUG_ON){
                    cout << "substr of line is now:" <<endl;
                    cout << line << endl;
                }
                varIndex = stoi( line.substr(0,line.find(" ")));
            }
            //Add -1 to end to denote end of the clause
            clauses[clauseNum][clauseIndex] = 0;
            clauseNum++;
        }

    }
    return clauses;
}


/*
 * Useful print function for debugging the parser
 */
void printClauses(int** clauses, int numClauses){
    cout << "printing clauses: numClauses = " << numClauses <<endl;

    for(int i = 0; i < numClauses; i++){
        //int* clause = clauses[i];
        int j = 0;
        int nextVar = clauses[i][j];
        //cout << "firstvar is " << nextVar;
        while (nextVar != 0){
            cout << nextVar << " ";
            j++;
            nextVar = clauses[i][j];
        }
        cout << clauses[i][j] << endl; 
    }
    cout << "Done printing clauses" <<endl;

}



