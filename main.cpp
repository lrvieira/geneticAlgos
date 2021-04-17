#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "tsp.h"


int main(){

    srand(time(NULL));                              //Allow random numbers

    std::vector<std::vector<double>> cities = 
    {
        {  8.660 ,  5.000},
        {  5.000 ,  8.660},
        {  0.000 , 10.000},
        { -5.000 ,  8.660},
        { -8.660 ,  5.000},
        {-10.000 ,  0.000},
        { -8.660 , -5.000},
        { -5.000 , -8.660},
        {  0.000 ,-10.000},
        {  5.000 , -8.660},
        {  8.660 , -5.000},
        { 10.000 ,  0.000}
    };

    double aim = 63;

    int     numberOfGenerations = 150;
    int     numberOfTournments = 400;
    int     populationSize = 1000;
    int     geneSize = cities.size();
    double  crossoverProbability = 0.7;
    double  mutationProbability = 0.1;

    TSP trip;
    trip.setPopulationSize(populationSize);
    trip.setGeneSize(geneSize);
    trip.setMutationProbability(mutationProbability);

    std::vector<std::vector<int>> routes;
    routes = trip.initializePopulation();

   for (int i = 0; i < numberOfGenerations; i++)
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

                trip.crossoverTournment(routes, fatherIndex, motherIndex, firstChild, secondChild);

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

        std::cout << "Generation: " << i+1;
        std::cout << " | Best: ";

        int indexOfBest = trip.getFittest(routes, cities);
        double scoreOfBest = trip.scoreOfIndividual(routes[indexOfBest], cities);
        trip.showIndividual(routes[indexOfBest]);

        std::cout << " | Score: " << scoreOfBest << "\n\n";

        if (scoreOfBest <= aim)
        {
            break;
        }
   }
}