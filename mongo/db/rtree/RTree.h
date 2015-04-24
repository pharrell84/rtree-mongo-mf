/*
 * RTree.h
 *
 *  Created on: Apr 17, 2015
 *      Author: assma
 */

#ifndef RTREE_H_
#define RTREE_H_

class BoundingBox;
class Entry;
class Node;

#include "Node.h"
#include <vector>
#include <algorithm>

extern bool didRun;

class RTree {
private:
	Node* root;
	int num_dimen;
	int maxim;
	int minim;
public:
	static RTree* theTree;
	RTree(int nd, Node* r, int M, int m);
	Node* getRoot();
	int getNumDimen();
	int getMax();
	int getMin();
	void setRoot(Node* r);
	void setMax(int M);
	void setMin(int m);
	std::vector<Entry*> search(BoundingBox* s);
	void insert(Entry* e);
	Node* chooseLeaf(Entry* e);
	void adjustTree(Node* L);
	void adjustTree(std::vector<Node*> newNodes);
	void deleteEntry(Entry* e);
	Node* findLeaf(Entry* e);
	void condenseTree(Node* L);
	std::vector<Node*> splitNode(Node* n);
	std::vector<Entry*> pickSeeds(Node* n);
	Entry* pickNext(std::vector<Entry*> right, std::vector<Entry*> left, std::vector<Entry*> remaining);
	BoundingBox smallestBoundingBox(std::vector<Entry*> entries);
};


#endif /* RTREE_H_ */