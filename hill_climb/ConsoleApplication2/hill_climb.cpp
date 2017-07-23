

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


//scalar gradient ascent
void gradient()
{
	int stuff = 1;
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);


	double x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 100000000;
	double y = -100;

	while (stuff<500000)
	{
		while ((fvg1(x)> 0.001) || (fvg1(x) < -0.001))
		{
			x = x - fvg1(x) / fvg2(x);
		}

		if (fvg(x)> fvg(y)) y = x;
		stuff++;
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 100000000;
		if (stuff % 2 == 0) x = -x;

	}
	printf("%f", y);

}

//vector gradient ascent
void gradientv()
{
	int stuff = 1;
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);

	double x[2];
	double y[2];
	y[0] = 1000;
	y[1] = 1000;
	for (int i = 0; i < 2; i++)
	{
		x[i] = ((double)pcg32_boundedrand_r(&rng, 1000000000 - 500000000)) / 100000000;
	}

	while (stuff<100000)
	{
		while (sqrt(fv1(x, 0)*fv1(x, 0) + fv1(x, 1)*fv1(x, 1))> 0.01 ||
			sqrt(fv1(x, 0)*fv1(x, 0) + fv1(x, 1)*fv1(x, 1)) < -0.01)
		{
			x[0] = x[0] - (fv1(x, 0)*fv2(x, 1, 1) + fv1(x, 1)*-fv2(x, 0, 1)) / (fv2(x, 0, 0)*fv2(x, 1, 1) - fv2(x, 0, 1)*fv2(x, 1, 0));
			x[1] = x[1] - (fv1(x, 0)*-fv2(x, 1, 0) + fv1(x, 1)*fv2(x, 0, 0)) / (fv2(x, 0, 0)*fv2(x, 1, 1) - fv2(x, 0, 1)*fv2(x, 1, 0));
			//printf("%f  |  %f\n", x[0], x[1]);
		}

		if (fv(x) > fv(y)) { y[0] = x[0]; y[1] = x[1]; }
		stuff++;
		x[0] = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 100000000;
		x[1] = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 100000000;
		if (stuff % 4 == 0) x[0] = -x[0];
		if (stuff % 4 == 1) x[1] = -x[1];
		if (stuff % 4 == 2) { x[0] = -x[0]; x[1] = -x[1]; }

	}
	printf("%f | %f\n", y[0], y[1]);
	//printf("%f", sqrt(fv1(y, 0)*fv1(y, 0) + fv1(y, 1)*fv1(y, 1)));

}


//hill climbing
void hillclimb()
{
	int n = 10;
	int stuff = 1;
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);

	double x;
	double s = 0;
	double r;
	double w;
	double best = s;
	double neg = 1;
	double neg2 = 1;

	double sugar = 1;

	while (stuff<50000)
	{
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar);
		if (stuff % 2 == 0) x = -x;

		r = s + x;

		for (int i = 0; i < n; i++)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar);
			if (i % 2 == 0) x = -x;

			w = s + x;

			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;
		}
		s = r;

		if (fvg1(s) < 0) neg = -1;
		else neg = 1;
		if (fvg1(best) < 0) neg2 = -1;
		else neg2 = 1;
		if (neg*fvg1(s)< neg2*fvg1(best)) best = s;

		stuff++;
	}
	printf("%f", best);

}

//hill climbing with gaussian random number
void gaussian_climb()
{
	int n = 100;		//number of children
	int stuff = 1;		//counter for while cycle
	pcg32_random_t rng;	//random number generator
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);

	double x, y;		//this will be random numbers
	double s = 0;		//original candidate
	double r;			//child number
	double w;			//child number variations
	double best = s;	//best number
	double neg = 1;		//negate first value in if statement
	double neg2 = 1;	//negate 2nd value in if statement

	double nu = 0;		//gaussian nu
	double sigma = 1;	//gaussian sigma
	double distro;		//save into this the number from gaussian distribution
	double z = 0;		//for the distro
	int k = 1;			//inner counter

	double sugar = 1;	//radius of rng around 0

	while (fvg1(best)>0.0001 || fvg1(best)<-0.0001)
	{
		//rng
		z = 0;
		while (z <= 0 || z >= 1)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;

			z = x*x + y*y;
			k++;
		}

		//gaussian random number
		distro = nu + x*sigma*sqrt(-2 * log(z) / z);

		r = s + distro;

		for (int i = 0; i < n; i++)
		{
			//rng
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;
				k++;
			}


			//gaussian random number
			distro = nu + x*sigma*sqrt(-2 * log(z) / z);

			w = s + distro;

			//derivate as closes as possible to 0
			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;
		}
		s = r;

		//derivate as closes as possible to 0
		if (fvg1(s) < 0) neg = -1;
		else neg = 1;
		if (fvg1(best) < 0) neg2 = -1;
		else neg2 = 1;
		if (neg*fvg1(s)< neg2*fvg1(best)) best = s;

		stuff++;
	}
	printf("%f    %i", best, stuff);

}

