/*
* Entry.h
*
*  Created on: Apr 17, 2015
*      Author: assma
*/

#ifndef ENTRY_H_
#define ENTRY_H_

class Node;
class BoundingBox;
#include "Node.h"
#include "BoundingBox.h"
#include <unordered_map>
#include <string>

class Entry
{
private:
	Node* childNode;
	std::unordered_map<int, std::string> childDoc;
	BoundingBox I;
	bool inLeaf;
public:
	Entry(BoundingBox i, Node* c);
	Entry(BoundingBox i, std::unordered_map<int, std::string> c);
	Node* getChildPointer();
	std::unordered_map<int, std::string> getTupleID();
	std::unordered_map<int, std::string> & refTupleID(){ return childDoc; };
	BoundingBox getI() const;
	BoundingBox & refI() { return I; };
	int getNumDimen();
	bool getInLeaf();
	void setInLeaf(bool l);
	void setChild(Node* c);
	void setChild(std::unordered_map<int, std::string> h);
	void setI(BoundingBox i);
	bool equal(const Entry* other) const;
};

bool operator==(const Entry& lhs, const Entry& rhs);

#endif /* ENTRY_H_ */



