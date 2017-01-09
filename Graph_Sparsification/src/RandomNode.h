/*
 * RandomNode.h
 *
 *  Created on: Jan 8, 2017
 *      Author: dkwan
 */

#ifndef RANDOMNODE_H_
#define RANDOMNODE_H_

#include "Structures.h"

class RandomNode {
public:
	RandomNode();

	virtual ~RandomNode();

	// random node sample uniformly, without replacement, exactly sample $sampled_size nodes. return induced graph by those nodes.
	vector<Edge> get_sampled_graph(vector<vector<int> > &paths, int sampled_size);

};

#endif /* RANDOMNODE_H_ */
