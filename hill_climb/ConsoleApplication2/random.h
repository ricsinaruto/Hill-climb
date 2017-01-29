void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq);
void pcg32_srandom(uint64_t seed, uint64_t seq);
uint32_t pcg32_random_r(pcg32_random_t* rng); 
uint32_t pcg32_random();
uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound);
uint32_t pcg32_boundedrand(uint32_t bound);