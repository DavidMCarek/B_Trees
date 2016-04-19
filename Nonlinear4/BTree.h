#pragma once
#include <string>
#include <chrono>
#include <fstream>
class BTree
{
public:
	BTree();
	~BTree(); 
	void insert(char input[30]);
	void printStats();
	void setFilePath(std::string filePath);
private:
	std::ifstream inputTreeFile;
	std::ofstream outputTreeFile;
	std::string treeFilePath;
	struct Node {
		int leftChild = -1;
		int rightChild = -1;
		char value[30];
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
	std::ifstream inputTreeFile;
	std::ofstream outputTreeFile;
	std::string treeFilePath;
};

