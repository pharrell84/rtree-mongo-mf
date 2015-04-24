/*
* RTree.cpp
*
*  Created on: Apr 12, 2015
*      Author: assma
*/

#include "BoundingBox.h"
#include "Entry.h"
#include "Node.h"
#include <iostream>
#include <limits>
#include <cmath>
#include "RTree.h"

using namespace std;

RTree* RTree::theTree;

RTree::RTree(int nd, Node* r, int M, int m) {
	root = r;
	num_dimen = nd;
	maxim = M;
	minim = m;
}

Node* RTree::getRoot(){ return root; }
int RTree::getNumDimen(){ return num_dimen; }
int RTree::getMax(){ return maxim; }
int RTree::getMin(){ return minim; }
void RTree::setRoot(Node* r){ root = r; }
void RTree::setMax(int M){ maxim = M; }
void RTree::setMin(int m){ minim = m; }

std::vector<Entry*> RTree::search(BoundingBox* s){
	std::vector<Entry*> results;
	std::vector<Entry*> entries = root->getEntries();

	//cout << "WE ARE SEARCHING ON LOWER " << s->get_ithLower(0) << "," << s->get_ithLower(1) << endl;
	//cout << "WE ARE SEARCHING ON UPPER " << s->get_ithUpper(0) << "," << s->get_ithUpper(1) << endl;

	if (root->isLeaf()){
		for (int i = 0; i<entries.size(); i++){
			BoundingBox b = entries.at(i)->getI();
			//cout << "CHECK OVERLAP FOR LOWER ROOT LEAF " << b.get_ithLower(0) << " " << b.get_ithLower(1) << endl;
			//cout << "CHECK OVERLAP FOR UPPER ROOT LEAF " << b.get_ithUpper(0) << " " << b.get_ithUpper(1) << endl;
			if (s->overlaps(b)){
				results.push_back(entries.at(i));
			}
		}
	}
	else{
		//cout << "CHECK OVERLAP FOR " << entries.size() << " ENTRIES" << endl;
		for (int i = 0; i<entries.size(); i++){
			BoundingBox b = entries.at(i)->getI();
			//cout << "CHECK OVERLAP FOR LOWER " << b.get_ithLower(0) << " " << b.get_ithLower(1) << endl;
			//cout << "CHECK OVERLAP FOR UPPER " << b.get_ithUpper(0) << " " << b.get_ithUpper(1) << endl;
			if (s->overlaps(b)){
				//cout << "SAW OVERLAP, CREATE SUBTREE" << endl;
				RTree* subTree = new RTree(num_dimen, entries.at(i)->getChildPointer(), maxim, minim);
				std::vector<Entry*> tmpRes = subTree->search(s);
				results.reserve(tmpRes.size());
				results.insert(results.end(), tmpRes.begin(), tmpRes.end());
				//results.addAll(subTree->search(s));
			}
		}
	}

	return results;

}

void RTree::insert(Entry* e){
	Node* leaf = chooseLeaf(e);
	//int numEnt = leaf->getEntries().size();
	int numEnt; //NEW CODE!!!
	if (leaf->getEntries().empty()){
		numEnt = 0;
	}
	else{
		numEnt = leaf->getEntries().size();
	}

	//cout << "in insert, numEnt = " << numEnt << endl;

	leaf->addEntry(e);

	//cout << "ADDED Entry" << endl;
	if (numEnt + 1> maxim){
		//cout << "SPLITNODE" << endl;
		std::vector<Node*> newNodes = splitNode(leaf);
		adjustTree(newNodes);// check if I need to pass leaf in too
	}
	else{
		//cout << "ADJUSTTREE" << endl;
		adjustTree(leaf);
	}

	//cout << leaf->getEntries().size() << endl;
	////cout << "Node's bounds: "
}
Node* RTree::chooseLeaf(Entry* e){
	//bounding box of entry to be inserted
	BoundingBox b = e->getI();

	Node* N = root;
	if (N->isLeaf()){
		//cout << "root is a leaf, insert directly in root" << endl;
		return N;
	}
	else{
		while (!N->isLeaf()){
			Entry* F = N->get_ithEntry(0);
			double minAddition = std::numeric_limits<double>::infinity();
			for (int i = 0; i<N->getEntries().size(); i++){
				Entry* current = N->get_ithEntry(i);
				BoundingBox currentI = current->getI();
				double addToCurrent = currentI.areaToAdd(b);
				if (addToCurrent < minAddition){
					minAddition = addToCurrent;
					F = current;
				}
				else if (addToCurrent == minAddition){
					if (currentI.calcArea()<F->getI().calcArea()){
						F = current;
					}
				}
			}
			N = F->getChildPointer();
		}


		return N;
	}
}

