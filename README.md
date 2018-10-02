# Evolutionary_Algorithms_MAXSAT
Nature Inspired Computation project 1
By Erik Wurman and Ian Squiers

__Description__:
This project compares two nature inspired algorithms, a genetic algorithm and a Population Based Incremental Learning algorithm. The goal was to determine which is better to solve the NP-Complete MAXSAT problem.

__How To Run__:
To run this project, compile the main.cpp file like g++ main.cpp -o evoAlgs. Then you can run ./evoAlgs:

There are 8 total arguments necessary to run either algorithm. there is no error handling... so make sure you do it correctly.

To run GA:
    The first parameter is the .cnf file containing the MAXSAT prooblem you want to solve.
    The second parameter is the number of individuals in the population.
    The third parameter is the selection method. 'rs' for rank selection, 'ts' for tournament selection, and 'bs' for Boltzmann selection.
    The fourth parameter is the crossover method to use. '1c' for one-point crossover and 'uc' for uniform crossover.
    The fifth parameter is the probability of crossover. A recomendation is 0.7
    The sixth parameter is the probability of mutation. A recomendation is 0.01
    The seventh parameter is the number of generations. 2000 is good.
    The last parameter is 'g' to signal you want the genetic algorithm.


To run PBIL: 
    The first parameter is the .cnf file containing the MAXSAT prooblem you want to solve.
    The second parameter is the number of samples per iteration.
    The third parameter is the positive learning rate. (0.1 works) 
    The fourth parameter is the negative learning rate. (0.075 works) 
    The fifth parameter is the probability of mutation. (0.02 is good)
    The sixth parameter is the value of a mutation. (0.05 works) 
    The seventh parameter is the number of iterations. (2000 is good)
    The last parameter is 'p' to signal you want the PBIL algorithm.
