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

#define PI 3.1415926535897
#define e 2.71828182845904523536



//simple bubble sort
void sort()
{
	int k;
	int v[5] = { 1,5,3,4,2 };
	bool swapped = true;
	while (swapped == true)
	{
		swapped = false;
		for (int i = 0; i < 4; i++)
		{
			if (v[i]>v[i + 1])
			{
				k = v[i];
				v[i] = v[i + 1];
				v[i + 1] = k;
				swapped = true;
			}
		}
	}

	for (int j = 0; j < 5; j++)
	{
		printf("%i", v[j]);
	}
}

//prototype for wolfram alpha style function declaring
void cucc()
{
	char* f = "x*x*x-x*x-x";
	double x = 2;
	double g = 0;
	double h = x;

	for (int i = 0; i < 17; i++)
	{
		if (f[i] == '*') h = h*x;
		if (f[i] == '-')
		{
			if (g == 0)  g = h;
			else g = g - h;
			h = x;
		}


	}
	g = g - h;
	printf("   %f", g);
}

//rng tester
void rng_test()
{
	double x;
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
	for (int i = 0; i < 10; i++)
	{

		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 500000000;

		printf("%f\n", x);
	}
}