void RTree::adjustTree(Node* L){
	//cout << "inside adjustTree.." << endl;
	Node* N = L;

	if (root->getEntries().size() > 1) {
		//cout << "ADJT SPLIT NODE RIGHTP: " << root->getEntries().at(0)->getInLeaf();
		//cout << "ADJT SPLIT NODE LEFTP: " << root->getEntries().at(1)->getInLeaf();
	}

	if (!root->equal(N)){
		//cout << "Node is not root, we adjust..." << endl;
		Node* P = N->getParent();
		Entry* EN = NULL;
		int Psize;
		if (P->getEntries().empty()){
			Psize = 0;
		}
		else{
			Psize = P->getEntries().size();
		}
		//cout << "Psize = " << Psize << endl;
		for (int i = 0; i< Psize; i++){
			//cout << "inside for-loop..." << endl;
			Node* currentPointer = P->get_ithEntry(i)->getChildPointer();
			if (currentPointer->equal(N)){
				//cout << "in adjustTree: found correct parent entry" << endl;
				Entry* E = P->get_ithEntry(i);
				EN = E;
				break;
			}
		}
		if (EN != NULL){
			std::vector<Entry*> N_Entries = N->getEntries();
			BoundingBox newBounds = smallestBoundingBox(N_Entries);
			//cout << "adjusted bounds: " << newBounds.get_ithLower(0) << ", " << newBounds.get_ithLower(1) << " and " << newBounds.get_ithUpper(0) << ", " << newBounds.get_ithUpper(1) << endl;
			EN->setI(newBounds);
			adjustTree(P);
		}
	}
	else{
		//cout << "adjusting root entries" << endl;
		std::vector<Entry*> rootEnts = root->getEntries();
		int Rsize;
		if (rootEnts.empty()){
			Rsize = 0;
		}
		else{
			Rsize = rootEnts.size();
		}
		//cout << "root has " << Rsize << " entries." << endl;
		for (int i = 0; i<Rsize; i++){
			//cout << "adjusting entry number " << i << endl;
			Entry* currentEnt = rootEnts.at(i);
			std::vector<Entry*> allEnts;
			//cout << "CUR ENT INLEAF IS " << currentEnt->getInLeaf() << endl;
			if (!currentEnt->getInLeaf()){
				//cout << "we are not in leaf" << endl;
				Node* currentChild = currentEnt->getChildPointer();
				//next line causes a segmentation fault...
				//cout << "currentChild: " << currentChild << endl;
				std::vector<Entry*> childEnts = currentChild->getEntries();
				//cout << "got child entries.." << endl;
				allEnts = childEnts;
			}
			allEnts.push_back(currentEnt);
			BoundingBox newBounds = smallestBoundingBox(allEnts);
			currentEnt->setI(newBounds);
			//cout << "set new bounds for entry number " << i << " : " << newBounds.get_ithLower(0) << ", " << newBounds.get_ithLower(1) << " and " << newBounds.get_ithUpper(0) << ", " << newBounds.get_ithUpper(1) << endl;
		}
	}
}
bool rootSplit = false;
Node* lastSplit = NULL;
void RTree::adjustTree(std::vector<Node*> newNodes) {
	//cout << "inside adjustTree 2.." << endl;
	Node* N = newNodes[0];
	Node* NN = newNodes[1];
	//cout << "ADJ TREE 2, IS ROOT IS " << root->equal(N) << endl;
	//cout << "ADJ TREE 2, IS ROOT IS " << N->isRoot() << endl;
	//if (!root->equal(N)){
	if (!N->isRoot()){
		Node* P = N->getParent();
		if (rootSplit) { 
			//cout << "PARENT IS ROOT IN ADJ TREE 2" << endl; 
			return; 
		}
		//cout << "PARENT IS " << P << endl;
		Entry* EN = NULL;
		int Psize;
		if (P->getEntries().empty()){
			Psize = 0;
		}
		else{
			Psize = P->getEntries().size();
		}
		for (int i = 0; i< Psize; i++){
			Node* currentPointer = P->get_ithEntry(i)->getChildPointer();
			//if (currentPointer->equal(N)){
			if (currentPointer == lastSplit) {
				//cout << "FOUND N IN P AT " << i;
				P->getEntries().at(i)->setChild(N);
				Entry* E = P->get_ithEntry(i);
				E->setInLeaf(false);
				EN = E;
			}
		}
		if (EN != NULL){
			std::vector<Entry*> N_Entries = N->getEntries();
			BoundingBox ENnewBounds = smallestBoundingBox(N_Entries);
			EN->setI(ENnewBounds);

			std::vector<Entry*> NN_Entries = NN->getEntries();
			BoundingBox ENNnewBounds = smallestBoundingBox(NN_Entries);

			Entry* ENN = new Entry(ENNnewBounds, NN);
			if (P->getEntries().size() + 1 <= maxim){
				//cout << "enough space... add new node to P " << ENN << endl;
				P->addEntry(ENN);
				adjustTree(P);
			}
			else{
				P->addEntry(ENN);
				std::vector<Node*> newP = splitNode(P);
				adjustTree(newP);
			}

		}
	}
}
void RTree::deleteEntry(Entry* e){
	Node* L = findLeaf(e);
	if (L != NULL){
		std::vector<Entry*> entries = L->getEntries();
		int pos = std::find(entries.begin(), entries.end(), e) - entries.begin();
		entries.erase(entries.begin() + pos);
		// entries.erase(std::find(entries.begin(), entries.end(), e) + entries.begin());
		condenseTree(L);
		//if root node has only one child, make child the new root
		if (root->getEntries().size() == 1){
			Node* newRoot = root->get_ithEntry(0)->getChildPointer();
			root = newRoot;
		}
	}
}

