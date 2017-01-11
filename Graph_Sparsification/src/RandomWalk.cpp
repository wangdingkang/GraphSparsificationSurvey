/*
 * RandomWalk.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "RandomWalk.h"

RandomWalk::RandomWalk() {
	// TODO Auto-generated constructor stub
}

vector<Edge> RandomWalk::get_sampled_graph(const AdjLinkGraph& paths,
		int sampled_size, random_walk_type t, double p) {
	vector<int> nodes;
	vector<Edge> ret;
	switch (t) {
	case RANDOM_WALK_WITH_JUMP:
		nodes = rwj_sampled_points(paths, sampled_size, p);
		break;

	case RANDOM_WALK_WITH_RESTART:
		break;

	default:
		nodes = rw_sampled_points(paths, sampled_size);
	}

	return graph_from_sampled_points(paths, nodes);
}

vector<Edge> RandomWalk::graph_from_sampled_points(const AdjLinkGraph& paths,
		vector<int> &nodes) {
	vector<Edge> ret;
	int n = paths.size();
	bool has_this_point[n];
	memset(has_this_point, false, sizeof(has_this_point));
	for (auto p : nodes) {
		has_this_point[p] = true;
	}

	for (auto p : nodes) {
		for (auto t : paths[p]) {
			if (has_this_point[t]) {
				ret.push_back(Edge(p, t, 1));
			}
		}
	}

	return ret;
}

// pure random walk without restart
vector<int> RandomWalk::rw_sampled_points(const AdjLinkGraph& paths,
		int sampled_size) {
	int n = paths.size();
	srand(time(NULL));
	int s = rand() % n;
	// start from s

	int iterations = 0;
	vector<int> ret;
	bool visited[n];
	memset(visited, false, sizeof(visited));

	// if walk too long, and still get too few points, just return the sampled points.
	while ((int) ret.size() < sampled_size && iterations < 5 * sampled_size) {
		if (!visited[s]) {
			ret.push_back(s);
			visited[s] = true;
		}
		int m = paths[s].size();
		int next = rand() % m;
		s = paths[s][next];
		iterations++;
	}
	return ret;
}

// random walk without jump
vector<int> RandomWalk::rwj_sampled_points(const AdjLinkGraph& paths,
		int sampled_size, double jump_prob) {
	int n = paths.size();
	srand(time(NULL));
	int s = rand() % n;
	int iterations = 0;
	vector<int> ret;
	bool visited[n];
	memset(visited, false, sizeof(visited));

	// if walk too long, and still get too few points, just return the sampled points.
	while ((int) ret.size() < sampled_size && iterations < 5 * sampled_size) {
		if (!visited[s]) {
			ret.push_back(s);
			visited[s] = true;
		}

		double f = (double) rand() / RAND_MAX;

		if (f > jump_prob) {
			int m = paths[s].size();
			int next = rand() % m;
			s = paths[s][next];
		} else { // jump to a random point.
			int next = rand() % n;
			s = next;
		}
		iterations++;
	}

	return ret;
}

RandomWalk::~RandomWalk() {
	// TODO Auto-generated destructor stub
}

