/*
 * Sampling.h
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#ifndef SAMPLING_H_
#define SAMPLING_H_
#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> PII;

class InputGraph {
public:

	InputGraph(string filename);

	virtual ~InputGraph();

private:

	string filename;

	int N, M; // #nodes, #edges of the original graph.

	vector<vector<int>> paths; // edges in the original graph.

	int cnt_index = 0;

	void read_in_graph(); // read input file, starting with #nodes, and #edges, following by m edges u --> v, u, v are the indexes of endpoints.


};

#endif /* SAMPLING_H_ */
