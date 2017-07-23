
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


int main(int argc, char** argv)
{

	simulated_annealing_optimization();

	_getch();
    return 0;
}

