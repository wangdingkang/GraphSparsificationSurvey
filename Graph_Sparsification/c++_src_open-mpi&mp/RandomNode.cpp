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

EdgeGraph RandomNode::get_sampled_graph(const AdjLinkGraph &graph,
		int sampled_size) {
	int n = graph.get_num_of_nodes();

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

	EdgeGraph ret;
	for (int i = 0; i < n; i++) {
		if (selected_nodes[i]) {
			for (auto& p : graph.adjlink[i]) {
				if(selected_nodes[p.v] && p.v > i) {
					ret.push_back(Edge(i, p.v, p.w));
				}
			}
		}
	}
	return ret;
}
