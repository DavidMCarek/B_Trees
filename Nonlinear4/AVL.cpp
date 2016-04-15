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
	treeFile.setf(std::ios::binary);
	treeFile.open(treeFilePath);
	if (treeFile.fail())
		std::cout << "failed to open file" << std::endl;
}

AVL::~AVL()
{
	treeFile.close();
}

void AVL::writeToDisk(Node node, unsigned int index)
{
	treeFile.open(treeFilePath);
	treeFile.seekp(index * sizeof(Node), treeFile.beg);
	char * buffer = (char *)&node;
	treeFile.write(buffer, sizeof(Node));
	treeFile.close();
}

AVL::Node AVL::readFromDisk(unsigned int index)
{
	treeFile.open(treeFilePath);
	Node node;
	treeFile.seekg(index * sizeof(Node));
	treeFile.read((char *)&node, sizeof(Node));
	treeFile.close();
	return node;
}

// inserts a node into the tree and then checks balance factors to decide if a rotation is needed
// to maintain the balance of the tree
void AVL::insert(char input[50])
{
	node1 = readFromDisk(root);
	
	// If the root is null we only need to do a dumb insert and nothing else
	if (node1.index == -1)
	{
		for (int i = 0; i < 50; i++)
			node1.value[i] = input[i];

		node1.count = 1;
		node1.index = 0;
		writeToDisk(node1, 0);
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
		if (input == node1.value)
		{
			node1.count++;
			writeToDisk(node1, node1.index);
			return;
		}

		if (node1.balanceFactor != 0)
		{
			a = p;
			f = q;
		}

		q = p;
		p = (input < node1.value) ? node1.leftChild : node1.rightChild;
		node1 = (input < node1.value) ? readFromDisk(node1.leftChild) : readFromDisk(node1.rightChild);
	}

	// Now the previous node is a leaf and the current node is the child of that leaf so we need
	// to create a new node to insert

	// node to insert is node1
	for (int i = 0; i < 50; i++)
		node1.value[i] = input[i];
	node1.count = 1;
	node1.index = uniqueInserts;
	writeToDisk(node1, node1.index);
	int y = node1.index;

	int b; // this is the child of the most recent nonzero balance factor in the direction of the potential rotation
	int displacement; // this is used to correct balance factors later

	node2 = readFromDisk(q);
	// we need to know if the new node we are inserting is the left or the right child of the previous node so we
	// can have the correct child pointer point to the new node
	if (input < node2.value)
		node2.leftChild = node1.index;
	else
		node2.rightChild = node1.index;

	writeToDisk(node2, node2.index);

	// if the value of the node we just inserted is less than that of the most recent nonzero balance factor node
	// then we went left so the pivot needs to be the left child else its the right child
	// the displacement is set based on the direction taken

	node3 = readFromDisk(a);

	if (input > node3.value)
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
		if (input > node1.value)
		{
			node1.balanceFactor = -1;
			p = node1.rightChild;
			writeToDisk(node1, node1.index);
			node1 = readFromDisk(node1.rightChild);
		}
		else
		{
			node1.balanceFactor = 1;
			p = node1.leftChild;
			writeToDisk(node1, node1.index);
			node1 = readFromDisk(node1.leftChild);
		}
	}

	// if the tree was completely balanced recentNonZero will be at the root of the tree and our insert will
	// have pushed the tree slightly out of balance
	if (0 == node3.balanceFactor)
	{
		node3.balanceFactor = displacement;
		writeToDisk(node3, node3.index);
		return;
	}

	// if the most reent nonzero balance factor is opposite the displacement then we put the tree back in balance 
	if (node3.balanceFactor == -displacement)
	{
		node3.balanceFactor = 0;
		writeToDisk(node3, node3.index);
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
		}
	}

	writeToDisk(node1, node1.index);
	writeToDisk(node2, node2.index);
	writeToDisk(node3, node3.index);

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

	writeToDisk(node1, node1.index);
}

void AVL::setFilePath(std::string filePath)
{
	this->filePath = filePath;
}