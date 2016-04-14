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
	AVL avl("C:\\Users\\DMCar\\Desktop\\test.txt");
	char input[50] {0};

	input[0] = 'f';
	input[1] = 'o';
	input[2] = 'o';
	avl.insert(input);

	input[0] = 'b';
	input[1] = 'a';
	input[2] = 'r';
	avl.insert(input);
	
    return 0;
}

