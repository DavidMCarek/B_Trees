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
	struct Node {
		int leftChild = 0;
		int rightChild = 0;
		char value[50];
		int count = 1;
		int balanceFactor = 0;
	};

	Node node;
	for (int i = 49; i > -1; i--)
	{
		node.value[i] = 0;
	}
	node.value[0] = 'f';
	node.value[1] = 'o';
	node.value[2] = 'o';

	int nodeSize = sizeof(node);
	fstream file;
	file.setf(ios::binary);
	file.open("c:\\Users\\DMCar\\Desktop\\test.txt");
	if (file.fail())
	{
		cout << "Could not open file" << endl;
		return 0;
	}

	file.seekp(0, file.beg);
	char * buffer = (char *)&node;
	file.write(buffer, nodeSize);

	file.seekg(0, file.beg);
	Node n2;
	file.read((char *)&n2, nodeSize);

	

    return 0;
}