//simulated annealing
void simulated_annealing()
{
	double n = 503;		//number of children
	int stuff = 1;		//counter for while cycle
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);	//random number generator

	double x, y;		//this will be random numbers
	double s = 0;		//original candidate
	double r;			//child number
	double w;			//child number variations
	double best = s;	//best number
	double neg = 1;		//negate first value in if statement
	double neg2 = 1;	//negate 2nd value in if statement
	int t = 100;		//"temperature"

	double nu = -0.4;		//gaussian nu
	double sigma = 0.28;	//gaussian sigma
	double distro;		//save into this the number from gaussian distribution
	double z;			//for the distro
	int k = 1;			//inner counter

	double sugar = 1;	//radius of rng around 0

	while ((fvg1(best)>0.0001 || fvg1(best)<-0.0001) && t>0)
	{
		//rng
		z = 0;
		while (z <= 0 || z >= 1)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			z = x*x + y*y;
			k++;
		}

		//gaussian random number
		distro = nu + x*sigma*sqrt(-2 * log(z) / z);


		r = s + distro;

		for (int i = 0; i < (int)n; i++)
		{
			//rng
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;
				k++;
			}

			//gaussian random number
			distro = nu + x*sigma*sqrt(-2 * log(z) / z);

			w = s + distro;

			//derivate as closes as possible to 0
			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;
			//stuff++;
		}
		k++;
		//random number between 0 and 1
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 1000000000;

		//derivate as closes as possible to 0
		if (fvg1(r) < 0) neg = -1;
		else neg = 1;
		if (fvg1(s) < 0) neg2 = -1;
		else neg2 = 1;

		if (neg*fvg1(r) < neg2*fvg1(s) || x<pow(e, ((1 / (neg*fvg1(r)) - 1 / (neg2*fvg1(s)))) / t)) s = r;

		//derivate as closes as possible to 0
		if (fvg1(s) < 0) neg = -1;
		else neg = 1;
		if (fvg1(best) < 0) neg2 = -1;
		else neg2 = 1;
		if (neg*fvg1(s)< neg2*fvg1(best)) best = s;


		t--;
		stuff++;
	}
	printf("%f    %i", best, stuff);

}

//simulated annealing function to be optimized
double simulated_annealing_proto(double n, double nu, double sigma, pcg32_random_t rng)
{
	int stuff = 1;		//counter for while cycle


	double x, y;		//this will be random numbers
	double s = 0;		//original candidate
	double r;			//child number
	double w;			//child number variations
	double best = s;	//best number
	double neg = 1;		//negate first value in if statement
	double neg2 = 1;	//negate 2nd value in if statement
	int t = 100;		//"temperature"



	double distro;		//to save the gaussian number
	double z;			//needed for distro

	double sugar = 1;	//a random szám generálás sugara 0 körül

	while ((fvg1(best)>0.001 || fvg1(best)<-0.001) && t>0)
	{
		//rng
		z = 0;
		while (z <= 0 || z >= 1)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			z = x*x + y*y;

		}

		//gaussian random number
		distro = nu + x*sigma*sqrt(-2 * log(z) / z);

		r = s + distro;

		for (int i = 0; i <(int)n; i++)
		{
			//rng
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;

			}

			//gaussian random number
			distro = nu + x*sigma*sqrt(-2 * log(z) / z);
			//printf("%f\n", distro);

			w = s + distro;

			//derivate as closes as possible to 0
			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;

			//stuff++;
		}

		//random number between 0 and 1
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 1000000000;

		//derivate as closes as possible to 0
		if (fvg1(r) < 0) neg = -1;
		else neg = 1;
		if (fvg1(s) < 0) neg2 = -1;
		else neg2 = 1;

		if (neg*fvg1(r) < neg2*fvg1(s) || x<pow(e, ((1 / (neg*fvg1(r)) - 1 / (neg2*fvg1(s)))) / t)) s = r;

		//derivate as closes as possible to 0
		if (fvg1(s) < 0) neg = -1;
		else neg = 1;
		if (fvg1(best) < 0) neg2 = -1;
		else neg2 = 1;
		if (neg*fvg1(s)< neg2*fvg1(best)) best = s;

		stuff++;
		t--;
	}
	return stuff;

}

