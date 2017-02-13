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
	double w;

	Edge(int _u, int _v, double _w) {
		u = _u;
		v = _v;
		w = _w;
	}

};

struct Sort_Element {
	int index;
	double value;

	Sort_Element() {
		index = value = 0;
	}

	Sort_Element(int _i, double _v) {
		index = _i;
		value = _v;
	}

	bool operator <(const Sort_Element &a) const {
		return this->value > a.value;
	}

};

struct Edge_Out {
	int v;
	double w;
	Edge_Out(int _v, double _w) {
		v = _v;
		w = _w;
	}
};

struct AdjLinkGraph {

	bool weighted;

	vector<double> degrees;

	vector<vector<Edge_Out>> adjlink;

	AdjLinkGraph(){weighted = false;}

	AdjLinkGraph(bool _weighted, int N) {
		weighted = _weighted;
		adjlink = vector<vector<Edge_Out>>(N, vector<Edge_Out>());
		degrees = vector<double>(N);
		for (int i = 0; i < N; i++) {
			degrees[i] = 0.0;
		}
	}

	void init_degree() {
		for (int i = 0; i < (int)adjlink.size(); i++) {
			for(const auto& t : adjlink[i]) {
				degrees[i] += t.w;
			}
		}
	}

	int get_num_of_nodes() const {
		return adjlink.size();
	}

	int get_num_edge_of_node(int u) const {
		return adjlink[u].size();
	}

	// u: index of node, i: index of its neighbor
	// reurn index of its neighbor
	int get_neighbor_of_node(int u, int i) const {
		return adjlink[u][i].v;
	}

	// get degree of nodes, different from get_num_edge, since each edge may have different weights.
	double get_degree_of_node(int u) const {
		return degrees[u];
	}

	// get the weight of ith edges of node u
	double get_weight(int u, int i) const {
		return adjlink[u][i].w;
	}

	// For weighted graph.
	void insert_link(int u, int v, double w) {
		adjlink[u].push_back(Edge_Out(v, w));
	}

	// For unweighted graph.
	void insert_link(int u, int v) {
		adjlink[u].push_back(Edge_Out(v, 1.0));
	}

};

typedef pair<int, int> PII;
typedef vector<Edge> EdgeGraph;

#endif /* STRUCTURES_H_ */
