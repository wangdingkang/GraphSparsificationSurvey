/*
 * Node.h
 *
 *  Created on: Jan 24, 2017
 *      Author: dkwan
 */

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	double x, y, z;
	Node(double _x, double _y, double _z);
	virtual ~Node();

	double cal_dist(const Node& another) const;

//	double norm2() {
//		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
//	}

};

Node& operator+=(Node& lhs, const Node& rhs);
Node& operator-=(Node& lhs, const Node& rhs);

Node operator+(const Node& lhs, const Node& rhs);

Node operator-(const Node& lhs, const Node& rhs);

Node operator*(double lhs, const Node& rhs);
Node operator*(const Node& lhs, double rhs);

Node operator/(const Node& lhs, double rhs);

#endif /* NODE_H_ */
