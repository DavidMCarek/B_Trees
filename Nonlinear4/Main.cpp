// Nonlinear Data structures
// EECS 2510 : Lab 4
// David Carek

#include "stdafx.h"
#include "AVL.h"
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	AVL avl("c:\\Users\\DMCar\\Desktop\\test.txt");
	char input[50] {0};

	input[0] = 'f';
	input[1] = 'o';
	input[2] = 'o';
	avl.insert(input);
	
    return 0;
}

