#pragma once
#include <string>
#include <chrono>
#include <fstream>
#define treeDegree 8
class BTree
{
public:
	BTree(std::string inputTreeFile);
	~BTree(); 
	void insert(char input[30]);
	void printStats();
private:
	struct Node {
		int numberOfKeys = 0;
		char keys[2 * treeDegree] = { 0 };
		int count[2 * treeDegree] = { 0 };
		int children[2 * treeDegree + 1] = { 0 };
		int index = -1;
		bool isLeaf = false;
	};
	Node node1;
	Node node2;
	Node node3;
	int treeHeight = 0;
	long itemsInTree = 0;
	std::chrono::duration<double> totalInsertTime;
	void setStats();
	void traverseSetStats(Node x, int nodeHeight);
	void splitChild(Node node, int i);
	unsigned int uniqueInserts = 0;
	int root = 0;
	Node readFromDisk(int index);
	void writeToDisk(Node node);
	std::fstream treeFile;
	int reads = 0;
	int writes = 0;
};

