//============================================================================
// Name        : Random_Graph_Gen.cpp
// Author      : wdk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
#include "Node.h"
using namespace std;

#define N 100
//#define SIGMA 1	// sigma of gaussian kernel
#define EPSILON 1e-6

#define THRESHOLD 0.1 // normalized by one.

#define OUTPUT string("output/")
#define POSITIONS string("sphere_node")
#define GRAPH string("sphere_graph")
#define SUFFIX string(".txt")


struct Edge {
	int u, v;
	double w;

	Edge(int _u, int _v, double _w) {
		u = _u;
		v = _v;
		w = _w;
	}
};

vector<Edge> edges;

vector<Node> rand_line(const int n, const Node& a, const Node& b) {
	vector<Node> nodes(n);
	srand(time(NULL));

	Node d = (b - a);
	for (int i = 0; i < n; i++) {
		double temp = 1.0f * rand() / RAND_MAX;
		nodes[i] = (temp * d + a);
	}
	return nodes;
}

vector<Node> even_line(const int n, const Node& a, const Node& b) {
	vector<Node> nodes(n);

	Node d = (b - a) / (n + 1);
	for (int i = 1; i < n; i++) {
		nodes[i - 1] = (a + d * i);
	}

	return nodes;
}

/*
 * generate n nodes on sphere on the surface of a sphere with radius r and center c.
 * @param n number of nodes to be sampled
 * @param c center of the ball
 * @param r radius of the ball
 * @return the list of sampled nodes
 */
vector<Node> rand_sphere(const int n, const Node& c, const double r) {
	srand(time(NULL));
	vector<Node> nodes(n);
	for (int i = 0; i < n; i++) {
		double u, v;
		do {
			u = 2.0f * rand() / RAND_MAX - 1;
			v = 2.0f * rand() / RAND_MAX - 1;
		} while (u * u + v * v > 1 - EPSILON);
		double ss = u * u + v * v;
		double x = 2 * u * sqrt(1 - ss);
		double y = 2 * v * sqrt(1 - ss);
		double z = 1 - 2 * ss;
		nodes[i] = Node(x * r + c.x, y * r + c.y, z * r + c.z);
	}
	return nodes;
}

/*
 * generate a dumbbell shape model with two balls linked by a rod(line).
 * @param sn number of sampled nodes on each ball
 * @param ln number of sampeld nodes on the rod, (sampled evenly)
 * @param c1 the center of one ball
 * @param c2 the center of the other ball
 * @param r radius of the balls
 * @return a list of nodes generated
 */
vector<Node> rand_dumbbell(const int sn, const int ln, const Node& c1,
		const Node& c2, const double r) {
	vector<Node> ret;
	vector<Node> first_ball = rand_sphere(sn, c1, r);
	vector<Node> second_ball = rand_sphere(sn, c2, r);
	return ret;
}

void generate_proximity_by_threshold() {

}

int main() {

	vector<Node> nodes = rand_sphere(N, Node(0, 0, 0), 0.5f);


	return 0;
}
