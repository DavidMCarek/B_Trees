// EECS 2510 : BST VS AVL VS RBT
// David Carek

#include "stdafx.h"
#include "AVL.h"
#include <string>
#include <iostream>
#include <fstream>

AVL::AVL()
{
}


AVL::~AVL()
{
}

// inserts a node into the tree and then checks balance factors to decide if a rotation is needed
// to maintain the balance of the tree
void AVL::insert(char * input[50])
{
	int length = 50;
	std::fstream file;
	file.open(filePath);
	if (file.fail())
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}

	file.seekg(0, file.beg);
	char * buffer = new char[length];
	file.read(buffer, length);

}

void AVL::setFilePath(std::string filePath)
{
	this->filePath = filePath;
}