Node* RTree::findLeaf(Entry* e){
	BoundingBox EI = e->getI();
	Node* T = root;
	if (T->isLeaf()){
		for (int i = 0; i<T->getEntries().size(); i++){
			Entry* current = T->get_ithEntry(i);
			if (e->equal(current)){
				return T;
			}
		}
		return NULL;
	}
	else{
		for (int i = 0; i<T->getEntries().size(); i++){
			Entry* current = T->get_ithEntry(i);
			BoundingBox currentI = current->getI();
			if (EI.overlaps(currentI)){
				Node* newRoot = current->getChildPointer();
				RTree* newSubTree = new RTree(num_dimen, newRoot, maxim, minim);
				Node* F = newSubTree->findLeaf(e);
				if (F != NULL){ return F; }
			}

		}
		return NULL;
	}
}

void RTree::condenseTree(Node* L){
	Node* N = L;
	std::vector<Node*> Q;
	if (!root->equal(N)){
		Node* P = N->getParent();
		Entry* EN = NULL;
		for (int i = 0; i<P->getEntries().size(); i++){
			Entry* current = P->get_ithEntry(i);
			Node* currentPointer = current->getChildPointer();
			if (currentPointer->equal(N)){
				EN = current;
			}
		}
		if (N->getEntries().size()<minim){
			P->removeEntry(EN);
			Q.push_back(N);
		}
		else{
			BoundingBox newBounds = smallestBoundingBox(N->getEntries());
			EN->setI(newBounds);
		}
		N = P;
		condenseTree(N);
	}
	// TODO: re-insert nodes in Q. Entries from eliminated leaf nodes are re-inserted
	// in tree leaves as described in Insert, but entries from higher-level nodes must be placed
	// higher in the tree so that leaves of their dependent subtrees will be on the same level
	// as leaves of the main tree.
	//
}

