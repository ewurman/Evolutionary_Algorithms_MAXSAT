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

using namespace std;
int** parse(char* filename);

int main(int argc, char* argv[]) {
    char* filename = argv[1];
    int** clauses = parse(filename);


    if (*argv[8] == 'g') {
        ga(argv);
    }
    else {
        pbil(0);
    }
    return 0;
}


int** parse(char* filename){
    // parse the file and return the array of each clause array
    // of bool indices.
    ifstream infile(filename);
    int **clauses;
    int numVariables = 0;
    int numClauses = 0;
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
            line = line.substr(pcnf);
            //line >> p >> cnf >> numVariables >> numClauses;
            int space = line.find(" ");
            numVariables = stoi( line.substr(0,space));
            numClauses = stoi( line.substr(space));
            
            clauses = new int*[numClauses];
            initialized = true;
        }
        else if (initialized){ 
            // grab each index from line, subtract by 1 (they are 1-based)
            // then add that index to the end of the clause array.
            int varIndex = stoi( line.substr(0,line.find(" "))) - 1;
            int clauseIndex = 0; 
            while (varIndex != -1) { //0 ends the line and we subtract 1
                clauses[clauseNum][clauseIndex] = varIndex;
                clauseIndex++;
                line = line.substr(line.find(" "));
                varIndex = stoi( line.substr(0,line.find(" "))) - 1;
            }
        }
        clauseNum++;
    }
    return clauses;
}







