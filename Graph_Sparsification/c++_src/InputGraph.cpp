/*
 * Sampling.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "InputGraph.h"

InputGraph::InputGraph(string filename) {
	N = M = 0;
	this->filename = filename;
	read_in_graph();
}

int InputGraph::num_nodes() {
	return this->N;
}

AdjLinkGraph InputGraph::get_graph() {
	return this->graph;
}

vector<string> InputGraph::split(string s) {
	std::istringstream iss(s);
	vector<string> ret;
	do {
		std::string sub;
		iss >> sub;
		ret.push_back(sub);
	} while (iss);
	return ret;
}

void InputGraph::read_in_graph() {
	ifstream fin;
	fin.open(filename);
	fin >> N;
	graph = AdjLinkGraph(false, N);
	int u, v;
	double w;
	string temp;
	vector<string> t;
	std::getline(fin, temp);
	std::getline(fin, temp);

	t = split(temp);
//	for(auto &s : t) {
//		cout << s << endl;
//	}

	if (t.size() == 4) {
		cout << "Input file is weighted." << endl;
		graph.weighted = true;
		graph.insert_link(std::stoi(t[0]), std::stoi(t[1]), std::stod(t[2]));
		graph.insert_link(std::stoi(t[1]), std::stoi(t[0]), std::stod(t[2]));
		M++;
		while (fin >> u >> v >> w) {
			graph.insert_link(u, v, w);
			graph.insert_link(v, u, w);
			M++;
		}
		graph.init_degree();
	} else if (t.size() == 3) {
		cout << "Input file is unweighted." << endl;
		graph.weighted = false;
		graph.insert_link(std::stoi(t[0]), std::stoi(t[1]));
		graph.insert_link(std::stoi(t[1]), std::stoi(t[0]));
		M++;
		while (fin >> u >> v >> w) {
			graph.insert_link(u, v);
			graph.insert_link(v, u);
			M++;
		}
		graph.init_degree();
	} else {
		cout << "Input file is not a valid adjacent link table of a graph."
				<< endl;
	}
	fin.close();
}

InputGraph::~InputGraph() {
	// TODO Auto-generated destructor stub
}