std::vector<Node*> RTree::splitNode(Node* n){
	//cout << "SPLITNODE START" << endl;
	lastSplit = n;
	rootSplit = false;
	std::vector<Entry*> seeds = pickSeeds(n);
	//cout << "SPLITNODE 1" << endl;
	Entry* leftSeed = seeds.at(0);
	Entry* rightSeed = seeds.at(1);
	std::vector<Entry*> left;
	left.push_back(leftSeed);
	std::vector<Entry*> right;
	right.push_back(rightSeed);
	std::vector<Entry*> remaining = n->getEntries();
	//cout << "SPLITNODE 5" << endl;
	int pos = std::find(remaining.begin(), remaining.end(), rightSeed) - remaining.begin();
	remaining.erase(remaining.begin() + pos);
	int lpos = std::find(remaining.begin(), remaining.end(), leftSeed) - remaining.begin();
	remaining.erase(remaining.begin() + lpos);
	//cout << "SPLITNODE 8" << endl;
	while (!remaining.empty()){
		Entry* next = pickNext(right, left, remaining);

		// need to check if one node is getting too many, cant have a node with minimum...
		if (remaining.size() + right.size() == minim) {
			// if we have to put everything remaining in right to meet the min size
			//cout << "FORCE A PUSH TO RIGHT " << next->getI().get_ithLower(0) << next->getI().get_ithLower(1) << endl;
			right.push_back(next);
		}
		else if (remaining.size() + left.size() == minim) {
			// if we have to put everything remaining in left to meet the min size
			//cout << "FORCE A PUSH TO LEFT " << next->getI().get_ithLower(0) << next->getI().get_ithLower(1) << endl;
			left.push_back(next);
		}
		else {
			// normal case
			BoundingBox rightBB = smallestBoundingBox(right);
			BoundingBox leftBB = smallestBoundingBox(left);
			double areaIncrRight = rightBB.areaToAdd(next->getI());
			double areaIncrLeft = leftBB.areaToAdd(next->getI());
			if (areaIncrRight < areaIncrLeft){
				//cout << "PUSH TO RIGHT " << next->getI().get_ithLower(0) << next->getI().get_ithLower(1) << endl;
				right.push_back(next);
			}
			else{
				//cout << "PUSH TO LEFT " << next->getI().get_ithLower(0) << next->getI().get_ithLower(1) << endl;
				left.push_back(next);
			}
		}
		int npos = std::find(remaining.begin(), remaining.end(), next) - remaining.begin();
		remaining.erase(remaining.begin() + npos);

	}
	//cout << "SPLITNODE: finished splitting into 'left' and 'right' " << endl;
	Node* parent = n->getParent();
	Node* newRight = new Node(num_dimen, right, parent, maxim, minim, n->isLeaf());
	Node* newLeft = new Node(num_dimen, left, parent, maxim, minim, n->isLeaf());
	if (parent == NULL){//TODO NEW CODE STARTS
		//cout << "we split the root, special case..." << endl;
		rootSplit = true;
		Entry* rightP = new Entry(smallestBoundingBox(right), newRight);
		Entry* leftP = new Entry(smallestBoundingBox(left), newLeft);
		std::vector<Entry*> Elist;
		Elist.push_back(rightP);
		Elist.push_back(leftP);
		Node* newParent = new Node(num_dimen, Elist, maxim, minim, false);
		newRight->setParent(newParent);
		newLeft->setParent(newParent);
		newParent->setIsRoot(true);
		setRoot(newParent);
		//cout << "created and set a new Root.." << newParent << endl;
		newRight->setIsRoot(false);
		newLeft->setIsRoot(false);
		//cout << "ROOT IS " << root << " OF SIZE " << root->getEntries().size() << endl;
		//cout << "SPLIT NODE RIGHTP: " << root->getEntries().at(0) << " " << root->getEntries().at(0)->getInLeaf();
		//cout << "SPLIT NODE LEFTP: " << root->getEntries().at(1) << " " << root->getEntries().at(1)->getInLeaf();
	}
	else {
		// make sure parent isn't too full for another entry?
		// then split with a new setup at current location versus placing in parent?
		/*Entry* rightP = new Entry(smallestBoundingBox(right), newRight);
		Entry* leftP = new Entry(smallestBoundingBox(left), newLeft);
		std::vector<Entry*> Elist;
		Elist.push_back(rightP);
		Elist.push_back(leftP);
		newRight->setParent(n);
		newLeft->setParent(n);
		n->getEntries().clear();
		n->getEntries().push_back(leftP);
		n->getEntries().push_back(rightP);
		cout << "NONROOT SPLIT NODE RIGHTP: " << n->getEntries().at(0) << " " << n->getEntries().at(0)->getInLeaf();
		//cout << "NONROOT SPLIT NODE LEFTP: " << n->getEntries().at(1) << " " << n->getEntries().at(1)->getInLeaf();*/
	}
	//cout << "NEWRIGHT: " << newRight << " AND NEW LEFT: " << newLeft << endl;
	std::vector<Node*> result = { newRight, newLeft };

	for (Entry * entry : newRight->getEntries()) {
		if (Node *node = entry->getChildPointer())
			node->setParent(newRight);
	}
	for (Entry * entry : newLeft->getEntries()) {
		if (Node * node = entry->getChildPointer())
			node->setParent(newLeft);
	}

	return result;
}

