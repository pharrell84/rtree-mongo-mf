/*
 * Node.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: assma
 */

#pragma warning(disable:4996) // vs weird error

#include "Entry.h"
#include <iostream>
#include <algorithm>
#include "BoundingBox.h"
#include "Node.h"

//Node::Node(){ }

Node::Node(int nd, std::vector<Entry*> ent, Node* p, int M, int m, bool leaf){
	num_dimen = nd;
	maxim = M;
	minim = m;
	Leaf = leaf;
	Root = false;
	entries = ent;
	parent = p;
}
Node::Node(int nd, std::vector<Entry*> ent, int M, int m, bool leaf){
	num_dimen = nd;
	maxim = M;
	minim = m;
	Leaf = leaf;
	Root = false;
	entries = ent;
	parent = NULL;

}
int Node::getNumDimensions(){return num_dimen;}
std::vector<Entry*> Node::getEntries(){return entries;}
Entry* Node::get_ithEntry(int i){return entries[i];}
Node* Node::getParent(){return parent;}
bool Node::isRoot(){return Root;}
bool Node::isLeaf(){return Leaf;}
int Node::getMax(){return maxim;}
int Node::getMin(){return minim;}
bool Node::equal(Node* other) const {
	//return true;
	return (num_dimen == other->getNumDimensions()
		&& Leaf == other->isLeaf() && Root == other->isRoot()
		&& maxim == other->getMax() && minim == other->getMin()
		&& entries == other->getEntries()); //std::equal(entries.begin(), entries.end(), other.getEntries())); // ERROR 
}

void Node::addEntry(Entry* e){entries.push_back(e);}
// void Node::removeEntry(Entry e){entries.erase(std::remove(entries.begin(), entries.end(), e), entries.end());}
void Node::removeEntry(Entry* e){ 
	int pos = std::find(entries.begin(), entries.end(), e) - entries.begin(); // ERROR CANT COMPARE ENTRY OBJ
	entries.erase(entries.begin()+pos);
	//entries.erase(std::find(entries.begin(), entries.end(), e) - entries.begin()); 
}
void Node::set_Entries(std::vector<Entry*> newEntries){entries = newEntries;} //
void Node::set_ithEntry(int i, Entry* e){entries[i] = e;}
void Node::setIsRoot(bool r){Root = r;}
void Node::setIsLeaf(bool l){Leaf = l;}
void Node::setParent(Node* p){
	parent = p;
	Root = false;
}




