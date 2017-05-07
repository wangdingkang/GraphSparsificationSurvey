/*
 * Landmark.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "Landmark.h"

#define MAX_SIZE 1000000 // maximal size of a cluster

Landmark::Landmark(const InputGraph &graph) {
	this->network = &graph;
	this->sampled_size = 0;
	this->N = network->graph.get_num_of_nodes();
}

void Landmark::landmark_sampling(int S, landmark_type t) {
	sampled_size = 0;
	vector<int> landmarks;
	if (t == UNIFORM) {
		landmarks = network->uniform_random_sample(S);
	} else {
		landmarks = network->degree_random_sample(S);
	}

	subset = landmarks;
	vector<int> depth(N);
	assignment = vector<int>(N);
	std::fill(assignment.begin(), assignment.end(), -1);
	for (int i = 0; i < S; i++) {
		assignment[landmarks[i]] = i;
	}

	int cnt_landmark = landmarks.size();
	cnt_cluster = vector<int>(cnt_landmark);
	fill(cnt_cluster.begin(), cnt_cluster.end(), 1);

	assign_nodes_to_landmarks(assignment, landmarks, cnt_cluster, depth);
	int cnt_cmax = *max_element(cnt_cluster.begin(), cnt_cluster.end());
	int cnt_cmin = *min_element(cnt_cluster.begin(), cnt_cluster.end());
	cerr << "max_cluster has " << cnt_cmax << " nodes, and min_cluster has "
			<< cnt_cmin << " nodes." << endl;
	construct_graph_eigen(assignment, cnt_cluster, ret_eigen);
	construct_graph_apsp(assignment, depth, ret_apsp);
	sampled_size = (int) landmarks.size();

}

void Landmark::construct_graph_apsp(vector<int>& assignment, vector<int>& depth,
		vector<Edge>& ret) {
	ret.clear();
	map<pair<int, int>, int> connections;
	for (int i = 0; i < N; i++) {
		int a = assignment[i];
		for (auto& child : network->graph.adjlink[i]) {
			int b = assignment[child.v];
			if (a != b) {
				pair<int, int> tp = make_pair(min(a, b), max(a, b));
				int distance = depth[i] + depth[child.v] + 1;
				if (connections.find(tp) != connections.end()) {
					connections[tp] = min(connections[tp], distance);
				} else {
					connections.insert(make_pair(tp, distance));
				}
			}
		}
	}
	for (auto& ele : connections) {
		int u = ele.first.first;
		int v = ele.first.second;
		ret.push_back(Edge(u, v, ele.second)); // using shortest path between two landmarks.
	}
}

void Landmark::construct_graph_eigen(vector<int>& assignment,
		vector<int> &cnt_cluster, vector<Edge>& ret) {
	ret.clear();
	map<pair<int, int>, double> connections;
	for (int i = 0; i < N; i++) {
		int a = assignment[i];
		for (auto& child : network->graph.adjlink[i]) {
			int b = assignment[child.v];
			if (a != b) {
				pair<int, int> tp = make_pair(min(a, b), max(a, b));
				if (connections.find(tp) != connections.end()) {
					connections[tp] = connections[tp] + child.w;
				} else {
					connections.insert(make_pair(tp, child.w));
				}
			}
		}
	}
	for (auto& ele : connections) {
		int u = ele.first.first;
		int v = ele.first.second;
		ret.push_back(
				Edge(u, v,
						ele.second / (2.0f * cnt_cluster[u] * cnt_cluster[v]))); // using edge density.
	}
}


void Landmark::assign_nodes_to_landmarks(vector<int>& assignment,
		const vector<int>& landmarks, vector<int>& cnt_cluster,
		vector<int>& depth) {
	vector<int> to_be_assign(landmarks);
	add_set_to_assign(assignment, to_be_assign);
	int td = 1;
	while (!to_be_assign.empty()) {
		vector<pair<int, int>> this_turn;
		for (auto& node : to_be_assign) {
			vector<int> candidates;
			int max_size = MAX_SIZE;
			for (auto& child : network->graph.adjlink[node]) {
				if (assignment[child.v] >= 0) {
					int tcnt = cnt_cluster[assignment[child.v]];
					if (tcnt < max_size) {
						max_size = tcnt;
						candidates.clear();
						candidates.push_back(assignment[child.v]);
					} else if (tcnt == max_size) {
						candidates.push_back(assignment[child.v]);
					}
				}
			}
			int r = rand() % (int) candidates.size();
			this_turn.push_back(make_pair(node, candidates[r]));
			cnt_cluster[candidates[r]]++;
			depth[node] = td;
		}
		for(auto item: this_turn) {
			assignment[item.first] = item.second;
		}
		add_set_to_assign(assignment, to_be_assign);
		td++;
	}
}

void Landmark::add_set_to_assign(vector<int>& assignment,
		vector<int>& to_be_assign) {
	vector<int> temp(to_be_assign);
	to_be_assign.clear();
	for (auto& node : temp) {
		for (auto& child : network->graph.adjlink[node]) {
			// if not assigned yet and not added to the to_be_assign set (-2)
			if (assignment[child.v] == -1) {
				to_be_assign.push_back(child.v);
				assignment[child.v] = -2; // already in set, ready to be assigned, avoid duplicate insertion.
			}
		}
	}
}

Landmark::~Landmark() {
// TODO Auto-generated destructor stub

}

