#include "stdafx.h"
#include "BTree.h"
#include <fstream>
#include <iostream>

BTree::BTree(std::string treeFilePath)
{
	treeFile.open(treeFilePath, std::ios::binary | std::ios::trunc | std::ios::in | std::ios::out);
	if (treeFile.fail())
		std::cout << "failed to open tree file" << std::endl;
}


BTree::~BTree()
{
	treeFile.close();
}

BTree::Node BTree::readFromDisk(int index)
{
	Node node;
	if (index == -1)
		return node;
	treeFile.seekg(index * sizeof(Node));
	treeFile.read((char *)&node, sizeof(Node));
	reads++;
	return node;
}

void BTree::writeToDisk(BTree::Node node)
{
	treeFile.seekp(node.index * sizeof(Node));
	char * buffer = (char *)&node;
	treeFile.write(buffer, sizeof(Node));
	writes++;
	treeFile.flush();
}


void BTree::insert(char input[30])
{
	node1 = readFromDisk(root);

	if (node1.numberOfKeys == (2 * treeDegree - 1))
	{
		root = uniqueInserts;
		memset(node2.children, 0, sizeof(node2.children));
	}
		

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


void BTree::setStats()
{
	treeHeight = 0;
	itemsInTree = 0;
	if (root == 0)
		return;

	// if the set is not empty traverse the list in order and output the node values and counts
	traverseSetStats(readFromDisk(root), treeHeight);
}

void BTree::traverseSetStats(Node node, int nodeHeight)
{

}

void BTree::printStats()
{
	setStats();
	std::cout
		<< "<----------AVL Statistics---------->" << std::endl
		<< "Tree height : " << treeHeight << std::endl
		<< "Total items : " << itemsInTree << std::endl
		<< "Unique items : " << uniqueInserts << std::endl
		<< "Number of nodes : " << uniqueInserts << std::endl
		<< "Total reads : " << reads << std::endl
		<< "Total writes : " << writes << std::endl;
	printf("Insert time : %.3f s\n", totalInsertTime.count());
	std::cout
		<< "<---------------------------------->" << std::endl << std::endl;
}