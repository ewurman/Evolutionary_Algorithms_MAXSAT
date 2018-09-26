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


using namespace std;


main(int argc, char* argv[]) {
    char* filename = argv[1];
    int** clauses = parse(filename);


    if (argv[8] == 'g') {
        ga(argv);
    }
    else {
        pbil(argv);
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
    while (getline(infile, line)){
        if (line[0] = 'c'){
            continue; //lines that start with c are comments
        }
        if (line[0] == 'p'){
            char p;
            char* cnf;
            line >> p >> cnf >> numVariables >> numClauses;
            clauses = new int*[numClauses];
            initialized = true;
        }
        else if (initialized){ 
            // grab each index from line, subtract by 1 (they are 1-based)
            // then add that index to the end of the clause array.
            int varIndex = (int) line.substr(0,str.find(" ")) - 1;
            int clauseIndex = 0; 
            while (varIndex != -1) { //0 ends the line and we subtract 1
                clauses[clauseNum][clauseIndex] = varIndex;
                clauseIndex++;
                line = line.substr(str.find(" ");
                varIndex = (int) line.substr(0,str.find(" ")) - 1;
            }
        }
        clauseNum++;
    }
    return clauses;
}







