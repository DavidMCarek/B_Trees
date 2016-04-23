// EECS 2510 : AVL vs BTREE
// David Carek

#pragma once
#include <string>
#include <chrono>
#include <fstream>
#include "globals.h"

// Produces a data structure with 2 or more keys on a node to reduce the amount of reading
// and writing for disk based operations. This makes the processor take a slight hit in performance 
// but since disk operations are more costly, speed is increased greatly
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

