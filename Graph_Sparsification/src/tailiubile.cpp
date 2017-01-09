#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> PII;
typedef vector<vector<PII>> Graph;
const int DEFAULT_EDGE_VALUE = 1;

vector<int> random_ints(int n, int k) {
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

Graph snow_ball_sampling(const Graph& g, int arg_N, int arg_K, int arg_T) {
	Graph g_sample(g.size());
	const auto& v0_vec = random_ints(g.size(), arg_N);
	unordered_set<int> v_prev(v0_vec.begin(), v0_vec.end());
	unordered_set<int> v_selected(v_prev);
	cout << "initial: ";
	for (const auto& var : v_selected) {
		cout << var << " ";
	}
	cout << endl;
	for (int i = 1; i <= arg_T; ++i) {
		unordered_set<int> v_i;
		cout << "prev: ";
		for (const auto& var : v_prev) {
			cout << var << " ";
		}
		cout << endl;
		for (const auto& vertex : v_prev) {
			const auto& edge_indices = random_ints(g[vertex].size(), arg_K);
			for (const auto& idx : edge_indices) {
				const auto& neighbor = g[vertex][idx].first;
				v_i.insert(neighbor);
				g_sample[vertex].push_back(g[vertex][idx]);
				g_sample[g[vertex][idx].first].push_back( { vertex,
						g[vertex][idx].second });
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

vector<int> read_edge_info(const string& s) {
	stringstream ss(s);
	vector<int> data;
	int val;
	while (ss >> val) {
		data.push_back(val);
	}
	return data;
}

vector<vector<int>> re_number_edge(const vector<vector<int>>& edges) {
	unordered_map<int, int> index_map;
	vector<vector<int>> new_edges;
	for (auto edge : edges) {
		if (!index_map.count(edge[0])) {
			index_map[edge[0]] = index_map.size() - 1;
		}
		if (!index_map.count(edge[1])) {
			index_map[edge[1]] = index_map.size() - 1;
		}
		edge[0] = index_map[edge[0]];
		edge[1] = index_map[edge[1]];
		new_edges.push_back(edge);
	}
	return new_edges;
}

int get_max_node_index(const vector<vector<int>>& edges) {
	int max_index = 0;
	for (const auto& edge : edges) {
		max_index = max(edge[0], max_index);
		max_index = max(edge[1], max_index);
	}
	return max_index;
}

Graph read_graph(const string& file_path) {
	fstream fin(file_path);
	Graph g;
	vector<vector<int>> edges;
	string str;
	while (getline(fin, str)) {
		const auto& edge = read_edge_info(str);
		edges.push_back(edge);
	}
	edges = re_number_edge(edges);
	cout << get_max_node_index(edges) << endl;
	g.resize(get_max_node_index(edges) + 1);
	for (const auto& edge : edges) {
		if (edge.size() <= 2) {
			g[edge[0]].emplace_back(edge[1], DEFAULT_EDGE_VALUE);
			g[edge[1]].emplace_back(edge[0], DEFAULT_EDGE_VALUE);
		} else {
			g[edge[0]].emplace_back(edge[1], edge[2]);
			g[edge[1]].emplace_back(edge[0], edge[2]);
		}
	}
	return g;
}

int main() {
	auto g = read_graph("input.txt");
	for (size_t i = 0; i < g.size(); ++i) {
		for (const auto& edge : g[i]) {
			cout << i << " " << edge.first << " " << edge.second << endl;
		}
	}
	auto sample = snow_ball_sampling(g, 1, 1, 2);
	for (size_t i = 0; i < sample.size(); ++i) {
		for (const auto& edge : sample[i]) {
			cout << i << " " << edge.first << endl;
		}
	}
	return 0;
}
