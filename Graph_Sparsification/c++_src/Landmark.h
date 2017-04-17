/*
 * Landmark.h
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#ifndef LANDMARK_H_
#define LANDMARK_H_

#include "Structures.h"
#include "InputGraph.h"

enum landmark_type {
	UNIFORM, DEGREE_BIASED
};

class Landmark {
public:
//	Landmark(int depth, const AdjLinkGraph &graph);
	Landmark(const InputGraph &graph);

//	int SEARCH_DEPTH;
	int sampled_size;
	int N;

	InputGraph network;
	vector<int> subset;
	vector<int> assignment;
	vector<int> cnt_cluster;

	vector<Edge> ret_eigen;
	vector<Edge> ret_apsp;

//	vector<Edge> get_sampled_graph_degree_biased();
//
//	vector<Edge> get_sampled_graph_degree_biased_random_assignment();
//
//	vector<Edge> get_sampled_graph_uniform_random_assignment();
//
//	vector<Edge> get_sampled_graph_farthest_biased();

// stored in variables ret_eigen & ret_apsp, since we have multiple results and may be more.
// void farthest_landmark_sampling(int sample_size);

// stored in variables ret_eigen & ret_apsp, since we have multiple results and may be more.
	void landmark_sampling(int sample_size, landmark_type t);

	virtual ~Landmark();

private:

	/*
	 * Landmark sampling, each time pick the point with max degree, and assign its neighbors within distance "depth" to it to form a cluster,
	 till no point left.
	 * weight between any two cluster(landmark), it's the number of connections between two landmark.
	 */
	void assign_nodes_to_landmarks(vector<int>& assignment,
			const vector<int>& landmarks, vector<int>& cnt_cluster,
			vector<int>& depth);

	void construct_graph_eigen(vector<int>& assignment,
			vector<int>& cnt_cluster, vector<Edge>& ret);

	void construct_graph_apsp(vector<int>& assignment, vector<int>& depth,
			vector<Edge>& ret);

	void add_set_to_assign(vector<int>& assignment, vector<int>& assign_set);

//	void update_depth(int s, vector<int>& depth);

//	void bfs_search(int s, vector<int>& visited);
//
//	void bfs_search(int s, vector<vector<int> >& visited);
//
//	void bfs_cover(int s, vector<int>& depth);
};

#endif /* LANDMARK_H_ */
