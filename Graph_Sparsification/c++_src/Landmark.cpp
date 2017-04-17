/*
 * Landmark.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "Landmark.h"
#define MAX_SIZE 100000
//Landmark::Landmark(int _depth, const AdjLinkGraph& _graph) {
//	this->SEARCH_DEPTH = _depth;
//	this->graph = _graph;
//	this->sampled_size = 0;
//	this->N = graph.get_num_of_nodes();
//}

Landmark::Landmark(const InputGraph &graph) {
	this->network = graph;
	this->sampled_size = 0;
	this->N = network.graph.get_num_of_nodes();
}

void Landmark::landmark_sampling(int S, landmark_type t) {
	sampled_size = 0;
//	vector<Edge> ret;
	srand(time(NULL));
	vector<int> landmarks;
	if (t == UNIFORM) {
		vector<int> randoms;
		for (int i = 0; i < N; i++) {
			randoms.push_back(i);
		}
		random_shuffle(randoms.begin(), randoms.end());
		landmarks = vector<int>(randoms.begin(), randoms.begin() + S);
	} else {
		landmarks = network.degree_random_sample(S);
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
//	cout << "remove small clusters..." << endl;
//	int remove_limit = 0.1 * S, removed = 0;
//	for (int i = cnt_landmark - 1; i >= 0; i--) {
//		int size = cnt_cluster[assignment[landmarks[i]]];
//		if (size < 5 && removed < remove_limit) {
//			removed++;
//			landmarks.erase(landmarks.begin() + i);
//		}
//	}
//
//	cout << "reassign..." << endl;
//	fill(cnt_cluster.begin(), cnt_cluster.end(), 0);
//	fill(assignment.begin(), assignment.end(), -1);
//	std::fill(assignment.begin(), assignment.end(), -1);
//	for (int i = 0; i < (int) landmarks.size(); i++) {
//		assignment[landmarks[i]] = i;
//	}
//	cnt_landmark = landmarks.size();
//	cnt_cluster = vector<int>(cnt_landmark);
//	fill(cnt_cluster.begin(), cnt_cluster.end(), 0);
//	assign_nodes_to_landmarks(assignment, landmarks, cnt_cluster);
//	cnt_cmax = *max_element(cnt_cluster.begin(), cnt_cluster.end());
//	cnt_cmin = *min_element(cnt_cluster.begin(), cnt_cluster.end());
//	cerr << "after removal, max = " << cnt_cmax << ", min = " << cnt_cmin << "." <<  endl;
	construct_graph_eigen(assignment, cnt_cluster, ret_eigen);
	construct_graph_apsp(assignment, depth, ret_apsp);
	sampled_size = (int) landmarks.size();
//	return ret;
}

void Landmark::construct_graph_apsp(vector<int>& assignment, vector<int>& depth,
		vector<Edge>& ret) {
	ret.clear();
	map<pair<int, int>, int> connections;
	for (int i = 0; i < N; i++) {
		int a = assignment[i];
		for (auto& child : network.graph.adjlink[i]) {
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
		for (auto& child : network.graph.adjlink[i]) {
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

// layer by layer, assign new nodes to existing & adjacent cluster with smallest size,
// if multiply having the same size, then randomly pick one.
void Landmark::assign_nodes_to_landmarks(vector<int>& assignment,
		const vector<int>& landmarks, vector<int>& cnt_cluster,
		vector<int>& depth) {
	vector<int> to_be_assign(landmarks);
	add_set_to_assign(assignment, to_be_assign);
	int td = 1;
	while (!to_be_assign.empty()) {
		for (auto& node : to_be_assign) {
			vector<int> candidates;
			int max_size = MAX_SIZE;
			for (auto& child : network.graph.adjlink[node]) {
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
			assignment[node] = candidates[r];
			cnt_cluster[candidates[r]]++;
			depth[node] = td;
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
		for (auto& child : network.graph.adjlink[node]) {
			// if not assigned yet and not added to the to_be_assign set (-2)
			if (assignment[child.v] == -1) {
				to_be_assign.push_back(child.v);
				assignment[child.v] = -2; // already in set, ready to be assigned, avoid duplicate insertion.
			}
		}
	}
}

//void Landmark::update_depth(int s, vector<int>& depth) {
//	queue<PII> bfs_queue;
//	bfs_queue.push(make_pair(s, 0));
//	while (!bfs_queue.empty()) {
//		PII root = bfs_queue.front();
//		bfs_queue.pop();
//		int i = root.first;
//		int d = root.second;
//		depth[i] = d;
//		for (auto &child : network.graph.adjlink[i]) {
//			if (d + 1 < depth[child.v]) {
//				bfs_queue.push(make_pair(child.v, d + 1));
//			}
//		}
//	}
//}

//vector<Edge> Landmark::farthest_landmark_sampling(int S) {
//	sampled_size = 0;
//	vector<Edge> ret;
//
//	srand(time(NULL));
//	int s = rand() % N;
//
//	vector<int> depth(N);
//	std::fill(depth.begin(), depth.end(), N);
//	vector<int> assignment(N);
//	std::fill(assignment.begin(), assignment.end(), -1);
//
//	vector<int> landmarks;
//
//	// do farthest sampling, each time you sample the node that is farthest from existing set.
////	cerr << "farthest sampling" << endl;
//	for (int i = 0; i < S; i++) {
//
//		assignment[s] = (int) landmarks.size();
//		landmarks.push_back(s);
//		update_depth(s, depth);
//
//		// find the max depth
//		vector<int> candidates;
//		int max_v = *max_element(depth.begin(), depth.end());
//		if (max_v != 0) {
//			// add candidates;
//			for (int j = 0; j < N; j++) {
//				if (depth[j] == max_v) {
//					candidates.push_back(j);
//				}
//			}
//			int t = rand() % (int) candidates.size();
//			s = candidates[t];
//		} else {
//			cerr << "Warning: all nodes sampled." << endl;
//			break;
//		}
//
//	}
//
//	assign_nodes_to_landmarks(assignment, landmarks);
//	int cnt_landmark = (int) landmarks.size();
//
//	vector<int> cnt_cluster(cnt_landmark);
//
//	construct_graph(assignment, cnt_cluster, ret);
//
//	int cnt_cmax = *max_element(cnt_cluster.begin(), cnt_cluster.end());
//	int cnt_cmin = *min_element(cnt_cluster.begin(), cnt_cluster.end());
//	cout << "max_cluster has " << cnt_cmax << " nodes, and min_cluster has "
//			<< cnt_cmin << " nodes." << endl;
//
//	sampled_size = (int) landmarks.size();
//
//	return ret;
//}

//void Landmark::bfs_search(int s, vector<vector<int> >& visited) {
//	queue<PII> bfs_queue;
//	bfs_queue.push(make_pair(s, 0));
//	bool temp_visited[N];
//	memset(temp_visited, false, sizeof(temp_visited));
//	while (!bfs_queue.empty()) {
//		PII root = bfs_queue.front();
//		bfs_queue.pop();
//		int ri = root.first;
//		int rdep = root.second;
//		temp_visited[ri] = true;
//		visited[ri].push_back(sampled_size);
//		for (auto child : graph.adjlink[ri]) {
//			if (!temp_visited[child.v] && rdep < SEARCH_DEPTH)
//				bfs_queue.push(make_pair(child.v, rdep + 1));
//		}
//	}
//
//}

//vector<Edge> Landmark::get_sampled_graph_farthest_biased() {
//	sampled_size = 0;
//	vector<Edge> ret;
//	vector<vector<int> > visited = vector<vector<int>>(N, vector<int>());
//
//	srand(time(NULL));
//	int s = rand() % N;
//	double cnt_cluster[N];
//	vector<int> depth(N);
//	for (int i = 0; i < N; i++)
//		depth[i] = N;
//	memset(cnt_cluster, 0, sizeof(cnt_cluster));
//
//	while (s != -1) {
//		bfs_search(s, visited);
//		bfs_cover(s, depth);
//		sampled_size++;
//		s = -1;
//		int max_d = SEARCH_DEPTH;
//		for (int i = 0; i < N; i++) {
//			if (depth[i] > max_d) {
//				s = i;
//				max_d = depth[i];
//			}
//		}
//	}
//
//	for (int i = 0; i < N; i++) {
//		if (visited[i].size() > 1) {
//			visited[i][0] = visited[i][rand() % (visited[i].size())];
//		}
//	}
//
//	map<pair<int, int>, double> connections;
//	for (int i = 0; i < N; i++) {
//		cnt_cluster[visited[i][0]]++;
//		for (auto t : graph.adjlink[i]) {
//			int fc = visited[i][0];
//			int tc = visited[t.v][0];
//			if (fc != tc) {
//				int mic = min(fc, tc);
//				int mac = max(fc, tc);
//
//				pair<int, int> tp = make_pair(mic, mac);
//				if (connections.find(tp) != connections.end()) {
//					connections[tp] = connections[tp] + t.w;
//				} else {
//					connections.insert(make_pair(tp, t.w));
//				}
//			}
//		}
//	}
//
//	for (auto &ele : connections) {
//		int u = ele.first.first;
//		int v = ele.first.second;
//		ret.push_back(
//				Edge(u, v, ele.second / (2.0f * cnt_cluster[u] * cnt_cluster[v])));
//	}
//
//	return ret;
//}
//
//vector<Edge> Landmark::get_sampled_graph_degree_biased_random_assignment() {
//	sampled_size = 0;
//	vector<Edge> ret;
//	vector<vector<int> > visited = vector<vector<int>>(N, vector<int>());
//	vector<Sort_Element> degree = vector<Sort_Element>(N);
//
//	for (int i = 0; i < N; i++) {
//		degree[i].index = i;
//		degree[i].value = graph.get_degree_of_node(i);
//	}
//
//	sort(degree.begin(), degree.end());
//	for (int i = 0; i < N; i++) {
//		int index = degree[i].index;
//		if (visited[index].size() == 0) {
//			bfs_search(index, visited);
//			sampled_size++;
//		}
//	}
//
//	srand(time(NULL));
//	for (int i = 0; i < N; i++) {
//		if (visited[i].size() > 1) {
//			visited[i][0] = visited[i][rand() % (visited[i].size())];
//		}
//	}
//
//	map<pair<int, int>, double> connections;
//	for (int i = 0; i < N; i++) {
//		for (auto t : graph.adjlink[i]) {
//			int fc = visited[i][0];
//			int tc = visited[t.v][0];
//			if (fc != tc) {
//				int mic = min(fc, tc);
//				int mac = max(fc, tc);
//				pair<int, int> tp = make_pair(mic, mac);
//				if (connections.find(tp) != connections.end()) {
//					connections[tp] = connections[tp] + t.w;
//				} else {
//					connections.insert(make_pair(tp, t.w));
//				}
//			}
//		}
//	}
//
//	for (auto &ele : connections) {
//		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
//	}
//
//	return ret;
//}
//
//vector<Edge> Landmark::get_sampled_graph_degree_biased() {
//	sampled_size = 0;
//	vector<Edge> ret;
//	vector<int> visited = vector<int>(N);
//	std::fill(visited.begin(), visited.end(), -1);
//	vector<Sort_Element> degree = vector<Sort_Element>(N);
//
//	for (int i = 0; i < N; i++) {
//		degree[i].index = i;
//		degree[i].value = graph.get_degree_of_node(i);
//	}
//
//	sort(degree.begin(), degree.end());
//
//	for (int i = 0; i < N; i++) {
//		int index = degree[i].index;
//		if (visited[index] == -1) {
//			bfs_search(index, visited);
//			sampled_size++;
//		}
//	}
//
//	map<pair<int, int>, double> connections;
//	for (int i = 0; i < N; i++) {
//		for (auto t : graph.adjlink[i]) {
//			int fc = visited[i];
//			int tc = visited[t.v];
//			if (fc != tc) {
//				int mic = min(fc, tc);
//				int mac = max(fc, tc);
//				pair<int, int> tp = make_pair(mic, mac);
//				if (connections.find(tp) != connections.end()) {
//					connections[tp] = connections[tp] + t.w;
//				} else {
//					connections.insert(make_pair(tp, t.w));
//				}
//			}
//		}
//	}
//
//	for (auto &ele : connections) {
//		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
//	}
//
//	return ret;
//}
//
//vector<Edge> Landmark::get_sampled_graph_uniform_random_assignment() {
//	sampled_size = 0;
//	vector<Edge> ret;
//	vector<int> order;
//	vector<vector<int> > visited = vector<vector<int>>(N, vector<int>());
//	for (int i = 0; i < N; i++) {
//		order.push_back(i);
//	}
//	srand(time(NULL));
//	random_shuffle(order.begin(), order.end());
//
//	for (int i = 0; i < N; i++) {
//		int index = order[i];
//		if (visited[index].size() == 0) {
//			bfs_search(index, visited);
//			sampled_size++;
//		}
//	}
//
//	for (int i = 0; i < N; i++) {
//		if (visited[i].size() > 1) {
//			visited[i][0] = visited[i][rand() % (visited[i].size())];
//		}
//	}
//
//	map<pair<int, int>, double> connections;
//	for (int i = 0; i < N; i++) {
//		for (auto t : graph.adjlink[i]) {
//			int fc = visited[i][0];
//			int tc = visited[t.v][0];
//			if (fc != tc) {
//				int mic = min(fc, tc);
//				int mac = max(fc, tc);
//				pair<int, int> tp = make_pair(mic, mac);
//				if (connections.find(tp) != connections.end()) {
//					connections[tp] = connections[tp] + t.w;
//				} else {
//					connections.insert(make_pair(tp, t.w));
//				}
//			}
//		}
//	}
//
//	for (auto &ele : connections) {
//		ret.push_back(Edge(ele.first.first, ele.first.second, ele.second / 2));
//	}
//
//	return ret;
//}
//
//void Landmark::bfs_search(int s, vector<int>& visited) {
//	queue<PII> bfs_queue;
//	bfs_queue.push(make_pair(s, 0));
//	while (!bfs_queue.empty()) {
//		PII root = bfs_queue.front();
//		bfs_queue.pop();
//		int ri = root.first;
//		int rdep = root.second;
//		visited[ri] = sampled_size;
//		for (auto child : graph.adjlink[ri]) {
//			if (visited[child.v] == -1) {
//				if (rdep < SEARCH_DEPTH)
//					bfs_queue.push(make_pair(child.v, rdep + 1));
//			}
//		}
//	}
//}
//
//void Landmark::bfs_search(int s, vector<vector<int> >& visited) {
//	queue<PII> bfs_queue;
//	bfs_queue.push(make_pair(s, 0));
//	bool temp_visited[N];
//	memset(temp_visited, false, sizeof(temp_visited));
//	while (!bfs_queue.empty()) {
//		PII root = bfs_queue.front();
//		bfs_queue.pop();
//		int ri = root.first;
//		int rdep = root.second;
//		temp_visited[ri] = true;
//		visited[ri].push_back(sampled_size);
//		for (auto child : graph.adjlink[ri]) {
//			if (!temp_visited[child.v] && rdep < SEARCH_DEPTH)
//				bfs_queue.push(make_pair(child.v, rdep + 1));
//		}
//	}
//
//}
//
//void Landmark::bfs_cover(int s, vector<int>& depth) {
//	queue<int> bfs_queue;
//	bfs_queue.push(s);
//	depth[s] = 0;
//	while (!bfs_queue.empty()) {
//		int root = bfs_queue.front();
//		int d = depth[root] + 1;
//		bfs_queue.pop();
//		for (auto child : graph.adjlink[root]) {
//			if (depth[child.v] > d) {
//				depth[child.v] = d;
//				bfs_queue.push(child.v);
//			}
//		}
//	}
//}

Landmark::~Landmark() {
// TODO Auto-generated destructor stub
}