//simulated annealing optimization function
void simulated_annealing_optimization()
{
	int n = 10;									//number of children
	int stuff = 1;								//counter for while cycle
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);	//random number generator
	int i;										//for for cycles

	double x, y;								//these will be random numbers
	double s[3] = { 500,-0.3,0.4 };				//original candidate
	double r[3];								//child number
	double w[3];								//child number variations
	double best[3] = { 500,-0.3,0.4 };			//best number

	int t = 10000;								//"temperature"

	double nu = 0;								//gaussian nu
	double sigma[3] = { 5,0.01,0.01 };			//gaussian sigma

	double distro;								//to save gaussian number
	double z;									//needed for distra

	/* numbers to save best candidates */
	double besto1 = 0;
	double besto2 = 0;
	double besto3 = 0;
	double besto4 = 0;
	double besto5 = 0;
	double besto6 = 0;

	double bestoszam = 0;						//best candidate value


	double sugar = 1;							//rng radius around 0

	while (t>0)
	{
		for (i = 0; i < 3; )
		{
			//rng
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;

			}

			//gaussian random number
			distro = nu + x*sigma[i] * sqrt(-2 * log(z) / z);

			r[i] = s[i] + distro;
			if (r[0] <= 1);
			else i++;
		}


		for (int j = 1; j <= n; j++)
		{
			for (i = 0; i < 3;)
			{
				//rng
				z = 0;
				while (z <= 0 || z >= 1)
				{
					x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
					y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
					z = x*x + y*y;

				}

				//gaussian random number
				distro = nu + x*sigma[i] * sqrt(-2 * log(z) / z);

				w[i] = s[i] + distro;
				if (w[0] <= 1);
				else i++;
			}

			//compare
			besto1 = 0;
			for (int o = 0; o < 5; o++)
			{
				besto1 = besto1 + simulated_annealing_proto(w[0], w[1], w[2], rng);
			}
			besto2 = besto2 + besto1 / 5;

			if (besto2 / (j + (stuff - 1)*n) < (besto2 - besto1 / 5) / (j + (stuff - 1)*n - 1))
			{
				r[0] = w[0];
				r[1] = w[1];
				r[2] = w[2];
			}

		}

		//random number between 0 and 1
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 1000000000;

		//compare
		besto3 = 0;
		for (int o = 0; o < 5; o++)
		{
			besto3 = besto3 + simulated_annealing_proto(r[0], r[1], r[2], rng);
		}
		besto6 = besto6 + besto3 / 5;

		if (besto6 / stuff < (besto6 - (besto3 / 5)) / (stuff - 1) || x < pow(e, ((1 / (besto6 / stuff) - 1 / (besto6 - (besto3 / 5)) / (stuff - 1))) / t))
		{
			s[0] = r[0];
			s[1] = r[1];
			s[2] = r[2];
		}

		//compare
		besto5 = 0;
		for (int o = 0; o < 5; o++)
		{
			besto5 = besto5 + simulated_annealing_proto(s[0], s[1], s[2], rng);
		}
		bestoszam = bestoszam + besto5 / 5;

		if (bestoszam / stuff<(bestoszam - (besto5 / 5)) / (stuff - 1))
		{
			best[0] = s[0];
			best[1] = s[1];
			best[2] = s[2];
		}

		printf("bestoszam=%f   %f   %f   %f\n", bestoszam / stuff, best[0], best[1], best[2]);
		stuff++;
		t--;

	}
	printf("n=%f\n", best[0]);
	printf("nu=%f\n", best[1]);
	printf("sigma=%f\n", best[2]);
	printf("t=%i\n", t);

}