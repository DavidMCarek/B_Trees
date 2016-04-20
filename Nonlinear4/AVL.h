// EECS 2510 : BST VS AVL VS RBT
// David Carek

// This class is used to create a type of height balanced tree called an AVL tree
// this tree uses balance factors to keep the trees height under 1.44lg(n)
#pragma once
#include <string>
#include <chrono>
#include <fstream>

class AVL
{
public:
	AVL(std::string treeFilePath);
	~AVL();
	void insert(char input[30]);
	void printStats();
	void setInsertTime(std::chrono::duration<double> time);
private:
	struct Node {
		int leftChild = -1;
		int rightChild = -1;
		char value[50];
		unsigned int count = 0;
		int balanceFactor = 0;
		int index = -1;
	};
	Node node1;
	Node node2;
	Node node3;
	int treeHeight = 0;
	long itemsInTree = 0;
	std::chrono::duration<double> totalInsertTime;
	void setStats();
	void traverseSetStats(Node node, int nodeHeight);
	unsigned int uniqueInserts = 0;
	int root = -1;
	Node readFromDisk(int index);
	void writeToDisk(Node node);
	std::fstream treeFile;
	int reads = 0;
	int writes = 0;
};

