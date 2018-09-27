//
//  main.cpp
//  
//
//  Created by Ian Squiers on 9/25/18.
//

#include <stdio.h>
#include <fstream>
#include "pbil.cpp"
#include "ga.cpp"
#include <sstream>


bool DEBUG_ON = false;

using namespace std;
int** parse(char* filename, int& numClauses);
void printClauses(int** clauses, int numClauses);


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    int numClauses;
    int** clauses = parse(filename, numClauses);
    if (DEBUG_ON){
        cout << "numClauses: " << numClauses << endl; 
        printClauses(clauses, numClauses);
    }
    
    /*
    if (*argv[8] == 'g') {
        ga(argv);
    }
    else {
        pbil(0);
    }
    */
    return 0;
}


int** parse(char* filename, int& numClauses){
    // parse the file and return the array of each clause array
    // of bool indices.
    ifstream infile(filename);
    int **clauses;
    int numVariables = 0;
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
            // grab each index from line, subtract by 1 (they are 1-based)
            // then add that index to the end of the clause array.
            
            if (DEBUG_ON){
                cout << "in initialized else if" <<endl; 
            }
            
            int varIndex = stoi( line.substr(0,line.find(" "))) - 1;
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

            while (varIndex != -1) { //0 ends the line and we subtract 1
                clauses[clauseNum][clauseIndex] = varIndex;
                clauseIndex++;
                line = line.substr(line.find(" ")+1); 
                if (DEBUG_ON){
                    cout << "substr of line is now:" <<endl;
                    cout << line << endl; 
                }
                varIndex = stoi( line.substr(0,line.find(" "))) - 1;
            }
            //Add -1 to end to denote end of the clause
            clauses[clauseNum][clauseIndex] = -1;
            clauseNum++;
        }
        
    }
    return clauses;
}


//TODO: test parsing works by writing a debug printer for clauses
void printClauses(int** clauses, int numClauses){
    cout << "printing clauses: numClauses = " << numClauses <<endl;
    
    for(int i = 0; i < numClauses; i++){
        //int* clause = clauses[i]; 
        int j = 0;
        int nextVar = clauses[i][j];
        //cout << "firstvar is " << nextVar;
        while (nextVar != -1){
            cout << nextVar << " ";
            j++;
            nextVar = clauses[i][j];
        }
        cout << clauses[i][j] << endl;

    }

}



