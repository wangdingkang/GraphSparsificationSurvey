/*
 * Landmark.h
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#ifndef LANDMARK_H_
#define LANDMARK_H_

#include "Structures.h"

class Landmark {
public:
	Landmark(int depth, const AdjLinkGraph &graph);

	int SEARCH_DEPTH;
	int sampled_size;
	int N;

	AdjLinkGraph graph;

	vector<Edge> get_sampled_graph_degree_biased();

	vector<Edge> get_sampled_graph_degree_biased_random_assignment();

	vector<Edge> get_sampled_graph_uniform_random_assignment();

	vector<Edge> get_sampled_graph_farthest_biased();

	virtual ~Landmark();


private:



	/*
	 * Landmark sampling, each time pick the point with max degree, and assign its neighbors within distance "depth" to it to form a cluster,
	   till no point left.
	 * weight between any two cluster(landmark), it's the number of connections between two landmark.
	 */

	void bfs_search(int s, vector<int>& visited);

	void bfs_search(int s, vector<vector<int> >& visited);

	void bfs_cover(int s, vector<int>& depth);
};

#endif /* LANDMARK_H_ */
