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

BTree::Node BTree::readFromDisk(int index)
{

}

void BTree::writeToDisk(BTree::Node node)
{

}

void BTree::insert(char input[30])
{
	node1 = readFromDisk(root);
}

void BTree::splitChild(Node x, int i) 
{
	node1 = readFromDisk(x.index);
	node2 = readFromDisk(node1.children[i]);
	node3.isLeaf = node2.isLeaf;
	node3.numberOfKeys = treeDegree - 1;

	for (int j = 1; j <= treeDegree - 1; j++)
		node3.keys[j] = node2.keys[j + treeDegree];

	if (!node2.isLeaf)
		for (int j = 1; j <= treeDegree; j++)
			node3.children[j] = node2.children[j + treeDegree];

	node2.numberOfKeys = treeDegree - 1;

	for (int j = node1.numberOfKeys + 1; j >= i + 1; j--)
		node1.children[j + 1] = node1.children[j];

	node1.children[i + 1] = node3.index;

	for (int j = node1.numberOfKeys; j >= i; j--)
		node1.keys[j + 1] = node1.keys[j];

	node1.keys[i] = node2.keys[treeDegree];
	node1.numberOfKeys = node1.numberOfKeys + 1;
	writeToDisk(node2);
	writeToDisk(node3);
	writeToDisk(node1);
}