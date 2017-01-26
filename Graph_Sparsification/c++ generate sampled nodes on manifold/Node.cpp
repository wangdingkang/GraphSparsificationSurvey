/*
 * Node.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: dkwan
 */

#include "Node.h"

Node::Node() {
	x = y = z = 0;
	color = "";
}

Node::Node(double _x, double _y, double _z, string _color /*="null"*/) {
	x = _x;
	y = _y;
	z = _z;
	color = _color;
}

double Node::cal_dist(const Node& another) const {
	double temp = 0;
	double dx = x - another.x;
	double dy = y - another.y;
	double dz = z - another.z;
	temp = dx * dx + dy * dy + dz * dz;
	return std::sqrt(temp);
}

double Node::norm2() const{
	return std::sqrt(x * x + y * y + z * z);
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

Node& operator+=(Node& lhs, const Node& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

Node& operator-=(Node& lhs, const Node& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

Node operator+(const Node& lhs, const Node& rhs) {
	return Node(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Node operator-(const Node& lhs, const Node& rhs) {
	return Node(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Node operator*(double lhs, const Node& rhs) {
	return Node(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}
Node operator*(const Node& lhs, double rhs) {
	return Node(rhs * lhs.x, rhs * lhs.y, rhs * lhs.z);
}

Node operator/(const Node& lhs, double rhs) {
	return Node(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}
