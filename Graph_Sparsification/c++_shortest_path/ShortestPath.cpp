//============================================================================
// Name        : ShortestPath.cpp
// Author      : WDK
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;
#define INF 10000000
#define CUTOFF 25
#define INPUT_FOLDER "input/"
#define OUTPUT_FOLDER "output/"
#define ITERATION 100

struct Adj_Edge {
	int to;
	int weight;
	Adj_Edge(int _to, int _weight) {
		to = _to;
		weight = _weight;
	}
};

struct Edge {
	int from;
	int to;
	int weight;
	Edge(int _from, int _to, int _weight) {
		from = _from;
		to = _to;
		weight = _weight;
	}
};

struct Graph {
	int N;
	vector<vector<Adj_Edge> > adj_list;

	Graph(int _N) {
		N = _N;
		adj_list = vector<vector<Adj_Edge>>(N, vector<Adj_Edge>());
	}

	void init(vector<Edge> input_edges) {
		for (auto& edge : input_edges) {
			adj_list[edge.from].push_back(Adj_Edge(edge.to, edge.weight));
			adj_list[edge.to].push_back(Adj_Edge(edge.from, edge.weight));
		}
	}

};

vector<int> dijk(int A, Graph &g, int cut_off) {
	int n = g.N;
	vector<int> dist(n);
	vector<bool> vis(n);

	vector<int> ret(cut_off + 1, 0);

	for (int i = 0; i < n; ++i) {
		dist[i] = INF;
	}
	dist[A] = 0;

	for (int i = 0; i < n; ++i) {
		int cur = -1;
		for (int j = 0; j < n; ++j) {
			if (vis[j])
				continue;
			if (cur == -1 || dist[j] < dist[cur]) {
				cur = j;
			}
		}
		if (dist[cur] > cut_off)
			break;
		vis[cur] = true;
		ret[dist[cur]]++;
		for (auto& edge : g.adj_list[cur]) {
			int path = dist[cur] + edge.weight;
			if (path < dist[edge.to]) {
				dist[edge.to] = path;
			}
		}
	}
	ret[0] = 0;
	return ret;
}

vector<string> fetch_all_input_files(const string input_folder) {
	struct dirent *entry;
	DIR *dp;
	vector<string> filenames;
	dp = opendir(input_folder.c_str());
	if (dp == NULL) {
		cerr << "Fold not exist." << endl;
		return filenames;
	}

	while ((entry = readdir(dp))) {
		string temp = entry->d_name;
		if (temp.find(".txt") != string::npos)
			filenames.push_back(temp);
	}

	closedir(dp);
	return filenames;
}

Graph read_in_graph(const string& filename) {
	ifstream fin;
	fin.open(filename.c_str());
	vector<Edge> edges;
	int N = 0;
	int u, v;
	int w;
	while (fin >> u >> v >> w) {
		N = max(N, u + 1);
		N = max(N, v + 1);
		edges.push_back(Edge(u, v, w));
	}
	fin.close();
	Graph g = Graph(N);
	g.init(edges);
	return g;
}

void output_ret(string filename, map<string, vector<double>> rets) {
	ofstream fout;
	fout.open(filename.c_str());
	fout << std::fixed << std::setprecision(4);
	for (auto it = rets.begin(); it != rets.end(); ++it) {
		string key = it->first;
		vector<double> ret = it->second;
		int len = ret.size();
		for (int i = 0; i < len - 1; i++) {
			fout << ret[i] / ret[len - 1] << " ";
		}
		fout << key << endl;
	}
	fout.close();
}

int main() {
	vector<string> filenames = fetch_all_input_files(INPUT_FOLDER);
	string network_name;
	// 0 - CUTOFF store the frequencies, CUTOFF + 1 stores # of iterations.
	map<string, vector<double>> rets;

	for (auto& filename : filenames) {
		network_name = filename.substr(filename.find_last_of('_') + 1);
		Graph g = read_in_graph(INPUT_FOLDER + filename);
		double num_pair = (double) g.N * (g.N - 1);
		string file_key = filename.substr(0,
				filename.find('_', filename.find('_') + 1));
		vector<int> ret(CUTOFF + 2, 0);
		for (int i = 0; i < g.N; i++) {
			vector<int> one_ret = dijk(i, g, CUTOFF);
			for(int j = 0; j<=CUTOFF; j++) {
				ret[j] += one_ret[j];
			}
		}

		if (!rets.count(file_key)) {
			rets[file_key] = vector<double>(CUTOFF + 2, 0.0);
		}

		vector<double> &stat = rets[file_key];
		for (int i = 0; i <= CUTOFF; i++) {
			stat[i] += (double) ret[i] / num_pair;
		}
		stat[CUTOFF + 1]++;

	}

	output_ret(OUTPUT_FOLDER + network_name + "_sp.txt", rets);

	return 0;
}
