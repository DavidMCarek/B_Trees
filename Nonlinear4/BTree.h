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
	int numberOfKeys = 11;
	struct Key {
		int count = 0;
		char value[30];
	};
	struct Node {
		Key keys[11];
		int children[12] = { 0 };
		int index = -1;
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
	int root = -1;
	Node readFromDisk(int index);
	void writeToDisk(Node node);
	std::ifstream inputTreeFile;
	std::ofstream outputTreeFile;
	std::string treeFilePath;
};

