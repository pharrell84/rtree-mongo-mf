/*
 * BoundingBox.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: assma
 */
#include <iostream>
#include <cmath>
#include <vector>
#include "BoundingBox.h"

BoundingBox::BoundingBox() {}
BoundingBox::BoundingBox(std::vector<double> l, std::vector<double> u)
{
	lowerLimits = l;
	upperLimits = u;
	num_dimen = l.size();
}
int BoundingBox::getNumDimen(void)
{
	return num_dimen;
}
std::vector<double> BoundingBox::getLower(void)
{
	return lowerLimits;
}
std::vector<double> BoundingBox::getUpper(void)
{
	return upperLimits;
}
double BoundingBox::get_ithLower(int i) const
{
	return lowerLimits[i];
}
double BoundingBox::get_ithUpper(int i) const
{
	return upperLimits[i];
}
void BoundingBox::set_ithLower(int i, double l)
{
	lowerLimits[i] = l;
}
void BoundingBox::set_ithUpper(int i, double p)
{
		upperLimits[i] = p;
}
void BoundingBox::set_Lower(std::vector<double> lower)
{
	lowerLimits = lower;
}
void BoundingBox::set_Upper(std::vector<double> upper)
{
	upperLimits = upper;
}
double BoundingBox::calcArea(void)
{
	double area = 1;
	for (int i=0; i<num_dimen; i++){
				double diff = get_ithUpper(i)-get_ithLower(i);
				area = area*diff;
			}
			return area;
}
bool BoundingBox::equal(BoundingBox other) const {
	for(int i=0; i<num_dimen; i++){
		if(get_ithLower(i)!= other.get_ithLower(i) || get_ithUpper(i)!= other.get_ithUpper(i)){
			return false;
		}
	}
	return true;
}
bool BoundingBox::overlaps(BoundingBox other){
	if(num_dimen != other.getNumDimen()){return false;}
	for(int i=0; i<num_dimen; i++){
		double l = get_ithLower(i);
		double u = get_ithUpper(i);
		double otherL = other.get_ithLower(i);
		double otherU = other.get_ithUpper(i);

		if(!(u>otherL && otherU>l)){return false;}
	}
	return true;
}

bool BoundingBox::includes(BoundingBox other){
	if(num_dimen != other.getNumDimen()){return false;}
	for(int i=0; i<num_dimen; i++){
		double l = get_ithLower(i);
		double u = get_ithUpper(i);
		double otherL = other.get_ithLower(i);
		double otherU = other.get_ithUpper(i);

		if(u<otherU || l>otherL){return false;}
	}
	return true;
}

double BoundingBox::areaToAdd(BoundingBox other){
	if (includes(other)){return 0;}
	double extra = 1;
	for(int i=0; i<num_dimen; i++){
		double l = get_ithLower(i);
		double u = get_ithUpper(i);
		double otherL = other.get_ithLower(i);
		double otherU = other.get_ithUpper(i);
		double diff=0;
		if(u<otherU){
			diff += std::abs(otherU-u);
		}
		if(l>otherL){
			diff += std::abs(l-otherL);
		}
		if(diff !=0){
			extra = extra*diff;
		}
	}
	return extra;
}








