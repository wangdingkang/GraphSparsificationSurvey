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
	Landmark(int depth, const AdjLinkGraph &paths);

	int SEARCH_DEPTH;
	int sampled_size;
	vector<int> visited;
	vector<Sort_Element> degree;
	vector<vector<int> > paths;
	vector<Edge> get_sampled_graph();
	virtual ~Landmark();


private:



	/*
	 * Landmark sampling, each time pick the point with max degree, and assign its neighbors within distance "depth" to it to form a cluster,
	   till no point left.
	 * weight between any two cluster(landmark), it's the number of connections between two landmark.
	 */

	void bfs_search(int s);
};

#endif /* LANDMARK_H_ */
