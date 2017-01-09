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

#define GRAPH_SIZE 20000
#define INDEX_LIMIT 200000

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


	/*
	int get_real_index(int sudo_index);
	int cnt_temp_max = 0; // record the temp largest connected component
	vector<int> temp_comp_set; // record ids of the vertices in the component
	int max_comp_size = 0; // size of the largest component.

	void init_lcc();
	void dfs_search(int s); // find out the connected component
	 */

};

#endif /* SAMPLING_H_ */
