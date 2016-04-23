// EECS 2510 : AVL vs BTREE
// David Carek

#include "stdafx.h"
#include "BTree.h"
#include <fstream>
#include <iostream>
#include "globals.h"

// open the file when creating the tree
BTree::BTree(std::string treeFilePath)
{
	treeFile.open(treeFilePath, std::ios::binary | std::ios::trunc | std::ios::in | std::ios::out);
	if (treeFile.fail())
		std::cout << "failed to open tree file" << std::endl;
}

// close the file when the tree is destructed 
BTree::~BTree()
{
	treeFile.close();
}

// reads a node from the file given an index of the file in order to generate an offset
BTree::Node BTree::readFromDisk(int index)
{
	Node node;
	treeFile.seekg(index * sizeof(Node));
	treeFile.read((char *)&node, sizeof(Node));
	reads++;
	return node;
}

// writes the node to a given location of the output file based on the index of that node
void BTree::writeToDisk(BTree::Node node)
{
	treeFile.seekp(node.index * sizeof(Node));
	char * buffer = (char *)&node;
	treeFile.write(buffer, sizeof(Node));
	writes++;
	treeFile.flush();
}

// recursively checks to see if the input is already in the tree. if it is increment the count 
// of that key and 
bool BTree::isRepeat(Node node, char input[maxInputSize])
{
	int i = 1;
	// loop through each key checking the its value against the input 
	while (i <= node.numberOfKeys && strcmp(input, node.keys[i]) > 0)
		i++;
	// if the key matches the input we return true after incrementing the count of that key
	if (i <= node.numberOfKeys && strcmp(input, node.keys[i]) == 0)
	{
		node.count[i]++;
		writeToDisk(node);
		return true;
	}
	// else if we have reached a leaf then we are inserting a new item
	else if (node.isLeaf)
		return false;
	// else get the next node and check the keys
	else
	{
		node = readFromDisk(node.children[i]);
		return isRepeat(node, input);
	}
}

// insert the input into a node in the b tree
void BTree::insert(char input[maxInputSize])
{
	itemsInTree++;
	Node r;

	// if the root is empty we write a new node to the file
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

	// check the tree for a repeat insert
	if (isRepeat(r, input))
		return;

	uniqueItems++;
	
	// if the root is full split it 
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
	// else insert the input into the tree
	else
		insertNonFull(r, input);
	
}

// inserts the input into a node and then saves the changes to the tree
void BTree::insertNonFull(Node x, char input[maxInputSize])
{
	int i = x.numberOfKeys;
	// if we're inserting into a leaf start at the last key sliding all of the other keys over
	// until we've reached the location that the key will be inserted into, then write it to the file
	if (x.isLeaf)
	{
		while (i >= 1 && (strcmp(input, x.keys[i]) < 0))
		{
			strcpy_s(x.keys[i + 1], x.keys[i]);
			x.count[i + 1] = x.count[i];
			i--;
		}
		strcpy_s(x.keys[i + 1], input);
		x.count[i + 1] = 1;
		x.numberOfKeys++;
		writeToDisk(x);
	}
	// else find the key whose child we need to traverse to
	else
	{
		while (i >= 1 && (strcmp(input, x.keys[i]) < 0))
			i--;
		i++;

		// if the node we will be inserting into is full split it before we insert
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

// take node x's child at position i and split it into 2 children and attach them to x
void BTree::splitChild(Node x, int i) 
{
	nodeCount++;
	Node y = readFromDisk(x.children[i]);
	Node z;
	z.index = nodeCount;
	z.isLeaf = y.isLeaf;
	z.numberOfKeys = treeDegree - 1;

	// copy half of ys keys into z
	for (int j = 1; j <= treeDegree - 1; j++)
	{
		strcpy_s(z.keys[j], y.keys[j + treeDegree]);
		z.count[j] = y.count[j + treeDegree];
	}

	// if y is not a leaf fix the children corresponding to the ones copied over
	if (!y.isLeaf)
		for (int j = 1; j <= treeDegree; j++)
			z.children[j] = y.children[j + treeDegree];

	// fix the number of keys for the split
	y.numberOfKeys = treeDegree - 1;

	// fix xs children
	for (int j = x.numberOfKeys + 1; j >= i + 1; j--)
		x.children[j + 1] = x.children[j];

	x.children[i + 1] = z.index;

	// shift xs keys to make room for the key that gets pushed up
	for (int j = x.numberOfKeys; j >= i; j--)
	{
		strcpy_s(x.keys[j + 1], x.keys[j]);
		x.count[j + 1] = x.count[j];
	}
		
	strcpy_s(x.keys[i], y.keys[treeDegree]);
	x.count[i] = y.count[treeDegree];
	x.numberOfKeys++;
	// write all of the corrected nodes to the tree
	writeToDisk(y);
	writeToDisk(z);
	writeToDisk(x);
}

// sets the tree height based on number of nodes in the tree
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

// print the statistics for inserting into the BTree
void BTree::printStats()
{
	setStats();
	std::cout
		<< "<---------BTree Statistics--------->" << std::endl
		<< "Tree degree : " << treeDegree << std::endl
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

// sets the time taking to insert a file into the tree
void BTree::setInsertTime(std::chrono::duration<double> time)
{
	totalInsertTime = time;
}