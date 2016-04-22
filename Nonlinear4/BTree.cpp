#include "stdafx.h"
#include "BTree.h"
#include <fstream>
#include <iostream>
#include "globals.h"

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

bool BTree::isRepeat(Node node, char input[maxInputSize])
{
	int i = 1;
	while (i <= node.numberOfKeys && strcmp(input, node.keys[i]) > 0)
		i++;
	if (i <= node.numberOfKeys && strcmp(input, node.keys[i]) == 0)
	{
		node.count[i]++;
		writeToDisk(node);
		return true;
	}
	else if (node.isLeaf)
		return false;
	else
	{
		node = readFromDisk(node.children[i]);
		return isRepeat(node, input);
	}
}

void BTree::insert(char input[maxInputSize])
{
	itemsInTree++;
	Node r;

	if (root == 0)
	{
		r.count[1] = 1;
		root = r.index = 1;
		r.isLeaf = true;
		strcpy_s(r.keys[1], input);
		r.numberOfKeys++;
		writeToDisk(r);
		nodeCount++;
		uniqueItems++;
		return;
	}

	r = readFromDisk(root);

	if (isRepeat(r, input))
		return;

	uniqueItems++;
		
	if (r.numberOfKeys == (2 * treeDegree - 1))
	{
		Node s;
		nodeCount++;
		root = s.index = nodeCount;
		s.isLeaf = false;
		s.numberOfKeys = 0;
		s.children[1] = r.index; 
		splitChild(s ,1);
		s = readFromDisk(s.index);
		insertNonFull(s, input);
	}
	else
		insertNonFull(r, input);
	
}

void BTree::insertNonFull(Node x, char input[maxInputSize])
{
	int i = x.numberOfKeys;
	if (x.isLeaf)
	{
		while (i >= 1 && (strcmp(input, x.keys[i]) < 0))
		{
			strcpy_s(x.keys[i + 1], x.keys[i]);
			x.count[i + 1] = x.count[i];
			i = i - 1;
		}
		strcpy_s(x.keys[i + 1], input);
		x.count[i + 1] = 1;
		x.numberOfKeys++;
		writeToDisk(x);
	}
	else
	{
		while (i >= 1 && (strcmp(input, x.keys[i]) < 0))
			i--;
		i++;

		if (readFromDisk(x.children[i]).numberOfKeys == (2 * treeDegree - 1))
		{
			splitChild(x, i);
			x = readFromDisk(x.index);
			if (strcmp(input, x.keys[i]) > 0)
				i++;
		}
		insertNonFull(readFromDisk(x.children[i]), input);
	}		
}

void BTree::splitChild(Node x, int i) 
{
	nodeCount++;
	Node y = readFromDisk(x.children[i]);
	Node z;
	z.index = nodeCount;
	z.isLeaf = y.isLeaf;
	z.numberOfKeys = treeDegree - 1;

	for (int j = 1; j <= treeDegree - 1; j++)
	{
		strcpy_s(z.keys[j], y.keys[j + treeDegree]);
		z.count[j] = y.count[j + treeDegree];
	}

	if (!y.isLeaf)
		for (int j = 1; j <= treeDegree; j++)
			z.children[j] = y.children[j + treeDegree];

	y.numberOfKeys = treeDegree - 1;

	for (int j = x.numberOfKeys + 1; j >= i + 1; j--)
		x.children[j + 1] = x.children[j];

	x.children[i + 1] = z.index;

	for (int j = x.numberOfKeys; j >= i; j--)
	{
		strcpy_s(x.keys[j + 1], x.keys[j]);
		x.count[j + 1] = x.count[j];
	}
		
	strcpy_s(x.keys[i], y.keys[treeDegree]);
	x.count[i] = y.count[treeDegree];
	x.numberOfKeys++;
	writeToDisk(y);
	writeToDisk(z);
	writeToDisk(x);
}

void BTree::setStats()
{
	treeHeight = 0;
	if (root == 0)
		return;

	Node node = readFromDisk(root);

	// if the set is not empty traverse the list in order and output the node values and counts
	while (!node.isLeaf)
	{
		node = readFromDisk(node.children[1]);
		treeHeight++;
	}
}

void BTree::printStats()
{
	setStats();
	std::cout
		<< "<---------BTree Statistics--------->" << std::endl
		<< "Tree height : " << treeHeight << std::endl
		<< "Total items : " << itemsInTree << std::endl
		<< "Unique items : " << uniqueItems << std::endl
		<< "Number of nodes : " << nodeCount << std::endl
		<< "Total reads : " << reads << std::endl
		<< "Total writes : " << writes << std::endl;
	printf("Insert time : %.3f s\n", totalInsertTime.count());
	std::cout
		<< "<---------------------------------->" << std::endl << std::endl;
}

void BTree::setInsertTime(std::chrono::duration<double> time)
{
	totalInsertTime = time;
}