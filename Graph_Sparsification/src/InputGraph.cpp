/*
 * Sampling.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "InputGraph.h"


InputGraph:: InputGraph(string filename) {
	N = M = 0;
	this->filename = filename;
	read_in_graph();
}

AdjLinkGraph InputGraph::get_paths() {
	return this->paths;
}

void InputGraph::read_in_graph() {
	ifstream fin;
	fin.open(filename);
	fin >> N >> M;
	paths = vector<vector<int> >(N, vector<int>());
	int a, b;
	for (int i = 0; i < M; i++) {
		fin >> a >> b;
		paths[a].push_back(b);
		/*
		 * duplicate edges?/??? see the input file to decide.
		 *
		 *
		 */
		// paths[b].push_back(a);
	}
	fin.close();
}


InputGraph::~InputGraph() {
	// TODO Auto-generated destructor stub
}

