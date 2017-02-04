//============================================================================
// Name        : Random_Graph_Gen.cpp
// Author      : wdk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
#include <dirent.h>
#include <direct.h>
#include "Node.h"
using namespace std;

#define N 1000
//#define SIGMA 1	// sigma of gaussian kernel
#define EPSILON 1e-6

#define THRESHOLD 0.035 // normalized by one.

#define K_NEIGHBOR 3 // number of neighbors

#define COLOR_SPHERE "#0000ff" // color for ball
#define COLOR_LINE "#ff0000"  // color for line

#define OUTPUT string("output/")
#define INPUT string("models/")
#define NODES string("node_dumb_graph")
#define GRAPH string("dumb_graph")
#define NODES_GEPHI string("node_dumb_graph_gephi")
#define GRAPH_GEPHI string("dumb_graph_gephi")
#define SUFFIX string(".txt")
#define SUFFIX_GEPHI string(".csv")

struct Edge {
	int u, v;
	double w;
	Edge(int _u, int _v, double _w) {
		u = _u;
		v = _v;
		w = _w;
	}
};
typedef vector<Edge> Graph;

/*
 * generate n nodes on the line randomly
 * @param n number of nodes to be sampled
 * @param a one endpoint
 * @param b the other endpoint
 * @return the list of sampled nodes
 */
vector<Node> rand_line(const int n, const Node& a, const Node& b) {
	vector<Node> nodes(n);
	srand(time(NULL));

	Node d = (b - a);
	for (int i = 0; i < n; i++) {
		double temp = 1.0f * rand() / RAND_MAX;
		nodes[i] = (temp * d + a);
		nodes[i].color = COLOR_LINE;
	}
	return nodes;
}

/*
 * generate n nodes evenly along the line
 * @param n number of nodes to be sampled
 * @param a one endpoint
 * @param b the other endpoint
 * @return the list of sampled nodes
 */
