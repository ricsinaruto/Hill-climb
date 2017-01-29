
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

/* PRÓBA ALGORITMUSOK */





int main(int argc, char** argv)
{

	simulated_annealing_optimization();
	//simulated_annealing();

	_getch();
    return 0;
}

