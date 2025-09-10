#include <algorithm>
#include <thread>
#include "parameters.h"

const int SEED = 42;
const int POPULATION_SIZE = 50;
const int TOURNAMENT_SIZE = 5;
const float MUTATION_RATE = 0.1;
const float LOCAL_OPT_RATE = 0.3;
const int ELITE_SIZE = 2;
const int MAX_GENERATIONS = 50;
const int MAX_THREADS = std::max(2u, std::thread::hardware_concurrency());

