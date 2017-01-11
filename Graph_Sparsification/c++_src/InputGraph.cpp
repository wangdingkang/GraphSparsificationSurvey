/*
 * Sampling.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "InputGraph.h"


InputGraph::InputGraph(string filename) {
	N = 0;
	this->filename = filename;
	read_in_graph();
}

AdjLinkGraph InputGraph::get_paths() {
	return this->paths;
}

void InputGraph::read_in_graph() {
	ifstream fin;
	fin.open(filename);
	fin >> N;
	paths = vector<vector<int> >(N, vector<int>());
	int a, b;
	while(fin >> a >> b){
		paths[a].push_back(b);
		paths[b].push_back(a);
	}
	fin.close();
}


InputGraph::~InputGraph() {
	// TODO Auto-generated destructor stub
}

