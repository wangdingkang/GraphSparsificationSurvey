/*
 * OutputGraph.cpp
 *
 *  Created on: 2017Äê1ÔÂ9ÈÕ
 *      Author: Dingkang Wang
 */

#include "OutputGraph.h"

OutputGraph::OutputGraph() {
	// TODO Auto-generated constructor stub

}

void OutputGraph::output(string filename, EdgeGraph g) {
	ofstream fout;
	fout.open(filename);
	for (auto &edge : g) {
		fout << edge.u << " " << edge.v << endl;
	}
	fout.close();
}

void OutputGraph::output_weighted(string filename, EdgeGraph g) {
	ofstream fout;
	fout.open(filename);
	for (auto &edge : g) {
		fout << edge.u << " " << edge.v << " " << edge.w << endl;
	}
}

OutputGraph::~OutputGraph() {
	// TODO Auto-generated destructor stub
}

