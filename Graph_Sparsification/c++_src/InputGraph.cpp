/*
 * Sampling.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "InputGraph.h"

InputGraph::InputGraph(string filename) {
	N = M = 0;
	this->filename = filename;
	read_in_graph();
}

int InputGraph::num_nodes() {
	return this->N;
}

vector<string> InputGraph::split(string s) {
	std::istringstream iss(s);
	vector<string> ret;
	do {
		std::string sub;
		iss >> sub;
		ret.push_back(sub);
	} while (iss);
	return ret;
}

void InputGraph::read_in_graph() {
	ifstream fin;
	fin.open(filename);
	fin >> N;
	graph = AdjLinkGraph(false, N);
	int u, v;
	double w;
	string temp;
	vector<string> t;
	std::getline(fin, temp);
	std::getline(fin, temp);

	t = split(temp);
//	for(auto &s : t) {
//		cout << s << endl;
//	}

	if (t.size() == 4) {
		cout << "Input file is weighted." << endl;
		graph.weighted = true;
		graph.insert_link(std::stoi(t[0]), std::stoi(t[1]), std::stod(t[2]));
		graph.insert_link(std::stoi(t[1]), std::stoi(t[0]), std::stod(t[2]));
		M++;
		while (fin >> u >> v >> w) {
			graph.insert_link(u, v, w);
			graph.insert_link(v, u, w);
			M++;
		}
		graph.init_degree();
	} else if (t.size() == 3) {
		cout << "Input file is unweighted." << endl;
		graph.weighted = false;
		graph.insert_link(std::stoi(t[0]), std::stoi(t[1]));
		graph.insert_link(std::stoi(t[1]), std::stoi(t[0]));
		M++;
		while (fin >> u >> v >> w) {
			graph.insert_link(u, v);
			graph.insert_link(v, u);
			M++;
		}
		graph.init_degree();
	} else {
		cout << "Input file is not a valid adjacent link table of a graph."
				<< endl;
	}
	fin.close();
}

vector<int> InputGraph::degree_random_sample(int S) {
	// cerr << "in" << endl;
	const double ERROR = 1e-6;
	vector<int> rets;
	vector<double> sum_deg(N + 1);
	bool visited[N];
	memset(visited, false, sizeof(visited));

	vector<double> temp = this->graph.degrees;

	for (int i = 1; i <= N; i++) {
		sum_deg[i] = sum_deg[i - 1] + temp[i - 1];
	}

	double tsum = sum_deg[N];

	srand(time(NULL));

	while (true) {
		double randd = (double) rand() / RAND_MAX * tsum;
		int index = lower_bound(sum_deg.begin(), sum_deg.end(), randd - ERROR)
				- sum_deg.begin() - 1;
		index = max(0, index);

		if (!visited[index]) {
			visited[index] = true;
			rets.push_back(index);
		}

		if ((int) rets.size() == S)
			break;
	}
	// cerr << "out" << endl;
	return rets;
}

vector<double> InputGraph::sp_distribution(unordered_set<int> &indexes, int cut_off) {
	vector<int> vindexes(indexes.size());
	copy(indexes.begin(), indexes.end(), vindexes.begin());
	return sp_distribution(vindexes, cut_off);
}

vector<double> InputGraph::sp_distribution(vector<int> &indexes, int cut_off) {
	// 0 - cut_off
	cout << "Calculating subset sp distribution.." << endl;
	vector<double> rets(cut_off + 1);
	fill(rets.begin(), rets.end(), 0);
	unordered_set<int> map(indexes.begin(), indexes.end());
	double num_nodes = indexes.size();
	double num_pairs = num_nodes * (num_nodes - 1);
	for (int i = 0; i < num_nodes; i++) {
		// cout << i << endl;
		sp_bfs(rets, map, indexes[i], cut_off);
	}
	for(auto& v : rets) {
		v /= num_pairs;
	}
	return rets;
}

void InputGraph::sp_bfs(vector<double> &rets, unordered_set<int> &indexes,
		int p, int cut_off) {
	queue<pair<int, int>> q;
	q.push(make_pair(p, 0));
	bool visited[N];
	memset(visited, false, sizeof(visited));
	visited[p] = true;
	while (!q.empty()) {
		auto t = q.front();
		if (t.second != 0 && indexes.count(t.first)) {
			rets[t.second]++;
		}

		for (auto next : graph.adjlink[t.first]) {
			int np = next.v;
			if (!visited[np]) {
				int ndist = t.second + 1;
				if (ndist <= cut_off) {
					visited[np] = true;
					q.push(make_pair(np, ndist));
				}
			}
		}
		q.pop();
	}
}

InputGraph::~InputGraph() {
// TODO Auto-generated destructor stub
}

