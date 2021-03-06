//NOTE MUTATE_WEIGHT_SHIFT and MUTATE_WEIGHT_RANDOM are dependent

//MUTATION
#define MUTATE_LINK 5.0 //percent
#define FIND_LINK_TRIES 5.0 //num
#define MUTATE_GENE 5.0 //percent
#define MUTATE_LINK_TOGGLE 0.5 //percent
#define MUTATE_WEIGHT 80.0 //percent
#define MUTATE_WEIGHT_RANDOM 10.0 //percent
#define WEIGHT_SHIFT_STRENGTH 2.5 //num
#define WEIGHT_RAND_STRENGTH 5.0 //num

//COMPATABILITY
#define EXCESS_COEFF 1.0 //num
#define DISJOINT_COEFF 1.0 //num
#define WEIGHT_COEFF 0.4 //num

//SPECIATION
#define NUM_TARGET_SPECIES 15 //num - number of species wanted
#define INIT_COMPAT_TOLERANCE 3.0 //num - initial tolerance of population
#define DELTA_COMPAT_TOLERANCE 0.1 //num - change of tolerance every generation
#define STALE_THRESHOLD 15 //num - # of generations that a species cannot show improvement before death

//REPRODUCTION
#define FORCE_ENABLE 25.0 //percent - chance a disabled gene will be enabled during crossover
#define CROSSOVER_CHANCE 75.0 //percent - chance that a child will be breeded sexually (reciprocal = asexual chance)
#define SURVIVAL_THRESHOLD 50.0 //percent - percentage of species chosen for reproduction (by top fitness)
