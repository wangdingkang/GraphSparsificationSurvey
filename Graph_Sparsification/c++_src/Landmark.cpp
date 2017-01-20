/*
 * Landmark.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "Landmark.h"

Landmark::Landmark(int _depth, const AdjLinkGraph& _graph) {
	this->SEARCH_DEPTH = _depth;
	this->graph = _graph;
	this->sampled_size = 0;
	this->N = graph.get_num_of_nodes();
}

vector<Edge> Landmark::get_sampled_graph_farthest_biased() {
	sampled_size = 0;
	vector<Edge> ret;
	vector<vector<int> > visited = vector<vector<int>>(N, vector<int>());

	srand(time(NULL));
	int s = rand() % N;

	vector<int> depth(N);
	for (int i = 0; i < N; i++)
		depth[i] = N;

	while(s != -1) {
		bfs_search(s, visited);
		bfs_cover(s, depth);
		sampled_size++;
		s = -1;
		int max_d = SEARCH_DEPTH;
		for (int i = 0; i < N; i++) {
			if (depth[i] > max_d) {
				s = i;
				max_d = depth[i];
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (visited[i].size() > 1) {
			visited[i][0] = visited[i][rand() % (visited[i].size())];
		}
	}

	map<pair<int, int>, double> connections;
	for (int i = 0; i < N; i++) {
		for (auto t : graph.adjlink[i]) {
			int fc = visited[i][0];
			int tc = visited[t.v][0];
			if (fc != tc) {
				int mic = min(fc, tc);
				int mac = max(fc, tc);
				pair<int, int> tp = make_pair(mic, mac);
				if (connections.find(tp) != connections.end()) {
					connections[tp] = connections[tp] + t.w;
				} else {
					connections.insert(make_pair(tp, t.w));
				}
			}
		}
	}

	for (auto &ele : connections) {
		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
	}

	return ret;
}

vector<Edge> Landmark::get_sampled_graph_degree_biased_random_assignment() {
	sampled_size = 0;
	vector<Edge> ret;
	vector<vector<int> > visited = vector<vector<int>>(N, vector<int>());
	vector<Sort_Element> degree = vector<Sort_Element>(N);

	for (int i = 0; i < N; i++) {
		degree[i].index = i;
		degree[i].value = graph.get_degree_of_node(i);
	}

	sort(degree.begin(), degree.end());
	for (int i = 0; i < N; i++) {
		int index = degree[i].index;
		if (visited[index].size() == 0) {
			bfs_search(index, visited);
			sampled_size++;
		}
	}

	srand(time(NULL));
	for (int i = 0; i < N; i++) {
		if (visited[i].size() > 1) {
			visited[i][0] = visited[i][rand() % (visited[i].size())];
		}
	}

	map<pair<int, int>, double> connections;
	for (int i = 0; i < N; i++) {
		for (auto t : graph.adjlink[i]) {
			int fc = visited[i][0];
			int tc = visited[t.v][0];
			if (fc != tc) {
				int mic = min(fc, tc);
				int mac = max(fc, tc);
				pair<int, int> tp = make_pair(mic, mac);
				if (connections.find(tp) != connections.end()) {
					connections[tp] = connections[tp] + t.w;
				} else {
					connections.insert(make_pair(tp, t.w));
				}
			}
		}
	}

	for (auto &ele : connections) {
		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
	}

	return ret;
}

vector<Edge> Landmark::get_sampled_graph_degree_biased() {
	sampled_size = 0;
	vector<Edge> ret;
	vector<int> visited = vector<int>(N);
	std::fill(visited.begin(), visited.end(), -1);
	vector<Sort_Element> degree = vector<Sort_Element>(N);

	for (int i = 0; i < N; i++) {
		degree[i].index = i;
		degree[i].value = graph.get_degree_of_node(i);
	}

	sort(degree.begin(), degree.end());

	for (int i = 0; i < N; i++) {
		int index = degree[i].index;
		if (visited[index] == -1) {
			bfs_search(index, visited);
			sampled_size++;
		}
	}

	map<pair<int, int>, double> connections;
	for (int i = 0; i < N; i++) {
		for (auto t : graph.adjlink[i]) {
			int fc = visited[i];
			int tc = visited[t.v];
			if (fc != tc) {
				int mic = min(fc, tc);
				int mac = max(fc, tc);
				pair<int, int> tp = make_pair(mic, mac);
				if (connections.find(tp) != connections.end()) {
					connections[tp] = connections[tp] + t.w;
				} else {
					connections.insert(make_pair(tp, t.w));
				}
			}
		}
	}

	for (auto &ele : connections) {
		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
	}

	return ret;
}

vector<Edge> Landmark::get_sampled_graph_uniform_random_assignment() {
	sampled_size = 0;
	vector<Edge> ret;
	vector<int> order;
	vector<vector<int> > visited = vector<vector<int>>(N, vector<int>());
	for (int i = 0; i < N; i++) {
		order.push_back(i);
	}
	srand(time(NULL));
	random_shuffle(order.begin(), order.end());

	for (int i = 0; i < N; i++) {
		int index = order[i];
		if (visited[index].size() == 0) {
			bfs_search(index, visited);
			sampled_size++;
		}
	}

	for (int i = 0; i < N; i++) {
		if (visited[i].size() > 1) {
			visited[i][0] = visited[i][rand() % (visited[i].size())];
		}
	}

	map<pair<int, int>, double> connections;
	for (int i = 0; i < N; i++) {
		for (auto t : graph.adjlink[i]) {
			int fc = visited[i][0];
			int tc = visited[t.v][0];
			if (fc != tc) {
				int mic = min(fc, tc);
				int mac = max(fc, tc);
				pair<int, int> tp = make_pair(mic, mac);
				if (connections.find(tp) != connections.end()) {
					connections[tp] = connections[tp] + t.w;
				} else {
					connections.insert(make_pair(tp, t.w));
				}
			}
		}
	}

	for (auto &ele : connections) {
		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
	}

	return ret;
}

void Landmark::bfs_search(int s, vector<int>& visited) {
	queue<PII> bfs_queue;
	bfs_queue.push(make_pair(s, 0));
	while (!bfs_queue.empty()) {
		PII root = bfs_queue.front();
		bfs_queue.pop();
		int ri = root.first;
		int rdep = root.second;
		visited[ri] = sampled_size;
		for (auto child : graph.adjlink[ri]) {
			if (visited[child.v] == -1) {
				if (rdep < SEARCH_DEPTH)
					bfs_queue.push(make_pair(child.v, rdep + 1));
			}
		}
	}
}

void Landmark::bfs_search(int s, vector<vector<int> >& visited) {
	queue<PII> bfs_queue;
	bfs_queue.push(make_pair(s, 0));
	while (!bfs_queue.empty()) {
		PII root = bfs_queue.front();
		bfs_queue.pop();
		int ri = root.first;
		int rdep = root.second;
		visited[ri].push_back(sampled_size);
		for (auto child : graph.adjlink[ri]) {
			if (rdep < SEARCH_DEPTH)
				bfs_queue.push(make_pair(child.v, rdep + 1));
		}
	}

}

void Landmark::bfs_cover(int s, vector<int>& depth) {
	queue<PII> bfs_queue;
	bfs_queue.push(make_pair(s, 0));
	while (!bfs_queue.empty()) {
		PII root = bfs_queue.front();
		bfs_queue.pop();
		int ri = root.first;
		int rdep = root.second;
		depth[ri] = rdep;
		for (auto child : graph.adjlink[ri]) {
			if (depth[child.v] > rdep + 1) {
				bfs_queue.push(make_pair(child.v, rdep + 1));
			}
		}
	}
}

Landmark::~Landmark() {
	// TODO Auto-generated destructor stub
}

