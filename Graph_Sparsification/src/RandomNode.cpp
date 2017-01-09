/*
 * RandomNode.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: dkwan
 */

#include "RandomNode.h"

RandomNode::RandomNode() {
	// TODO Auto-generated constructor stub

}

RandomNode::~RandomNode() {
	// TODO Auto-generated destructor stub
}

vector<Edge> RandomNode::get_sampled_graph(const AdjLinkGraph &paths,
		int sampled_size) {
	int n = paths.size();

	int random_array[n];
	for (int i = 0; i < n; i++) {
		random_array[i] = i;
	}

	bool selected_nodes[n];
	memset(selected_nodes, false, sizeof(selected_nodes));

	for (int i = 0; i < sampled_size; i++) {
		srand(time(NULL));
		int r = rand() % (n - i);

		selected_nodes[random_array[r]] = true;

		swap(random_array[r], random_array[n - i - 1]);

	}

	vector<Edge> ret;
	for (int i = 0; i < n; i++) {
		if (selected_nodes[i]) {
			for (auto p : paths[i]) {
				if(selected_nodes[p] && p > i) {
					ret.push_back(Edge(i, p, 1));
				}
			}
		}
	}
	return ret;
}
