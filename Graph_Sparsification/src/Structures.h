/*
 * Structures.h
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_
#include <bits/stdc++.h>
using namespace std;

struct Edge {
	int u, v;
	int w;

	Edge(int _u, int _v, int _w) {
		u = _u;
		v = _v;
		w = _w;
	}

};

struct Sort_Element {
	int index;
	int value;

	Sort_Element() {
		index = value = 0;
	}

	Sort_Element(int _i, int _v) {
		index = _i;
		value = _v;
	}

	bool operator < (const Sort_Element &a ) const {
		return this->value > a.value;
	}


};


typedef pair<int, int> PII;
typedef vector<Edge> EdgeGraph;
typedef vector<vector<int> > AdjLinkGraph;


#endif /* STRUCTURES_H_ */
