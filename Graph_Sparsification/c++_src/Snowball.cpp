/*
 * Snowball.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: dkwan
 */

#include "Snowball.h"

Snowball::Snowball() {
	// TODO Auto-generated constructor stub
	sampled_size = 0;
}

vector<int> Snowball::random_ints(int n, int k) {
	k = min(k, n);
	vector<int> v(n);
	for (int i = 0; i < n; ++i) {
		v[i] = i;
	}

	default_random_engine gen(time(NULL));
	std::mt19937 g(gen());
	std::shuffle(v.begin(), v.end(), g);
	return vector<int>(v.begin(), v.begin() + k);
}

EdgeGraph Snowball::snowball_sampling(const AdjLinkGraph& graph, int arg_N,
		int arg_K, int arg_T) {
	sampled_size = 0;
	EdgeGraph g_sample;
	const auto& v0_vec = random_ints(graph.get_num_of_nodes(), arg_N);
	unordered_set<int> v_prev(v0_vec.begin(), v0_vec.end());
	unordered_set<int> v_selected(v_prev);

	for (int i = 1; i <= arg_T; ++i) {
		unordered_set<int> v_i;

		for (const auto& vertex : v_prev) {
			const auto& edge_indices = random_ints(graph.get_num_edge_of_node(vertex), arg_K);
			for (const auto& idx : edge_indices) {
				const auto& neighbor = graph.get_neighbor_of_node(vertex, idx);
				v_i.insert(neighbor);
				g_sample.push_back(Edge(vertex, neighbor, 1));
			}
		}
		v_prev.clear();
		for (const auto& v : v_i) {
			if (!v_selected.count(v)) {
				v_prev.insert(v);
				v_selected.insert(v);
			}
		}
	}
	sampled_size = v_selected.size();
	subset = v_selected;
	return g_sample;
}

EdgeGraph Snowball::snowball_sampling_with_size(const AdjLinkGraph& graph,
		int arg_N, int arg_K, int arg_SN) {
	sampled_size = 0;
	EdgeGraph g_sample;
	const auto& v0_vec = random_ints(graph.get_num_of_nodes(), arg_N);
	unordered_set<int> v_prev(v0_vec.begin(), v0_vec.end());
	unordered_set<int> v_selected(v_prev);
	int iterations = 0;
	while((int)v_selected.size() < arg_SN && iterations <= arg_SN) {
		unordered_set<int> v_i;
		// work as a buffer
		EdgeGraph temp;
		for (const auto& vertex : v_prev) {
			const auto& edge_indices = random_ints(graph.get_num_edge_of_node(vertex), arg_K);
			for (const auto& idx : edge_indices) {
				const auto& neighbor = graph.get_neighbor_of_node(vertex, idx);
				v_i.insert(neighbor);
				temp.push_back(Edge(vertex, neighbor, 1));
			}
		}
		v_prev.clear();
		for (const auto& v : v_i) {
			if (!v_selected.count(v)) {
				v_prev.insert(v);
				v_selected.insert(v);
			}
			if((int)v_selected.size() >= arg_SN)
				break;
		}
		for (auto& edge : temp) {
			if(v_selected.count(edge.u) && v_selected.count(edge.v)) {
				g_sample.push_back(Edge(edge.u, edge.v, edge.w));
			}
		}
		iterations++;
	}
	sampled_size = v_selected.size();
	subset = v_selected;
	return g_sample;
}

Snowball::~Snowball() {
	// TODO Auto-generated destructor stub
}

