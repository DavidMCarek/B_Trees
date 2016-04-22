#pragma once
#include <string>
#include <chrono>
#include <fstream>
#include "globals.h"

class BTree
{
public:
	BTree(std::string inputTreeFile);
	~BTree(); 
	void insert(char input[maxInputSize]);
	void printStats();
	void setInsertTime(std::chrono::duration<double> time);
private:
	struct Node {
		int numberOfKeys = 0;
		char keys[2 * treeDegree][maxInputSize] = { 0 };
		int count[2 * treeDegree] = { 0 };
		int children[2 * treeDegree + 1] = { 0 };
		int index = 0;
		bool isLeaf = false;
	};
	void insertNonFull(Node node, char input[maxInputSize]);
	int uniqueItems = 0;
	int treeHeight = -1;
	long itemsInTree = 0;
	std::chrono::duration<double> totalInsertTime;
	bool isRepeat(Node node, char input[maxInputSize]);
	void setStats();
	void splitChild(Node node, int i);
	unsigned int nodeCount = 0;
	int root = 0;
	Node readFromDisk(int index);
	void writeToDisk(Node node);
	std::fstream treeFile;
	int reads = 0;
	int writes = 0;
};

