#include "stdafx.h"
#include "BTree.h"
#include <fstream>
#include <iostream>

BTree::BTree()
{
	this->treeFilePath = treeFilePath;
	inputTreeFile.open(treeFilePath, std::ios::binary);
	if (inputTreeFile.fail())
		std::cout << "failed to open input file" << std::endl;
	outputTreeFile.open(treeFilePath, std::ios::binary);
	if (outputTreeFile.fail())
		std::cout << "failed to open output file" << std::endl;
}


BTree::~BTree()
{
	inputTreeFile.close();
	outputTreeFile.close();
}
