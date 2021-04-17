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


    void setMutationProbability(double mutProb)
    {
        m_mutationProbability = mutProb;
    }

    void showIndividual(std::vector<int> &indiv)
    {
        for (int i = 0; i < indiv.size(); i++)
        {
            std::cout << indiv.at(i) << " ";
        }

        //std::cout << std::endl;
    }

private:
    int     m_geneSize;
    int     m_populationSize;
    double  m_mutationProbability;

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

    void mutation(std::vector<int> &indiv)
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

    int getFittest(std::vector<std::vector<int>> &population, std::vector<std::vector<double>> &citiesPosition)
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

};

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

    int     numberOfGenerations = 200;
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