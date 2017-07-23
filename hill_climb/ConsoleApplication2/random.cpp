#include "stdafx.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcg_basic.h"
#include <math.h>
#include "random.h"
#include "functions.h"
#include "experiments.h"
#include "hill_climb.h"


static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

/* RANDOM START */
void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq)
{
	rng->state = 0U;
	rng->inc = (initseq << 1u) | 1u;
	pcg32_random_r(rng);
	rng->state += initstate;
	pcg32_random_r(rng);
}
void pcg32_srandom(uint64_t seed, uint64_t seq)
{
	pcg32_srandom_r(&pcg32_global, seed, seq);
}
// pcg32_random()
// pcg32_random_r(rng)
//     Generate a uniformly distributed 32-bit random number
uint32_t pcg32_random_r(pcg32_random_t* rng)
{
	uint64_t oldstate = rng->state;
	rng->state = oldstate * 6364136223846793005ULL + rng->inc;
	uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
	uint32_t rot = oldstate >> 59u;
	return (xorshifted >> rot) | (xorshifted << ((0x100000000ull - rot) & (30 - 1)));
}
uint32_t pcg32_random()
{
	return pcg32_random_r(&pcg32_global);
}
// pcg32_boundedrand(bound):
// pcg32_boundedrand_r(rng, bound):
//     Generate a uniformly distributed number, r, where 0 <= r < bound
uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound)
{
	// To avoid bias, we need to make the range of the RNG a multiple of
	// bound, which we do by dropping output less than a threshold.
	// A naive scheme to calculate the threshold would be to do
	//
	//     uint32_t threshold = 0x100000000ull % bound;
	//
	// but 64-bit div/mod is slower than 32-bit div/mod (especially on
	// 32-bit platforms).  In essence, we do
	//
	//     uint32_t threshold = (0x100000000ull-bound) % bound;
	//
	// because this version will calculate the same modulus, but the LHS
	// value is less than 2^32.

	uint32_t threshold = (0x100000000ull - bound) % bound;

	// Uniformity guarantees that this loop will terminate.  In practice, it
	// should usually terminate quickly; on average (assuming all bounds are
	// equally likely), 82.25% of the time, we can expect it to require just
	// one iteration.  In the worst case, someone passes a bound of 2^31 + 1
	// (i.e., 2147483649), which invalidates almost 50% of the range.  In 
	// practice, bounds are typically small and only a tiny amount of the range
	// is eliminated.
	for (;;) {
		uint32_t r = pcg32_random_r(rng);
		if (r >= threshold)
			return r % bound;
	}
}
uint32_t pcg32_boundedrand(uint32_t bound)
{
	return pcg32_boundedrand_r(&pcg32_global, bound);
}
/* RANDOM FINISH */