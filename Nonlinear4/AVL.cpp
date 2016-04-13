// EECS 2510 : BST VS AVL VS RBT
// David Carek

#include "stdafx.h"
#include "AVL.h"
#include <string>
#include <iostream>
#include <fstream>

AVL::AVL(std::string treeFilepath)
{
	this->treeFilePath = treeFilePath;
	treeFile.setf(std::ios::binary);
	treeFile.open(treeFilepath);
	if (treeFile.fail())
		std::cout << "failed to open file" << std::endl;
	treeFile.close();
}


AVL::~AVL()
{
	treeFile.close();
}

void AVL::writeToDisk(Node node, unsigned int offset)
{
	treeFile.open(treeFilePath);
	treeFile.seekp(offset, treeFile.beg);
	char * buffer = (char *)&node;
	treeFile.write(buffer, sizeof(Node));
	treeFile.close();
}

AVL::Node AVL::readFromDisk(unsigned int offset)
{
	Node node;
	treeFile.open(treeFilePath);
	treeFile.read((char *)&node, sizeof(Node));
	treeFile.close();
	return node;
}

// inserts a node into the tree and then checks balance factors to decide if a rotation is needed
// to maintain the balance of the tree
void AVL::insert(char input[50])
{
	Node foo = readFromDisk(0);
	if (uniqueInserts == 0)
	{
		Node node;
		for (int i = 0; i < 50; i++)
			node.value[i] = input[i];
		
		writeToDisk(node, 0);
		uniqueInserts++;
		return;
	}

	Node currentNode = readFromDisk(0);
	




	std::fstream file;
	file.setf(std::ios::binary);
	file.open("c:\\Users\\DMCar\\Desktop\\test.txt");
	if (file.fail())
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}

	Node root;
	file.read((char *)&root, sizeof(root));
	if (root.value)
	{

	}
}

void AVL::setFilePath(std::string filePath)
{
	this->filePath = filePath;
}