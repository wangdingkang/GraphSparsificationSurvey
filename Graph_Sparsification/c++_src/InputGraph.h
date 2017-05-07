/*
 * Sampling.h
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#ifndef SAMPLING_H_
#define SAMPLING_H_
#include "Structures.h"
using namespace std;

typedef pair<int, int> PII;

class InputGraph {
public:

	InputGraph(){ N = M = 0;}
	InputGraph(string filename);

	AdjLinkGraph graph; // edges stored in adjlink table of the original graph.

	void show() {
		cout << N << " nodes, and " << M << " edges." << endl;
	}

	// return # of nodes.
	int num_nodes();

	virtual ~InputGraph();

	// uniform random sampling, different nodes share the same probability to be chose.
	vector<int> uniform_random_sample(int S) const;

	// random sampling, nodes with higher degree have higher probability to be chosen.
	vector<int> degree_random_sample(int S) const;

	// calculate all pair shortest distances for chosen landmarks
	vector<double> sp_distribution(unordered_set<int> &indexes, int cut_off);
	vector<double> sp_distribution(vector<int> &indexes, int cut_off);

	// run bfs search for calculating shortest path distances, since all input(original) networks are unweighted.
	void sp_bfs(vector<double> &rets, unordered_set<int> &indexes, int p, int cut_off);

private:

	// filename of input graph.
	string filename;

	// # of nodes, edges of the original graph.
	int N, M;

	// read input file, starting with #nodes, and #edges, following by m edges u --> v, u, v are the indexes of endpoints.
	void read_in_graph();

	// check if the input graph is weighted or not, (one line has 3 or 2 elements).
	vector<string> split(string s);

};

#endif /* SAMPLING_H_ */
