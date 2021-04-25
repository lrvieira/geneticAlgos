#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <chrono>
#include "tsp.h"

int main(int argc, char *argv[]){

    auto start = std::chrono::steady_clock::now();

    srand(time(NULL));                              //Allow random numbers

    TSP trip;
    //double aim = 63;    

    std::vector<std::vector<double>> cities = trip.readPoints(argv[1]);

    int     numberOfGenerations =       std::atoi(argv[2]);     //150
    int     populationSize =            std::atoi(argv[3]);     //100
    int     numberOfTournments =        std::atoi(argv[4]);     //400
    double  crossoverProbability =      std::atof(argv[5]);     //0.5
    double  mutationProbability =       std::atof(argv[6]);     //0.1
    int     geneSize =                  cities.size();
    float   aim =                       std::atof(argv[7]);     //63

    trip.setPopulationSize(populationSize);
    trip.setGeneSize(geneSize);
    trip.setMutationProbability(mutationProbability);

    std::vector<std::vector<int>> routes;
    routes = trip.initializePopulation();

    std::ofstream outFile;
    std::string outFileName = argv[1];
    outFileName = outFileName.substr(0, outFileName.size() - 4) + ".out.csv";
    outFile.open(outFileName);

    outFile << "Generation, Best, Score, Average\n";

    int i;
    double      scoreOfBest;
    double      averageScore;
    std::string bestIndividual;

   for (i = 0; i < numberOfGenerations; i++)
   {
       for (int j = 0; j < numberOfTournments; j++)
       {
           double probability = ((double) std::rand()/ ((double)RAND_MAX + 1));

            if (probability > crossoverProbability)
            {
                int fatherIndex = std::rand() % populationSize;
                int motherIndex;

                do
                {
                    motherIndex = std::rand() % populationSize;
                } while (fatherIndex == motherIndex);
                
                std::vector<int> firstChild, secondChild;

                trip.crossover(routes, fatherIndex, motherIndex, firstChild, secondChild);

                double scoreFather =       trip.scoreOfIndividual(routes[fatherIndex], cities);
                double scoreMother =       trip.scoreOfIndividual(routes[motherIndex], cities);
                double scoreFirstChild =   trip.scoreOfIndividual(firstChild, cities);
                double scoreSecondChild =  trip.scoreOfIndividual(secondChild, cities);

                std::vector<int> fit1, fit2, nonFit1, nonFit2;
                
                if (scoreFirstChild + scoreSecondChild < scoreFather + scoreMother)
                {
                    for (int k = 0; k < geneSize; k++)
                    {
                        routes[fatherIndex][k] = firstChild[k];
                        routes[motherIndex][k] = secondChild[k];
                    }
                }
            }
       }

        int indexOfBest = trip.getFittest(routes, cities);

        scoreOfBest = trip.scoreOfIndividual(routes[indexOfBest], cities);
        averageScore = trip.averageScore(routes, cities);
        bestIndividual = trip.showIndividual(routes[indexOfBest]);

        outFile << i+1 << ", " << bestIndividual << ", " << scoreOfBest << ", " << averageScore << "\n";

        if (scoreOfBest <= aim)
        {
            break;
        }
   }

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;

    std::cout << "\nEnd of program. Execution time: " << std::chrono::duration <double, std::milli> (diff).count() << " ms. Solution below\n\n";
    std::cout << "Gen: " << i << " | Best: " << bestIndividual << " | Score: " << scoreOfBest << " | Average: " << averageScore << "\n\n";

    outFile.close();
}