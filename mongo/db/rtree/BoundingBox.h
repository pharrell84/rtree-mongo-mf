/*
 * BoundingBox.h
 *
 *  Created on: Apr 17, 2015
 *      Author: assma
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_
#include <vector>

class BoundingBox
{
private:
	int num_dimen;
	std::vector<double> lowerLimits;
	std::vector<double> upperLimits;

public:
	BoundingBox();
	BoundingBox(std::vector<double> l, std::vector<double> u);
	int getNumDimen(void);
	std::vector<double> getLower(void);
	std::vector<double> getUpper(void);
	double get_ithLower(int i) const;
	double get_ithUpper(int i) const;
	void set_ithLower(int i, double l) ;
	void set_ithUpper(int i, double p) ;
	void set_Lower(std::vector<double> lower);
	void set_Upper(std::vector<double> upper);
	double calcArea(void);
	bool equal(BoundingBox other) const;
	bool overlaps(BoundingBox other);
	bool includes(BoundingBox other);
	double areaToAdd(BoundingBox other);

};


#endif /* BOUNDINGBOX_H_ */
