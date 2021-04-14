#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>

class TSP{

public:
    void setPopulationSize(int popSize)
    {
        m_populationSize = popSize;
    }

    void setGeneSize(int genSize)
    {
        m_geneSize = genSize;
    }

    void setNumberOfGenerations(int numGen)
    {
        m_numberOfGenerations = numGen;
    }

    void setNumberOfTournments(int numTourn)
    {
        m_numberOfTournments = numTourn;
    }

    void setCrossoverProbability(double crossProb)
    {
        m_crossoverProbability = crossProb;
    }

    void setMutationProbability(double mutProb)
    {
        m_mutationProbability = mutProb;
    }


private:
    int     m_geneSize;
    int     m_populationSize;
    int     m_numberOfTournments;
    int     m_numberOfGenerations;
    double  m_mutationProbability;
    double  m_crossoverProbability;

    std::vector<std::vector<int>>   population;
    std::vector<int>                individual;

public:
    std::vector<std::vector<int>> initializePopulation()
    {

        for (int i = 0; i < m_populationSize; i++)
        {
            individual.clear();
            individual.push_back(rand() % m_geneSize);

            for (int j = 0; j < m_geneSize - 1; j++)
            {
                int city;

                do
                {
                    city = std::rand() % m_geneSize;
                } while (std::find(individual.begin(), individual.end(), city) != individual.end());
                //std::cout << "saiu" << std::endl;


                individual.push_back(city);
            }

            population.push_back(individual);
        }

        return population;
    }

    void showPopulation()
    {
        std::cout << "\n" << "----------------------------\n";
        std::cout << "Initial population: " << "\n\n";

        for (int i = 0; i < m_populationSize; i++)
        {
            for (int j = 0; j < m_geneSize; j++)
            {
                std::cout << population[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "\n" << "----------------------------\n";
    }

    double scoreOfIndividual(std::vector<int> &indiv,  std::vector<std::vector<double>> &citiesPosition)
    {
        double distance = 0.;
        double x1, y1, x2, y2;

        for (int i = 0; i < indiv.size() - 1; i++)
        {
            x1 = citiesPosition[indiv[i]][0];
            y1 = citiesPosition[indiv[i]][1];
            x2 = citiesPosition[indiv[i+1]][0];
            y2 = citiesPosition[indiv[i+1]][1];

            distance += std::sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        }
        
            x1 = citiesPosition[indiv[indiv.size() - 1]][0];
            y1 = citiesPosition[indiv[indiv.size() - 1]][1];
            x2 = citiesPosition[indiv[0]][0];
            y2 = citiesPosition[indiv[0]][1];

        distance += std::sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

        return distance;
    }

    void crossoverTournment(std::vector<std::vector<int>> &population, int fatherIndex, int motherIndex, std::vector<int> &child1, std::vector<int> &child2)
    {
        int point1 = std::rand() % m_geneSize;
        int point2;
        do
        {
            point2 = std::rand() % m_geneSize;
        } while (point1 == point2);

        int p1 = std::min(point1, point2);
        int p2 = std::max(point1, point2);
        std::cout << p1 << " " << p2 << std::endl;
                
        for (int i = 0; i < p1; i++)
        {
            child1.push_back(population[fatherIndex][i]);
            child2.push_back(population[motherIndex][i]);
        }

        for (int i = p1; i < p2; i++)
        {
            auto it = std::find(population[motherIndex].begin(), population[motherIndex].end(), population[fatherIndex][i]);
            
            if (std::distance(population[motherIndex].begin(), it) >= p1 && std::distance(population[motherIndex].begin(), it) < p2)
            {
                //std::cout << "encontrou no indice " << std::distance(population[motherIndex].begin(), it) << std::endl;
                child1.push_back(population[fatherIndex][i]);
                child2.push_back(population[motherIndex][i]);
            }
            else
            {
                child1.push_back(population[motherIndex][i]);
                child2.push_back(population[fatherIndex][i]);
            }
            
        }
        
        for (int i = p2; i < m_geneSize; i++)
        {
            child1.push_back(population[fatherIndex][i]);
            child2.push_back(population[motherIndex][i]);
        }
    }

};

int main(){

    srand(time(NULL));                              //Allow random numbers

    std::vector<std::vector<double>> cities = 
    {
        {15.5 ,  5.7},
        { 3.8 , 10.9},
        { 7.2 ,  3.4},
        {13.1 , 12.4},
        { 5.1 , 11.9},
        { 9.9 ,  4.8},
        { 2.7 ,  9.4}
    };

    TSP trip;
    trip.setPopulationSize(10);
    trip.setGeneSize(cities.size());
    trip.setNumberOfGenerations(100);
    trip.setNumberOfTournments(20);
    trip.setCrossoverProbability(0.7);
    trip.setMutationProbability(0.1);


    std::vector<std::vector<int>> routes;
    routes = trip.initializePopulation();

    trip.showPopulation();

    std::cout << trip.scoreOfIndividual(routes[1], cities) << std::endl;

    std::vector<int> firstChild, secondChild;
    trip.crossoverTournment(routes, 0, 1, firstChild, secondChild);

    for (int i = 0; i < cities.size(); i++)
    {
        std::cout << firstChild[i] << " ";
    }
    std::cout << std::endl;
    

}