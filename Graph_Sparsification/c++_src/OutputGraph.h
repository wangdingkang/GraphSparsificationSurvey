/*
 * OutputGraph.h
 *
 *  Created on: 2017Äê1ÔÂ9ÈÕ
 *      Author: Dingkang Wang
 */

#ifndef OUTPUTGRAPH_H_
#define OUTPUTGRAPH_H_

#include "Structures.h"

class OutputGraph {
public:
	OutputGraph();
	void output(string filename, EdgeGraph g);
	void output_assignment(string filename, vector<int>& assignment,
			vector<int>& cnt_cluster);
	void output_weighted(string filaname, EdgeGraph g);
	void output_cluster_size_distribution(string filename,
			vector<int>& cnt_cluster);
	void output_subset(string filename, vector<int>& subset);
	void output_subset(string filename, unordered_set<int>& subset);
	void output_subset_spd(string filename, map<string, vector<double> > dist, int iteration);
	virtual ~OutputGraph();
};

#endif /* OUTPUTGRAPH_H_ */
