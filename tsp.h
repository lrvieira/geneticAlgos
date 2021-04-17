#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>

class TSP{

public:
    void setPopulationSize(int popSize);

    void setGeneSize(int genSize);

    void setMutationProbability(double mutProb);

    void showIndividual(std::vector<int> &indiv);

private:
    int     m_geneSize;
    int     m_populationSize;
    double  m_mutationProbability;

    std::vector<std::vector<int>>   population;
    std::vector<int>                individual;

public:
    std::vector<std::vector<int>> initializePopulation();

    void showPopulation();

    double scoreOfIndividual(std::vector<int> &indiv,  std::vector<std::vector<double>> &citiesPosition);

    void crossoverTournment(std::vector<std::vector<int>> &population, int fatherIndex, int motherIndex, std::vector<int> &child1, std::vector<int> &child2);

    void mutation(std::vector<int> &indiv);

    int getFittest(std::vector<std::vector<int>> &population, std::vector<std::vector<double>> &citiesPosition);
};