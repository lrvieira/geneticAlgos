#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "tsp.h"

    void TSP::setPopulationSize(int popSize)
    {
        TSP::m_populationSize = popSize;
    }

    void TSP::setGeneSize(int genSize)
    {
        TSP::m_geneSize = genSize;
    }

    void TSP::setMutationProbability(double mutProb)
    {
        TSP::m_mutationProbability = mutProb;
    }

    std::string TSP::showIndividual(std::vector<int> &indiv)
    {
        std::string individualToShow;
        for (int i = 0; i < indiv.size(); i++)
        {
            //std::cout << indiv.at(i) << " ";
            individualToShow += std::to_string(indiv.at(i)) + " ";
        }
        return individualToShow;
    }

    std::vector<std::vector<int>> TSP::initializePopulation()
    {

        for (int i = 0; i < TSP::m_populationSize; i++)
        {
            TSP::individual.clear();
            TSP::individual.push_back(rand() % TSP::m_geneSize);

            for (int j = 0; j < TSP::m_geneSize - 1; j++)
            {
                int city;

                do
                {
                    city = std::rand() % m_geneSize;
                } while (std::find(individual.begin(), individual.end(), city) != individual.end());
                //std::cout << "saiu" << std::endl;


                TSP::individual.push_back(city);
            }

            TSP::m_population.push_back(individual);
        }

        return TSP::m_population;
    }

    void TSP::showPopulation()
    {
        std::cout << "\n" << "----------------------------\n";
        std::cout << "Initial population: " << "\n\n";

        for (int i = 0; i < TSP::m_populationSize; i++)
        {
            for (int j = 0; j < TSP::m_geneSize; j++)
            {
                std::cout << TSP::m_population[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "\n" << "----------------------------\n";
    }

    double TSP::scoreOfIndividual(std::vector<int> &indiv,  std::vector<std::vector<double>> &citiesPosition)
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

    void TSP::crossover(std::vector<std::vector<int>> &population, int fatherIndex, int motherIndex, std::vector<int> &child1, std::vector<int> &child2)
    {
        int crossoverPoint = std::rand() % m_geneSize;
        std::vector<int> auxChild1, auxChild2;
        
        child1.insert(child1.end(), population[fatherIndex].begin(), population[fatherIndex].begin()+crossoverPoint);
        child1.insert(child1.end(), population[motherIndex].begin()+crossoverPoint, population[motherIndex].end());

        child2.insert(child2.end(), population[motherIndex].begin(), population[motherIndex].begin()+crossoverPoint);
        child2.insert(child2.end(), population[fatherIndex].begin()+crossoverPoint, population[fatherIndex].end());

        auxChild1 = population[motherIndex];
        auxChild2 = population[fatherIndex];
        
        for (int i = 0; i < m_geneSize; i++)
        {
            auxChild1.erase(std::remove(auxChild1.begin(), auxChild1.end(), child1[i]), auxChild1.end());
            auxChild2.erase(std::remove(auxChild2.begin(), auxChild2.end(), child2[i]), auxChild2.end());
        }
        
        auto end = child1.end();
        for (auto it = child1.begin(); it != end; ++it)
            end = std::remove(it + 1, end, *it);

        child1.erase(end, child1.end());

        end = child2.end();
        for (auto it = child2.begin(); it != end; ++it)
            end = std::remove(it + 1, end, *it);

        child2.erase(end, child2.end());

        child1.insert(child1.end(), auxChild1.begin(), auxChild1.end());
        child2.insert(child2.end(), auxChild2.begin(), auxChild2.end());
    }

    void TSP::mutation(std::vector<int> &indiv)
    {
        int gene1 = rand() % m_geneSize;
        int gene2;

        do
        {
            gene2 = rand() % m_geneSize;
        } while (gene1 == gene2);

        std::cout << gene1 << " " << gene2 << std::endl;
        
        int auxGene;

        auxGene = indiv[gene1];

        indiv[gene1] = indiv[gene2];
        indiv[gene2] = auxGene;
    }

    int TSP::getFittest(std::vector<std::vector<int>> &population, std::vector<std::vector<double>> &citiesPosition)
    {
        int indexOfBest = 0;
        int scoreOfBest = scoreOfIndividual(population[indexOfBest], citiesPosition);

        for (int i = 1; i < m_populationSize; i++)
        {
            int score = scoreOfIndividual(population[i], citiesPosition);

            if (score < scoreOfBest)
            {
                indexOfBest = i;
                scoreOfBest = score;
            }
        }
        return indexOfBest;
    }

    std::vector<std::vector<double>> TSP::readPoints(std::string fileName)
    {
        std::vector<std::vector<double>> points;
        std::fstream myFile(fileName);

        if (myFile.is_open())
        {
            double x, y;
            char comma;

            while (myFile >> x >> comma >> y    &&  comma == ',')
            {
                points.push_back({x, y});
            }
            
            myFile.close();
        }
        
        return points;
    }

    double TSP::averageScore(std::vector<std::vector<int>> &population, std::vector<std::vector<double>> &citiesPosition)
    {
        double score=0;

        for (int i = 0; i < m_populationSize; i++)
        {
            score += scoreOfIndividual(population[i], citiesPosition);
        }
        return score/(double)m_populationSize;
    }
