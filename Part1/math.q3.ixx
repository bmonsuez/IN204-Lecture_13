module;

// Ceci est un fragment global permettant
// de charger des fichiers d'entête.

#include<concepts>

// Nous débutons le corps du module.

export module math;


inline double interative_fact(int n)
{
    double value = 1.0;
    for(int i = 2; i < n; i ++)
        value *= i;
    return value;
}

export inline double fact(std::integral auto n)
{
    return n == 0 ? 1 : interative_fact((int)n);
}

