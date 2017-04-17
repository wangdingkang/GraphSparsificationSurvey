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

	InputGraph(){}
	InputGraph(string filename);

	AdjLinkGraph graph; // edges in the original graph.

	void show() {
		cout << N << " nodes, and " << M << " edges." << endl;
	}

	int num_nodes();

	virtual ~InputGraph();

	vector<int> degree_random_sample(int S);

	vector<double> sp_distribution(unordered_set<int> &indexes, int cut_off);

	vector<double> sp_distribution(vector<int> &indexes, int cut_off);

	void sp_bfs(vector<double> &rets, unordered_set<int> &indexes, int p, int cut_off);

private:

	string filename;

	int N, M; // #nodes, #edges of the original graph.

	int cnt_index = 0;

	void read_in_graph(); // read input file, starting with #nodes, and #edges, following by m edges u --> v, u, v are the indexes of endpoints.

	vector<string> split(string s); // check if the input graph is weighted or not.

};

#endif /* SAMPLING_H_ */
