/*
 * Node.h
 *
 *  Created on: Apr 17, 2015
 *      Author: assma
 */

#ifndef NODE_H_
#define NODE_H_

class Entry;
#include <iostream>
#include <vector>

class Entry;

class Node{
private:
	int num_dimen;
	std::vector<Entry*> entries;
	Node* parent;
	bool Root;
	bool Leaf;
	int maxim;
	int minim;
public:
	//Node();
	Node(int nd, std::vector<Entry*> ent, Node* p, int M, int m, bool leaf);
	Node(int nd, std::vector<Entry*> ent, int M, int m, bool leaf);
	int getNumDimensions();
	std::vector<Entry*> getEntries();
	Entry* get_ithEntry(int i);
	std::vector<Entry*> & refEntries(){return entries;};
	Node* getParent();
	bool isRoot();
	bool isLeaf();
	int getMax();
	int getMin();
	bool equal(Node* other) const;
	void addEntry(Entry* e);
	void removeEntry(Entry* e);
	void set_Entries(std::vector<Entry*> newEntries);
	void set_ithEntry(int i, Entry* e);
	void setIsRoot(bool r);
	void setIsLeaf(bool l);
	void setParent(Node* p);
};

#endif /* NODE_H_ */
