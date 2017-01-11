/*
 * RandomWalk.h
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */


#ifndef RANDOMWALK_H_
#define RANDOMWALK_H_

#include "Structures.h"

enum random_walk_type {
		RANDOM_WALK,
		RANDOM_WALK_WITH_JUMP,
		RANDOM_WALK_WITH_RESTART
	};

class RandomWalk {
public:
	RandomWalk();

	vector<Edge> get_sampled_graph(const AdjLinkGraph& paths, int sampled_size, random_walk_type t, double p = 0.1);

	virtual ~RandomWalk();


private:

	vector<int> rw_sampled_points(const AdjLinkGraph& paths, int sampled_size);

	// random walk with jump.
	vector<int> rwj_sampled_points(const AdjLinkGraph& paths, int sampled_size, double jump_prob );

	vector<Edge> graph_from_sampled_points(const AdjLinkGraph& paths, vector<int> &nodes);
};

#endif /* RANDOMWALK_H_ */