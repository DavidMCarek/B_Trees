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

void BTree::insert(char input[30])
{

}

void BTree::splitChild(Node x, int i) 
{
	node1 = readFromDisk(x.index);
	node2 = readFromDisk(node1.children[i]);
	
		/*3  z.leaf = y.leaf
		4  z.n = t – 1
		5  for j = 1 to t – 1
		6       z.keyj = y.keyj + t
		7  if not y.leaf
		8       for j = 1 to t
		9            z.cj = y.cj + t
		10  y.n = t – 1
		11  for j = x.n + 1 downto i + 1
		12       x.cj + 1 = x.cj
		13  x.ci + 1 = z
		14  for j = x.n downto i
		15       x.keyj + 1 = x.keyj
		16  x.keyi = y.keyt
		17  x.n = x.n + 1
		18  Disk - Write(y)
		19  Disk - Write(z)
		20  Disk - Write(x)
*/
}