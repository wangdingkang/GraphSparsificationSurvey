/*
 * Landmark.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "Landmark.h"

Landmark::Landmark(int _depth, const AdjLinkGraph& _paths) {
	this->SEARCH_DEPTH = _depth;
	this->paths = _paths;
	this->sampled_size = 0;
	this->visited = vector<int>(paths.size());
	fill( visited.begin(), visited.end(), -1);
	this->degree = vector<Sort_Element>(paths.size());

}

vector<Edge> Landmark::get_sampled_graph() {
	vector<Edge> ret;

	int n = paths.size();

	for(int i = 0; i < n; i++) {
		degree[i].index = i;
		degree[i].value = paths[i].size();
	}

	sort(degree.begin(), degree.end());


	for(int i = 0; i < n; i++) {
		int index = degree[i].index;
		if(visited[index] == -1) {
			bfs_search(index);
			sampled_size++;
		}
	}

	map<pair<int, int>,  int> connections;
	for(int i = 0; i < n; i++) {
		for(auto v : paths[i]) {
			int fc = visited[i];
			int tc = visited[v];
			if(fc != tc) {
				int mic = min(fc, tc);
				int mac = max(fc, tc);
				pair<int, int> tp = make_pair(mic, mac);
				if(connections.find(tp) != connections.end()) {
					connections[tp] = connections[tp] + 1;
				} else {
					connections.insert(make_pair(tp, 1));
				}
			}
		}
	}

	for(auto &ele: connections) {
		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
	}

	return ret;
}

void Landmark::bfs_search(int s) {
	queue<PII> bfs_queue;
	bfs_queue.push(make_pair(s, 0));
	while (!bfs_queue.empty()) {
		PII root = bfs_queue.front();
		bfs_queue.pop();
		int ri = root.first;
		int rdep = root.second;
		visited[ri] = sampled_size;
		for (auto child : paths[ri]) {
			if (visited[child] == -1) {
				if (rdep < SEARCH_DEPTH)
					bfs_queue.push(make_pair(child, rdep + 1));
			}
		}
	}
}

Landmark::~Landmark() {
	// TODO Auto-generated destructor stub
}

