/*
 * Snowball.h
 *
 *  Created on: Jan 7, 2017
 *      Author: dkwan
 */

#ifndef SNOWBALL_H_
#define SNOWBALL_H_

#include "Structures.h"

class Snowball {
public:
	Snowball();

	// size of sampled graph
	int sampled_size;

	// indexes of sampled subset
	unordered_set<int> subset;

	// given fixed iteration arg_T
	EdgeGraph snowball_sampling(const AdjLinkGraph& graph, int arg_N, int arg_K, int arg_T);

	// given target size of sampling arg_SN
	EdgeGraph snowball_sampling_with_size(const AdjLinkGraph& graph, int arg_N, int arg_K, int arg_SN);

	virtual ~Snowball();

private:

	// generate random init seeds, ############################ need to rewrite to be consistent with Landmark Sampling.
	vector<int> random_ints(int n, int k);

};

#endif /* SNOWBALL_H_ */
