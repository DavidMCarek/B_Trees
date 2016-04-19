// EECS 2510 : BST VS AVL VS RBT
// David Carek

#include "stdafx.h"
#include "AVL.h"
#include <string>
#include <iostream>
#include <fstream>

AVL::AVL(std::string treeFilePath)
{
	this->treeFilePath = treeFilePath;
	inputTreeFile.open(treeFilePath, std::ios::binary);
	if (inputTreeFile.fail())
		std::cout << "failed to open input file" << std::endl;
	outputTreeFile.open(treeFilePath, std::ios::binary | std::ios::trunc);
	if (outputTreeFile.fail())
		std::cout << "failed to open output file" << std::endl;
}

AVL::~AVL()
{
	inputTreeFile.close();
	outputTreeFile.close();
}

void AVL::writeToDisk(Node node)
{
	outputTreeFile.seekp(node.index * sizeof(Node), std::ios::beg);
	char * buffer = (char *)&node;
	outputTreeFile.write(buffer, sizeof(Node));
	outputTreeFile.flush();
}

AVL::Node AVL::readFromDisk(int index)
{
	Node node;
	if (index == -1)
		return node;
	inputTreeFile.seekg(index * sizeof(Node));
	inputTreeFile.read((char *)&node, sizeof(Node));
	return node;
}

