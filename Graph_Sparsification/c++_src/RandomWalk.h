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

	// size of sampled graph
	int sampled_size;

	// indexes of sampled nodes
	unordered_set<int> subset;

	//	return sampled graph via a list of edges
	EdgeGraph get_sampled_graph(const AdjLinkGraph& graph, int sampled_size, random_walk_type t, double p = 0.1);

	virtual ~RandomWalk();


private:

	// pure random walk without restart
	EdgeGraph rw_sampled_points(const AdjLinkGraph& graph, int sampled_size);

	// random walk with jump.
	/*############################################ need to rewrite*/
	vector<int> rwj_sampled_points(const AdjLinkGraph& graph, int sampled_size, double jump_prob );

};

#endif /* RANDOMWALK_H_ */
