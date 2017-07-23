#include "fuggvenyek.h"
#include "stdafx.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcg_basic.h"
#include <math.h>
#include "random.h"
#include "experiments.h"
#include "hill_climb.h"

#define PI 3.1415926535897
#define e 2.71828182845904523536

/* FUNCTIONS AND DERIVATES */

//function parameterized by x, x is scalar
double fvg(double x)
{
	return x*x*x - x*x - x;
}

//derivate of function parameterized by x, x is scalar
double fvg1(double x)
{
	return 3 * x*x - 2 * x - 1;
}

//2nd derivate of function parameterized by x, x is scalar
double fvg2(double x)
{
	return 6 * x - 2;
}

//function parameterized by x, x is vector
double fv(double *x)
{

	return sin(x[0])*cos(x[1]);
}

//derivate of function parameterized by x, x is vector
double fv1(double *x, int i)
{
	if (i == 0) return cos(x[0])*cos(x[1]);
	if (i == 1) return sin(x[0])*-sin(x[1]);
}

//Hesse matrix of functio parameterized by x, x is vector
double fv2(double *x, int i, int j)
{
	if (i == 0 & j == 0) return -sin(x[0])*cos(x[1]);
	if (i == 0 & j == 1) return cos(x[0])*-sin(x[1]);
	if (i == 1 & j == 0) return cos(x[0])*-sin(x[1]);
	if (i == 1 & j == 1) return sin(x[0])*-cos(x[1]);
}