std::vector<Entry*> RTree::pickSeeds(Node* n){// Linear time version!
	//cout << "PICKSEEDS START" << endl;
	std::vector<Entry*> entries = n->getEntries();
	std::vector<double> dimenWidths;
	std::vector<double> biggestSeps; //biggest separations along each dimension
	std::vector<Entry*> lowestEntries;// entries that had the lowest highside along each dimension
	std::vector<Entry*> highestEntries;// entries that had the highest lowside along each dimension
	std::vector<double> normalizedSeps;
	//cout << "PICKSEEDS LOOP BEGIN" << endl;
	for (int i = 0; i<num_dimen; i++){
		//cout << "PICKSEEDS LOOP RAN " << i << endl;
		int indexOfHighestLow;
		int indexOfLowestHigh;
		std::vector<double> dimLows; //a list of all the lows along this dimension from each entry
		std::vector<double> dimHighs; // a list of all the highs ...
		for (int j = 0; j<entries.size(); j++){
			Entry* e = entries.at(j);
			BoundingBox ei = e->getI();
			dimLows.push_back(ei.get_ithLower(i));
			dimHighs.push_back(ei.get_ithUpper(i));
		}
		double highestLow = *max_element(dimLows.begin(), dimLows.end());
		double lowestHigh = *min_element(dimHighs.begin(), dimHighs.end());
		indexOfHighestLow = find(dimLows.begin(), dimLows.end(), highestLow) - dimLows.begin();
		indexOfLowestHigh = find(dimHighs.begin(), dimHighs.end(), lowestHigh) - dimHighs.begin();
		//cout << "IOHL is " << indexOfHighestLow << endl;
		//cout << "IOLH is " << indexOfLowestHigh << endl;
		dimenWidths.push_back(abs(*max_element(dimHighs.begin(), dimHighs.end()) - *min_element(dimLows.begin(), dimLows.end())));
		biggestSeps.push_back(highestLow - lowestHigh);
		//cout << "LOWESTENTRIES GETS " << entries.at(indexOfLowestHigh) << endl;
		lowestEntries.push_back(entries.at(indexOfLowestHigh));
		highestEntries.push_back(entries.at(indexOfHighestLow));
		//cout << "LOWESTENTRIES AT 0 " << lowestEntries.at(0) << endl;
	}
	//cout << "PICKSEEDS LOOP1 END" << endl;
	for (int i = 0; i<num_dimen; i++){
		double temp = biggestSeps[i] / dimenWidths[i];
		normalizedSeps.push_back(temp);
	}
	//cout << "PICKSEEDS LOOP2 END" << endl;
	std::vector<Entry*> bestSeeds;
	double d = *max_element(normalizedSeps.begin(), normalizedSeps.end());
	//cout << "PICKSEEDS FIND" << endl;
	int indx = std::find(normalizedSeps.begin(), normalizedSeps.end(), d) - normalizedSeps.begin();
	//cout << "PICKSEEDS FIND END INDEX IS " << indx << endl;
	bestSeeds.push_back(lowestEntries.at(indx));
	//cout << "GOT LOWEST FOR INDX " << indx << endl;
	bestSeeds.push_back(highestEntries.at(indx));
	//cout << "PICKSEEDS BESTSEEDS SET" << endl;
	return bestSeeds;
}

Entry* RTree::pickNext(std::vector<Entry*> right, std::vector<Entry*> left, std::vector<Entry*> remaining){// Quadratic time version! I felt the linear one was kinda silly.
	Entry* critical = remaining[0];
	double biggestDiff = 0;
	for (Entry* e : remaining){
		BoundingBox initRightBox = smallestBoundingBox(right);
		BoundingBox initLeftBox = smallestBoundingBox(left);
		double areaIncrRight = initRightBox.areaToAdd(e->getI());
		double areaIncrLeft = initLeftBox.areaToAdd(e->getI());
		if (abs(areaIncrRight - areaIncrLeft)>biggestDiff){
			critical = e;
		}
	}

	return critical;
}


BoundingBox RTree::smallestBoundingBox(std::vector<Entry*> entries){
	std::vector<double> Lower;
	std::vector<double> Upper;
	for (int i = 0; i< entries.front()->getNumDimen(); i++){
		std::vector<double> allLow;
		std::vector<double> allHigh;
		for (int j = 0; j< entries.size(); j++){
			Entry* current = entries[j];
			BoundingBox b = current->getI();
			allLow.push_back(b.get_ithLower(i));
			allHigh.push_back(b.get_ithUpper(i));
		}
		double newLow = *std::min_element(allLow.begin(), allLow.end());
		double newHigh = *std::max_element(allHigh.begin(), allHigh.end());
		Lower.insert(Lower.begin() + i, newLow);
		Upper.insert(Upper.begin() + i, newHigh);
	}
	BoundingBox result = BoundingBox(Lower, Upper);
	return result;
}
