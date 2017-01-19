//============================================================================
// Name        : Random_Graph_Gen.cpp
// Author      : wdk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
using namespace std;

#define N 100
#define SIGMA 1	// sigma of gaussian kernel
#define EPSILON 1e-6

#define OUTPUT string("output/")
#define POSITIONS string("sphere_node")
#define GRAPH string("sphere_graph")
#define SUFFIX string(".txt")

struct Node {
	double x, y, z;
	Node(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	double cal_weight(const Node& another) const {
		double temp = 0;
		double dx = x - another.x;
		double dy = y - another.y;
		double dz = z - another.z;
		temp = dx * dx + dy * dy + dz * dz;
		return exp(-temp / (2 * SIGMA * SIGMA));
	}

};

vector<Node> graph;

// generate nodes on radius-1 sphere.
void rand_sphere() {
	srand(time(NULL));
	for (int i = 0; i < N; i++) {
		double u, v;
		do {
			u = 2.0f * rand() / RAND_MAX - 1;
			v = 2.0f * rand() / RAND_MAX - 1;
		} while (u * u + v * v > 1 - EPSILON);
		double ss = u * u + v * v;
		double x = 2 * u * sqrt(1 - ss);
		double y = 2 * v * sqrt(1 - ss);
		double z = 1 - 2 * ss;
		graph.push_back(Node(x, y, z));
	}
}

void cal_weight_and_output() {
	ofstream fout;
	fout.open(OUTPUT + POSITIONS + SUFFIX);
	for (auto& node : graph) {
		fout << node.x << " " << node.y << " " << node.z << endl;
	}
	fout.close();
	fout.open(OUTPUT + GRAPH + SUFFIX);
	fout << N << endl;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			fout << i << " " << j << " " << graph[i].cal_weight(graph[j])
					<< endl;
		}
	}
	fout.close();
}

int main() {

	rand_sphere();
	cal_weight_and_output();

	return 0;
}
