

#include "stdafx.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcg_basic.h"
#include <math.h>
#include "random.h"
#include "fuggvenyek.h"
#include "probak.h"
#include "hill_climb.h"

#define PI 3.1415926535897
#define e 2.71828182845904523536


//skalár gradient ascent
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

//vektor gradient ascent
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
	int stuff = 1;		//a while számlálója
	pcg32_random_t rng;	//random number generator
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);

	double x, y;			//ez lesz egy random szám
	double s = 0;		//original candidate
	double r;			//child number
	double w;			//child number variations
	double best = s;	//best number
	double neg = 1;		//elsõ érték negálására az if-ben
	double neg2 = 1;	//második érték negálására az if-ben

	double nu = 0;		//gaussian nu-je
	double sigma = 1;	//gaussian sigmája
	double distro;		//amibe elmentjük a gaussian által létrehozott számot
	double z = 0;			//a distrohoz kell
	int k = 1;			//belsõ számláló

	double sugar = 1;	//a random szám generálás sugara 0 körül

	while (fvg1(best)>0.0001 || fvg1(best)<-0.0001)
	{
		//random szám generálás
		z = 0;
		while (z <= 0 || z >= 1)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;

			z = x*x + y*y;
			k++;
		}

		//gaussian random szám
		distro = nu + x*sigma*sqrt(-2 * log(z) / z);

		r = s + distro;

		for (int i = 0; i < n; i++)
		{
			//random szám generálás
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;
				k++;
			}


			//gaussian random szám
			distro = nu + x*sigma*sqrt(-2 * log(z) / z);

			w = s + distro;

			//a derivált minél közelebb legyen 0-hoz
			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;
		}
		s = r;

		//a derivált minél közelebb legyen 0-hoz
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
	int stuff = 1;		//a while számlálója
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);	//random number generator

	double x, y;			//ez lesz egy random szám
	double s = 0;		//original candidate
	double r;			//child number
	double w;			//child number variations
	double best = s;	//best number
	double neg = 1;		//elsõ érték negálására az if-ben
	double neg2 = 1;	//második érték negálására az if-ben
	int t = 100;	//"temperature"

	double nu = -0.4;		//gaussian nu-je
	double sigma = 0.28;	//gaussian sigmája
	double distro;		//amibe elmentjük a gaussian által létrehozott számot
	double z;			//a distrohoz kell
	int k = 1;			//belsõ számláló

	double sugar = 1;	//a random szám generálás sugara 0 körül

	while ((fvg1(best)>0.0001 || fvg1(best)<-0.0001) && t>0)
	{
		//random szám generálás
		z = 0;
		while (z <= 0 || z >= 1)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			z = x*x + y*y;
			k++;
		}

		//gaussian random szám
		distro = nu + x*sigma*sqrt(-2 * log(z) / z);


		r = s + distro;

		for (int i = 0; i < (int)n; i++)
		{
			//random szám generálás
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;
				k++;
			}

			//gaussian random szám
			distro = nu + x*sigma*sqrt(-2 * log(z) / z);

			w = s + distro;

			//a derivált minél közelebb legyen 0-hoz
			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;
			//stuff++;
		}
		k++;
		//random szám 0 és 1 közt
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 1000000000;

		//a derivált minél közelebb legyen 0-hoz
		if (fvg1(r) < 0) neg = -1;
		else neg = 1;
		if (fvg1(s) < 0) neg2 = -1;
		else neg2 = 1;

		if (neg*fvg1(r) < neg2*fvg1(s) || x<pow(e, ((1 / (neg*fvg1(r)) - 1 / (neg2*fvg1(s)))) / t)) s = r;

		//a derivált minél közelebb legyen 0-hoz
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