// inserts a node into the tree and then checks balance factors to decide if a rotation is needed
// to maintain the balance of the tree
void AVL::insert(char input[30])
{
	node1 = readFromDisk(root);
	
	// If the root is null we only need to do a dumb insert and nothing else
	if (node1.index == -1)
	{
		strcpy_s(node1.value, input);
		node1.count = 1;
		node1.index = 0;
		writeToDisk(node1);
		root = 0;
		uniqueInserts++;
		return;
	}

	int p; // our current location on the tree
	int q; // lags behind current node
	int a; // the last parent of current node that had a balance factor of +- 1
	int f; // lags behind recent nonzero

	p = a = node1.index;
	q = f = -1;

	// while the current node is not at the child of a leaf or a duplicate value keep traversing through the tree
	// keeping track of the most recent nonzero balance factor node



	while (p != -1)
	{
		if (strcmp(input, node1.value) == 0)
		{
			node1.count++;
			writeToDisk(node1);
			return;
		}

		if (node1.balanceFactor != 0)
		{
			a = p;
			f = q;
		}

		q = p;
		p = (strcmp(input, node1.value) < 0) ? node1.leftChild : node1.rightChild;
		node1 = (strcmp(input, node1.value) < 0) ? readFromDisk(node1.leftChild) : readFromDisk(node1.rightChild);
	}

	// Now the previous node is a leaf and the current node is the child of that leaf so we need
	// to create a new node to insert

	// node to insert is node1
	strcpy_s(node1.value, input);
	node1.count = 1;
	node1.index = uniqueInserts;
	writeToDisk(node1);
	uniqueInserts++;
	int y = node1.index;

	int b; // this is the child of the most recent nonzero balance factor in the direction of the potential rotation
	int displacement; // this is used to correct balance factors later

	node2 = readFromDisk(q);
	// we need to know if the new node we are inserting is the left or the right child of the previous node so we
	// can have the correct child pointer point to the new node
	if (strcmp(input, node2.value) < 0)
		node2.leftChild = node1.index;
	else
		node2.rightChild = node1.index;

	writeToDisk(node2);
	// if the value of the node we just inserted is less than that of the most recent nonzero balance factor node
	// then we went left so the pivot needs to be the left child else its the right child
	// the displacement is set based on the direction taken

	node3 = readFromDisk(a);
	if (strcmp(input, node3.value) > 0)
	{
		p = node3.rightChild;
		displacement = -1;
	}
	else
	{
		p = node3.leftChild;
		displacement = 1;
	}
	b = p;

	node1 = readFromDisk(p);
	// then we traverse down from the most recent nonzero balance factor node to the node we inserted setting balance factors
	// on the way down
	while (p != y)
	{
		if (strcmp(input, node1.value) > 0)
		{
			node1.balanceFactor = -1;
			p = node1.rightChild;
			writeToDisk(node1);
			node1 = readFromDisk(node1.rightChild);
		}
		else
		{
			node1.balanceFactor = 1;
			p = node1.leftChild;
			writeToDisk(node1);
			node1 = readFromDisk(node1.leftChild);
		}
	}

	// if the tree was completely balanced recentNonZero will be at the root of the tree and our insert will
	// have pushed the tree slightly out of balance
	if (0 == node3.balanceFactor)
	{
		node3.balanceFactor = displacement;
		writeToDisk(node3);
		return;
	}

	// if the most reent nonzero balance factor is opposite the displacement then we put the tree back in balance 
	if (node3.balanceFactor == -displacement)
	{
		node3.balanceFactor = 0;
		writeToDisk(node3);
		return;
	}

	// a b and c correspond to node1, node2, and node3
	node1 = readFromDisk(a);
	node2 = readFromDisk(b);
	//node3 = c

	// At this point we have thrown the tree out of balance by inserting
	// The displacement tells us the first direction of the rotation and the most recent non-zero balance factor node (b, node2) 
	// tells us the second direction
	if (displacement == 1)
	{
		if (node2.balanceFactor == 1) //LL
		{
			// set a's left child to b's right and b's right to a
			// then fix balance factors
			node1.leftChild = node2.rightChild;
			node2.rightChild = a;
			node1.balanceFactor = node2.balanceFactor = 0;
		}
		else //LR
		{
			// create a new node c that is b's right child
			node3 = readFromDisk(node2.rightChild);

			// for this rotation the order of a, b, and c are b < c < a
			// so c gets pulled up to the middle and sets its children to b (left) and a (right)
			// this cuts off c's children though so prior to this c's left needs to be attached as b's right
			// and c's right is attached as a's left

			// then attach c's children to a and b
			node1.leftChild = node3.rightChild;
			node2.rightChild = node3.leftChild;

			// then set c's children to b and a
			node3.leftChild = b;
			node3.rightChild = a;

			// then we set a and b's balance factors to 0 and correct one of them depending on what
			// c's balance factor
			node1.balanceFactor = node2.balanceFactor = 0;

			if (node3.balanceFactor == 1)
				node1.balanceFactor = -1;
			else if (node3.balanceFactor == -1)
				node2.balanceFactor = 1;

			// then c's balance factor is fixed and set to 0
			node3.balanceFactor = 0;
			b = node3.index; // this is for reattaching the subtree to the proper parent
			writeToDisk(node3);
		}
	}
	else // again the next parts are symmetric so almost all the operations are just flipped
	{
		if (node2.balanceFactor == -1) //RR
		{
			node1.rightChild = node2.leftChild;
			node2.leftChild = a;

			node1.balanceFactor = node2.balanceFactor = 0;
		}
		else //RL
		{
			node3 = readFromDisk(node2.leftChild);
			node1.rightChild = node3.leftChild;
			node2.leftChild = node3.rightChild;
			node3.rightChild = b;
			node3.leftChild = a;

			node1.balanceFactor = node2.balanceFactor = 0;

			if (node3.balanceFactor == 1)
				node2.balanceFactor = -1;
			else if (node3.balanceFactor == -1)
				node1.balanceFactor = 1;

			node3.balanceFactor = 0;
			b = node3.index;

			writeToDisk(node3);
		}
	}

	writeToDisk(node1);
	writeToDisk(node2);

	// if the parent of the recent non zero balance factor node was null then there were no nodes with a nonzero balance
	// or the only one was the root. in either case the recent non zero was the root so whatever is in position b needs to 
	// be set as the root
	if (f == -1)
	{
		root = b;
		return;
	}

	node1 = readFromDisk(f);

	// if the left child of the recent nonzero BF parent node is the recent nonzero node we need to attach the new
	// root of the subtree (b) in a's place
	if (node1.leftChild == a)
		node1.leftChild = b;
	else // otherwise its the right child that needs reattached
		node1.rightChild = b;

	writeToDisk(node1);
}

void AVL::setStats()
{
	treeHeight = 0;
	itemsInTree = 0;
	if (root == -1)
		return;

	// if the set is not empty traverse the list in order and output the node values and counts
	traverseSetStats(readFromDisk(root), treeHeight);
}

void AVL::traverseSetStats(Node node, int nodeHeight)
{
	itemsInTree += node.count;
	if (node.leftChild != -1)
		traverseSetStats(readFromDisk(node.leftChild), nodeHeight + 1);

	if (node.rightChild != -1)
		traverseSetStats(readFromDisk(node.rightChild), nodeHeight + 1);

	if (nodeHeight > treeHeight)
		treeHeight = nodeHeight;
}

void AVL::printStats()
{
	setStats();
	std::cout
		<< "<----------AVL Statistics---------->" << std::endl
		<< "Tree height : " << treeHeight << std::endl
		<< "Total items : " << itemsInTree << std::endl
		<< "Unique items : " << uniqueInserts << std::endl
		<< "Number of nodes : " << uniqueInserts << std::endl
		<< "<---------------------------------->" << std::endl << std::endl;
}