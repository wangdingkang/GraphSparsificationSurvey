/*
 * ForestFireSampling.cpp
 *
 *  Created on: Jan 13, 2017
 *      Author: dkwan
 */

#include "ForestFireSampling.h"

ForestFireSampling::ForestFireSampling() {
	// TODO Auto-generated constructor stub

}

EdgeGraph ForestFireSampling::get_sampled_graph(const AdjLinkGraph& g,
		int arg_N, int arg_K, int arg_T) {
	GeometricRandomGenerator generator(arg_K);
	EdgeGraph g_sample;
	const auto& v0_vec = random_ints(g.size(), arg_N);
	unordered_set<int> v_prev(v0_vec.begin(), v0_vec.end());
	unordered_set<int> v_selected(v_prev);
	for (int i = 1; i <= arg_T; ++i) {
		unordered_set<int> v_i;
		for (const auto& vertex : v_prev) {
			const auto& edge_indices = random_ints(g[vertex].size(),
					generator());
			for (const auto& idx : edge_indices) {
				const auto& neighbor = g[vertex][idx];
				v_i.insert(neighbor);
				g_sample.push_back(Edge(vertex, neighbor, 1));
//				g_sample[vertex].push_back(g[vertex][idx]);
//				g_sample[g[vertex][idx].first].push_back( { vertex,
//						g[vertex][idx].second });
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
	return g_sample;
}

EdgeGraph ForestFireSampling::ffs_sampling_with_size(const AdjLinkGraph &g,
		int arg_N, int arg_K, int arg_SN) {
	GeometricRandomGenerator generator(arg_K);
	EdgeGraph g_sample;
	const auto& v0_vec = random_ints(g.size(), arg_N);
	unordered_set<int> v_prev(v0_vec.begin(), v0_vec.end());
	unordered_set<int> v_selected(v_prev);
	while((int)v_selected.size() < arg_SN) {
		unordered_set<int> v_i;
		for (const auto& vertex : v_prev) {
			const auto& edge_indices = random_ints(g[vertex].size(),
					generator());
			for (const auto& idx : edge_indices) {
				const auto& neighbor = g[vertex][idx];
				v_i.insert(neighbor);
				g_sample.push_back(Edge(vertex, neighbor, 1));
				//				g_sample[vertex].push_back(g[vertex][idx]);
				//				g_sample[g[vertex][idx].first].push_back( { vertex,
				//						g[vertex][idx].second });
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
	return g_sample;
}

ForestFireSampling::~ForestFireSampling() {
	// TODO Auto-generated destructor stub
}