//optimalizálandó simulated annealing
double simulated_annealing_proto(double n, double nu, double sigma, pcg32_random_t rng)
{
	int stuff = 1;		//a while számlálója


	double x, y;			//ez lesz egy random szám
	double s = 0;		//original candidate
	double r;			//child number
	double w;			//child number variations
	double best = s;	//best number
	double neg = 1;		//elsõ érték negálására az if-ben
	double neg2 = 1;	//második érték negálására az if-ben
	int t = 100;	//"temperature"



	double distro;		//amibe elmentjük a gaussian által létrehozott számot
	double z;			//a distrohoz kell

	double sugar = 1;	//a random szám generálás sugara 0 körül

	while ((fvg1(best)>0.001 || fvg1(best)<-0.001) && t>0)
	{
		//random szám generálás
		z = 0;
		while (z <= 0 || z >= 1)
		{
			x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
			z = x*x + y*y;

		}

		//gaussian random szám
		distro = nu + x*sigma*sqrt(-2 * log(z) / z);

		r = s + distro;

		for (int i = 0; i <(int)n; i++)
		{
			//random szám generálás
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;

			}

			//gaussian random szám
			distro = nu + x*sigma*sqrt(-2 * log(z) / z);
			//printf("%f\n", distro);

			w = s + distro;

			//a derivált minél közelebb legyen 0-hoz
			if (fvg1(w) < 0) neg = -1;
			else neg = 1;
			if (fvg1(r) < 0) neg2 = -1;
			else neg2 = 1;
			if (neg*fvg1(w) < neg2*fvg1(r)) r = w;

			//stuff++;
		}

		//random szám 0 és 1 közt
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 1000000000;

		//a derivált minél közelebb legyen 0-hoz
		if (fvg1(r) < 0) neg = -1;
		else neg = 1;
		if (fvg1(s) < 0) neg2 = -1;
		else neg2 = 1;

		if (neg*fvg1(r) < neg2*fvg1(s) || x<pow(e, ((1 / (neg*fvg1(r)) - 1 / (neg2*fvg1(s)))) / t)) s = r;

		//a derivált minél közelebb legyen 0-hoz
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

//simulated annealing optimalizáló
void simulated_annealing_optimization()
{
	int n = 10;								//number of children
	int stuff = 1;								//a while számlálója
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rng);	//random number generator
	int i;										//for ciklusokhoz

	double x, y;								//ez lesz egy random szám
	double s[3] = { 500,-0.3,0.4 };				//original candidate
	double r[3];								//child number
	double w[3];								//child number variations
	double best[3] = { 500,-0.3,0.4 };			//best number

	int t = 10000;								//"temperature"

	double nu = 0;								//gaussian nu-je
	double sigma[3] = { 5,0.01,0.01 };		//gaussian sigmája

	double distro;								//amibe elmentjük a gaussian által létrehozott számot
	double z;									//a distrohoz kell
	double besto1 = 0;
	double besto2 = 0;
	double besto3 = 0;
	double besto4 = 0;
	double besto5 = 0;
	double besto6 = 0;

	double bestoszam = 0;


	double sugar = 1;							//a random szám generálás sugara 0 körül

	while (t>0)
	{
		for (i = 0; i < 3; )
		{
			//random szám generálás
			z = 0;
			while (z <= 0 || z >= 1)
			{
				x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
				z = x*x + y*y;

			}

			//gaussian random szám
			distro = nu + x*sigma[i] * sqrt(-2 * log(z) / z);

			r[i] = s[i] + distro;
			if (r[0] <= 1);
			else i++;
		}


		for (int j = 1; j <= n; j++)
		{
			for (i = 0; i < 3;)
			{
				//random szám generálás
				z = 0;
				while (z <= 0 || z >= 1)
				{
					x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
					y = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / (500000000 / sugar) - sugar;
					z = x*x + y*y;

				}

				//gaussian random szám
				distro = nu + x*sigma[i] * sqrt(-2 * log(z) / z);

				w[i] = s[i] + distro;
				if (w[0] <= 1);
				else i++;
			}

			//összehasonlítás
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

		//random szám 0 és 1 közt
		x = ((double)pcg32_boundedrand_r(&rng, 1000000000)) / 1000000000;

		//összehasonlítás
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

		//összehasonlítás
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