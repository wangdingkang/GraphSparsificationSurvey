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

	Landmark(const InputGraph &graph);

	int sampled_size; 	// size of returned subsampled graph
	int N;				// # of nodes in the original graph

	const InputGraph *network;	// reference to the original graph.
	vector<int> subset;		// subsampled nodes
	vector<int> assignment;	// assignment of each nodes in original graph.
	vector<int> cnt_cluster;	//  how many nodes each cluster has

	vector<Edge> ret_eigen;	// for eigen-structure purpose
	vector<Edge> ret_apsp;	// for sp purpose

	// stored in variables ret_eigen & ret_apsp, since we have multiple results and may be more.
	void landmark_sampling(int sample_size, landmark_type t);

	virtual ~Landmark();

private:

	/*
	 * Landmark sampling, each time pick the point with max degree, and assign its neighbors within distance "depth" to it to form a cluster,
	 till no point left.
	 * weight between any two cluster(landmark), it's the number of connections between two landmark.
	 */

	// layer by layer, assign new nodes to existing & adjacent cluster with smallest size,
	// if multiply having the same size, then randomly pick one.
	void assign_nodes_to_landmarks(vector<int>& assignment,
			const vector<int>& landmarks, vector<int>& cnt_cluster,
			vector<int>& depth);

	void construct_graph_eigen(vector<int>& assignment,
			vector<int>& cnt_cluster, vector<Edge>& ret);

	void construct_graph_apsp(vector<int>& assignment, vector<int>& depth,
			vector<Edge>& ret);

	// find a new layer of nodes to be assigned
	void add_set_to_assign(vector<int>& assignment, vector<int>& assign_set);

};

#endif /* LANDMARK_H_ */