vector<Node> even_line(const int n, const Node& a, const Node& b) {
	vector<Node> nodes(n);

	Node d = (b - a) / (n + 1);
	for (int i = 1; i <= n; i++) {
		nodes[i - 1] = (a + d * i);
		nodes[i - 1].color = COLOR_LINE;
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
		nodes[i] = Node(x * r + c.x, y * r + c.y, z * r + c.z, COLOR_SPHERE);
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
	Node d = c2 - c1;
	double norm = d.norm2();
	d = r * d / norm;
	vector<Node> rod = even_line(ln, c1 + d, c2 - d);
	ret.reserve(sn * 2 + ln);
	ret.insert(ret.end(), first_ball.begin(), first_ball.end());
	ret.insert(ret.end(), second_ball.begin(), second_ball.end());
	ret.insert(ret.end(), rod.begin(), rod.end());
	return ret;
}

Graph generate_proximity_by_threshold(const vector<Node>& nodes) {
	Graph g;
	for (int i = 0; i < (int) nodes.size(); i++) {
		for (int j = i + 1; j < (int) nodes.size(); j++) {
			double dist = nodes[i].cal_dist(nodes[j]);
			if (dist < THRESHOLD) {
				g.push_back(Edge(i, j, 1));
			}
		}
	}
	return g;
}

Graph generate_proximity_by_k_nearest(const vector<Node>& nodes) {
	cout << "Generating K-nearest neighbor graph with K = " << K_NEIGHBOR << "."
			<< endl;
	Graph g;
	set<pair<int, int> > my_set;
	for (int i = 0; i < (int) nodes.size(); i++) {
		priority_queue<pair<double, int>, std::vector<pair<double, int> > > q_min;
		for (int j = 0; j < (int) nodes.size(); j++) {
			if (i != j) {
				q_min.push(make_pair(nodes[i].cal_dist(nodes[j]), j));
			}
			if (q_min.size() > K_NEIGHBOR) {
				q_min.pop();
			}
		}
		while (!q_min.empty()) {
			pair<double, int> t = q_min.top();
			int u = min(i, t.second);
			int v = max(i, t.second);
			pair<int, int> p = make_pair(u, v);
			if (my_set.find(p) == my_set.end()) {
				my_set.insert(p);
				g.push_back(Edge(u, v, 1.0));
			}
			q_min.pop();
		}
	}
	return g;
}

void output_nodes(vector<Node>& nodes, string filename) {
	cout << nodes.size() << " nodes." << endl;
	ofstream fout;
	fout.open(filename.c_str());
	for (int i = 0; i < (int) nodes.size(); i++) {
		fout << i << " " << nodes[i].x << " " << nodes[i].y << " " << nodes[i].z
				<< endl;
	}
	fout.close();
}

void output_proximity_graph(Graph& g, string filename) {
	cout << g.size() << " edges." << endl;
	ofstream fout;
	fout.open(filename.c_str());
	for (int i = 0; i < (int) g.size(); i++) {
		fout << g[i].u << " " << g[i].v << " " << g[i].w << endl;
	}
	fout.close();
}

void output_nodes_for_gephi(vector<Node>& nodes, string filename) {
	ofstream fout;
	fout.open(filename.c_str());
	fout << "Id;Color" << endl;
	for (int i = 0; i < (int) nodes.size(); i++) {
		fout << i << ";" << nodes[i].color << endl;
	}
	fout.close();
}

void output_proximity_graph_for_gephi(Graph &g, string filename) {
	ofstream fout;
	fout.open(filename.c_str());
	fout << "Source;Target;Weight;Type" << endl;
	for (int i = 0; i < (int) g.size(); i++) {
		fout << g[i].u << ";" << g[i].v << ";" << g[i].w << ";Undirected"
				<< endl;
	}
	fout.close();
}

vector<string> input_models(string dir) {
	DIR *dp;
	struct dirent *dirp;
	vector<string> rets;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cerr << "Error opening " << dir << endl;
		return rets;
	}

	while ((dirp = readdir(dp)) != NULL) {
		char *filename = dirp->d_name;
		int len = strlen(filename);
		if (len > 4 && strcmp(filename + len - 4, ".txt") == 0)
			rets.push_back(string(filename));
	}
	closedir(dp);
	return rets;
}

vector<Node> read_file(string filename) {
	cout << "Reading " + filename + "." << endl;
	vector<Node> rets;
	ifstream fin;
	fin.open(INPUT + filename);
	int dim, n;
	fin >> dim >> n;
	double x, y, z;
	while (fin >> x >> y >> z) {
		rets.push_back(Node(x, y, z));
	}
	fin.close();
	return rets;
}

int main() {

//	when generating models, you should manually control the diameter of the model to be exactly one;
//	vector<Node> nodes = rand_dumbbell(500, 50, Node(-1.0 / 3, 0, 0),
//			Node(1.0 / 3, 0, 0), 1.0 / 6);
//	vector<Node> nodes;
//
//	Graph g = generate_proximity_by_k_nearest(nodes);
//	output_nodes_for_gephi(nodes, OUTPUT + NODES_GEPHI + SUFFIX_GEPHI);
//	output_nodes(nodes, OUTPUT + NODES + SUFFIX);
//	output_proximity_graph(g, OUTPUT + GRAPH + SUFFIX);
//	output_proximity_graph_for_gephi(g,
//	OUTPUT + GRAPH_GEPHI + SUFFIX_GEPHI);

	vector<string> filenames = input_models(INPUT);
	for (auto &filename : filenames) {
		vector<Node> nodes = read_file(filename);
		Graph g = generate_proximity_by_threshold(nodes);
		string dir = OUTPUT + filename.substr(0, filename.length() - 4);
		mkdir(dir.c_str());
		output_nodes(nodes, dir + "/nodes_" + filename);
		output_proximity_graph(g, dir + "/" + filename);
	}

	return 0;
}
