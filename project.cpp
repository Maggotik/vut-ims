#include <stdio.h>
#include <string.h>
#include <fstream>
#include <tgmath.h>

using namespace std;

class Population
{
public:
    float population_size;
    float susceptible;
    float infected;
    float recovered;
    float u_population;
    float v_population;
    float u_susceptible;
    float v_susceptible;

    Population(float unvacc_pop, float vacc_pop, float inf)
    {
        population_size = unvacc_pop + vacc_pop;
        infected = inf;
        recovered = 0;
        u_susceptible = (population_size - inf) * unvacc_pop;
        v_susceptible = (population_size - inf) * vacc_pop;
    }

    void iterate(float a, float b, float c)
    {
        float d_susceptible_u = (a * u_susceptible * infected); // pocet nakazenych za den nezaockovanych
        float d_susceptible_v = (c * v_susceptible * infected); // pocet nakazenych za den zaockovanych
        float d_susceptible = d_susceptible_v + d_susceptible_u; // pocet nakazenych za den spolu
        float d_recovered = infected / b; // pocet uzdravenych za den 

        //infikovani
        infected += d_susceptible - d_recovered;

        // zdravi
        u_susceptible -= d_susceptible_u;
        v_susceptible -= d_susceptible_v;

        recovered += d_recovered;
    }
};

int main(int argc, char **argv)
{
    float u_population;
    float infected_init;
    float v_population;
    if (strcmp(argv[1], "-d") == 0)
    {
        u_population = 1;
        infected_init = 0.00000046729;
        v_population = 0;
    }
    else if (strcmp(argv[1], "-v50") == 0)
    {
        u_population = 0.5;
        infected_init = 0.00000046729;
        v_population = 0.5;
    }
    else if (strcmp(argv[1], "-v80") == 0)
    {
        u_population = 0.2;
        infected_init = 0.00000046729;
        v_population = 0.8;
    }
    else if (strcmp(argv[1], "-v100") == 0)
    {
        u_population = 0;
        infected_init = 0.00000046729;
        v_population = 1;
    }
    int days = 31;
    
    float b = 54;   //removal rate
    float a = 0.267; //transmission rate
    float c = a * 0.813; //transmission rate of vaccined ppl

    float susceptible = u_population + v_population - infected_init;
    float infected;
    float recovered = 0;

    Population population(u_population, v_population, infected_init);

    ofstream output;
    output.open("output.csv");
    output << "Day, Susceptible_u, Susceptible_v, Infected, Recovered" << endl;
    output << "1" << ",         " << ceil(u_population * 10700000) << ",        " << ceil(v_population * 10700000) << ",        " << ceil(infected * 10700000) << ",        " << ceil(recovered * 10700000) << endl;

    for (int i = 2; i <= days; i++)
    {
        population.iterate(a, b, c);
        output << i << ",       " << ceil(population.u_susceptible * 10700000) << ",        " << ceil(population.v_susceptible * 10700000) << ",        " << ceil(population.infected * 10700000) << ",         " << ceil(population.recovered * 10700000) << endl;
    }
    output.close();
    return 0;
}